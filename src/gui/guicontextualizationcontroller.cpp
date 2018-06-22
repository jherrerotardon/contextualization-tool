#include "guicontextualizationcontroller.h"

GuiContextualizationController::GuiContextualizationController(QQuickWindow *view, QObject *parent)
    : ContextualizationControllerBase(parent)
{
    view_ = view;
}

GuiContextualizationController::~GuiContextualizationController()
{
    delete model_;
}

void GuiContextualizationController::add(QString newString, int findType)
{
    int response;
    int error;
    bool ok;
    QString selected;
    QStringList comboBoxOptions;
    QList <FirmwareString *> stringsFound;

    if (newString.isEmpty()){
        Utils::errorMessage("You must introduce some string.", "An empty string cannot be added.");

        return;
    }

    stringsFound = findString(newString, (FindType)findType);
    switch (stringsFound.size()) {
    // Case where string not found in fp file.
    case 0:
        if (findType == ByValue) {
            response = Utils::warningMessage(
                "Impossible to find the string in " + DONE_FP_FILE + " file.",
                "Are you sure to add the string?"
            );
            if (response == QMessageBox::Yes) {
                error = addString(
                    new FirmwareString(
                        QString(""),
                        newString,
                        QString(""),
                        QString::number(newString.size()),
                        "DONE",
                        false
                    )
                );

                if (error == StringAlreadyExists) {
                    Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");
                }
            }
        } else if (findType == ByID) {
            Utils::informativeMessage("Not found.", "No string was be found with this ID.");
        }

    break;

    // Case where more than one string was found in fp file.
    default:
        // Remove found strings that already be in the model.
        eraseExistStrings(&stringsFound);

        switch (stringsFound.size()) {
        // Case where all strings found alraedy be in the model.
        case 0:
            response = Utils::warningMessage(
                "All strings found already are in the table.",
                "Do you want to add a new string with this value?"
            );
            if (response == QMessageBox::Yes) {
                error = addString(
                    new FirmwareString(
                        QString(""),
                        newString,
                        QString(""),
                        QString::number(newString.size()),
                        "DONE",
                        false
                    )
                );

                if (error == StringAlreadyExists) {
                    Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");
                }
            }
            break;

        // Case where of all the strings found int fp file only one is not already in the model
        case 1:
            addString(stringsFound.first());
            break;

        // Case where more than one string was found in fp file and are not in the model yet.
        default:
            // Load strings found in combo box to ask user which one he was looking for
            foreach (FirmwareString *fwString, stringsFound) {
                comboBoxOptions << fwString->getId() + " - " + fwString->getState();
            }

            selected = QInputDialog::getItem(
                Q_NULLPTR,
                tr("Warning!!"),
                tr("More than one string found with this value. Select the ID of the string you want."),
                comboBoxOptions,
                0,
                false,
                &ok
            );

            // Add string selected by user and delete all others.
            foreach (FirmwareString *fwString, stringsFound) {
                if (ok && fwString->getId() == selected) {
                    addString(fwString);
                } else {
                    delete fwString;
                }
            }

            break;
        }
    }
}

void GuiContextualizationController::remove(QString stringId)
{
    removeString(stringId);
}

void GuiContextualizationController::clear()
{
    removeAllStrings();
}

void GuiContextualizationController::capture(bool detectStringsOnLoad)
{
    QString path;

    view_->setVisible(false);
    path = takeCaptureArea();
    view_->setVisible(true);

    if (!path.isEmpty()) {
        // If setImage is succesfully and have to detect strings, call function to detect strings.
        if (setImage(path)) {
            if (detectStringsOnLoad) {
                detect();
            }
        } else {
            Utils::errorMessage("Can't set image.", "Had a problem with the capture.");
        }
    }
}

void GuiContextualizationController::load(bool detectStringsOnLoad)
{
    QString selectedImage;

    QFileDialog dialog(
        Q_NULLPTR,
        tr("Open Image"),
        QDir::homePath(),
        tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)")
    );

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        selectedImage = dialog.selectedFiles().first();

        // If setImage is succesfully and have to detect strings, call function to detect strings.
        if (setImage(selectedImage)) {
            if (detectStringsOnLoad) {
                    detect();
            }
        } else {
            Utils::errorMessage("Can't set image.", "Not exists the image: " + selectedImage);
        }
    }
}

void GuiContextualizationController::detect()
{
    QStringList *extractedStrings;

    extractedStrings = detectStringsOnImage();
    if (extractedStrings) {
        processStrings(*extractedStrings);
    } else {
        Utils::errorMessage("It's not possible to detect texts in the image.", "See the log for more details.");
    }


    delete extractedStrings;
}

void GuiContextualizationController::send()
{
    QString contextualizationPath;
    QString username;
    QString password;
    int hasError;

    switch (validateModel()) {
        case OkModel:
            contextualizationPath = generateContextualization();
            if (contextualizationPath.isEmpty()) {
                Utils::errorMessage("Fail to send", "Failure to package contextualization.");

                return;
            }

            username = requestUsername();
            // If user press cancel, remove contextualization file and cancel process.
            if (username.isEmpty()) {
                QFile::remove(contextualizationPath);

                return;
            }

            password = requestPassword();
            // If user press cancel, remove contextualization file and cancel process.
            if (password.isEmpty()) {
                QFile::remove(contextualizationPath);

                return;
            }

            hasError = sendContextualization(contextualizationPath, username, password);

            // Remove contextualization file.
            QFile::remove(contextualizationPath);

            if (hasError) {
                //TODO: filtrar cada error.
            } else {
                Utils::informativeMessage("Finished.", "Send process has finished succesfully.");
            }

            break;
        case NoImage:
            Utils::errorMessage("Fail to send!!", "There isn't an image asociated.");
            break;
        case ImageNotExist:
            Utils::errorMessage("Fail to send!!", "The image associated doesn't exist. Please reload the image.");
            break;
        case NoStrings:
            Utils::errorMessage("Fail to send!!", "There aren't any string asociated.");
            break;
        default:
            Utils::errorMessage("Fail to send!!", "Unknown error.");
            break;
    }
}

void GuiContextualizationController::cancel()
{
    int response;

    response = Utils::warningMessage("Are you sure?", "If you not save the proyect it will be deleted.");
    if (response == QMessageBox::Yes) {
        saveConfig();

        QFile::remove(DONE_FP_FILE);

        QApplication::quit();
    }
}

void GuiContextualizationController::save()
{

}

void GuiContextualizationController::saveAs()
{
    QString path;
    QFileDialog dialog(
        Q_NULLPTR,
        tr("Save Project"),
        QDir::homePath(),
        tr("Contextualization File (*.json)")
    );

    // If the proyect if empty nothing will be saved.
    if (model_->isEmpty()) {
        Utils::informativeMessage("Empty contextualization.", "Nothing to be save.");

        return;
    }

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        path = dialog.selectedFiles().first();
        path = path.endsWith(QString(".json")) ? path : path + ".json";
        exportToJsonFile(path);
    }
}

void GuiContextualizationController::open()
{
    QFileDialog dialog(
        Q_NULLPTR,
        tr("Open Image"),
        QDir::homePath(),
        tr("Contextualization File (*.json)")
    );

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        importProjectFromJsonFile(dialog.selectedFiles().first());
    }
}

void GuiContextualizationController::configFpFile()
{
    bool ok;
    QString englishFpFile;

    do {
        englishFpFile = QInputDialog::getText(
            Q_NULLPTR,
            tr("Configuration"),
            tr("English fp path:"),
            QLineEdit::Normal,
            englishFpFile_,
            &ok
        );
    } while(ok && englishFpFile.isEmpty());

    // Only change value of path if user doesn't press cancel. After done fp file is generated.
    if (ok) {
        englishFpFile_ = englishFpFile;

        generateDoneFpFile();
    }
}

void GuiContextualizationController::configRemoteHost()
{
    bool ok;
    QString remoteHost;

    do {
        remoteHost = QInputDialog::getText(
            Q_NULLPTR,
            tr("Configuration"),
            tr("English fp path:"),
            QLineEdit::Normal,
            remoteHost_,
            &ok
        );
    } while(ok && remoteHost.isEmpty());

    // Only change value of remote host if user doesn't press cancel.
    remoteHost_ = ok ? remoteHost : remoteHost_;
}

void GuiContextualizationController::configValidStates()
{
    bool ok;
    QString validStates;

    do {
        validStates = QInputDialog::getText(
            Q_NULLPTR,
            tr("Configuration"),
            tr("English fp path:"),
            QLineEdit::Normal,
            validStates_.join(','),
            &ok
        );
    } while(ok && validStates.isEmpty());

    // Only change value of remote host if user doesn't press cancel.
    validStates_ = ok ? validStates.split(',', QString::SkipEmptyParts) : validStates_;
}

void GuiContextualizationController::refresh()
{
    emit imageChanged();
    emit stringsListChanged();

    generateDoneFpFile();
}

QQuickWindow *GuiContextualizationController::getView()
{
    return view_;
}

void GuiContextualizationController::setView(QQuickWindow *view)
{
    view_ = view;
    connectSignalsAndSlots();

    emit viewChanged();
}

QString GuiContextualizationController::requestUsername()
{
    QString username;
    bool ok;

    // Request username to user.
    do {
        username = QInputDialog::getText(
            Q_NULLPTR,
            tr("Login"),
            tr("User Name:"),
            QLineEdit::Normal,
            username_,
            &ok
        );
    } while(ok && username.isEmpty());

    return ok ? username : QString();
}

QString GuiContextualizationController::requestPassword()
{
    QString password;
    bool ok;

    // Request username to user.
    do {
        password = QInputDialog::getText(
            Q_NULLPTR,
            tr("Login"),
            tr("Password:"),
            QLineEdit::Password,
            QString(),
            &ok
        );
    } while(ok && password.isEmpty());

    return ok ? password : QString();
}

void GuiContextualizationController::connectSignalsAndSlots()
{
    if (view_) {
        // Connect signals with slots
        QObject::connect(
            view_,
            SIGNAL(clearRequested()),
            this,
            SLOT(clear())
        );
        QObject::connect(
            view_,
            SIGNAL(addRequested(QString, int)),
            this,
            SLOT(add(QString, int))
        );
        QObject::connect(
            view_,
            SIGNAL(stringRemoved(QString)),
            this,
            SLOT(remove(QString))
        );
        QObject::connect(
            view_,
            SIGNAL(cancelRequested()),
            this,
            SLOT(cancel())
        );
        QObject::connect(
           view_,
            SIGNAL(sendRequested()),
            this,
            SLOT(send())
        );
        QObject::connect(
            view_,
            SIGNAL(captureRequested(bool)),
            this,
            SLOT(capture(bool))
        );
        QObject::connect(
            view_,
            SIGNAL(loadImageRequested(bool)),
            this,
            SLOT(load(bool))
        );
        QObject::connect(
            view_,
            SIGNAL(detectStringsRequested()),
            this,
            SLOT(detect())
        );
        QObject::connect(
            view_,
            SIGNAL(openRequested()),
            this,
            SLOT(open())
        );
        QObject::connect(
            view_,
            SIGNAL(saveAsRequested()),
            this,
            SLOT(saveAs())
        );
        QObject::connect(
            view_,
            SIGNAL(fpFileConfigRequested()),
            this,
            SLOT(configFpFile())
        );
        QObject::connect(
            view_,
            SIGNAL(remoteHostConfigRequested()),
            this,
            SLOT(configRemoteHost())
        );
        QObject::connect(
            view_,
            SIGNAL(validStatesConfigRequested()),
            this,
            SLOT(configValidStates())
        );
        QObject::connect(
            view_,
            SIGNAL(refreshRequested()),
            this,
            SLOT(refresh())
        );
    }
}
