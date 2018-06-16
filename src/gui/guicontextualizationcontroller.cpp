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
    bool ok;
    QString selected;
    QStringList comboBoxOptions;
    QList <FirmwareString *> stringsFound;

    stringsFound = this->findString(newString, (FindType)findType);
    switch (stringsFound.size()) {
        case 0:
            if (findType == ByValue) {
                response = Utils::warningMessage(
                    "Impossible to find the string in " + fpFile_ + " file.",
                    "Are you sure to add the string?"
                );
                if (response == QMessageBox::Yes) {
                    this->addString(
                        new FirmwareString(
                            QString(""),
                            newString,
                            QString(""),
                            QString::number(newString.size()),
                            "TODO",
                            false
                        )
                    );
                }
            } else if (findType == ByID) {
                Utils::informativeMessage("Not found.", "No string was be found with this ID.");
            }
            //TODO: Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");
        break;

        case 1:
            this->addString(stringsFound.first());
            break;

        default:
            this->eraseExistStrings(&stringsFound);
            switch (stringsFound.size()) {
                case 0:
                    Utils::informativeMessage("Not found.", "All strings found already are in the table.");
                    response = Utils::warningMessage(
                        "All strings found already are in the table.",
                        "Do you add a new string with this value?"
                    );
                    if (response == QMessageBox::Yes) {
                        this->addString(
                            new FirmwareString(
                                QString(""),
                                newString,
                                QString(""),
                                QString::number(newString.size()),
                                "TODO",
                                false
                            )
                        );
                    }
                    break;

                case 1:
                    this->addString(stringsFound.first());
                    break;

                default:
                    foreach (FirmwareString *fwString, stringsFound) {
                        comboBoxOptions << fwString->getId();
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

                    foreach (FirmwareString *fwString, stringsFound) {
                        if (ok && fwString->getId() == selected) {
                            this->addString(fwString);
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
    this->removeString(stringId);
}

void GuiContextualizationController::clear()
{
    this->removeAllStrings();
}

void GuiContextualizationController::capture()
{
    QString path;

    view_->setVisible(false);
    path = this->takeCaptureArea();
    this->setImage(path);
    view_->setVisible(true);
}

void GuiContextualizationController::load()
{
    QFileDialog dialog(
        Q_NULLPTR,
        tr("Open Image"),
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)")
    );

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        this->setImage(dialog.selectedFiles().first());
    }
}

void GuiContextualizationController::detect()
{
    QStringList *extractedStrings;

    extractedStrings = this->detectStringsOnImage();
    this->processStrings(*extractedStrings);

    delete extractedStrings;
}

void GuiContextualizationController::send()
{
    QString contextualizationPath;
    int hasError;

    switch (validateModel()) {
        case OkModel:
            contextualizationPath = this->generateContextualization();
            if (contextualizationPath.isEmpty()) {
                Utils::errorMessage("Fail to send", "Failure to package contextualization.");

                return;
            }

            //TODO: pedir login de alguna forma.
            ///< Any errors are processed in the function.
            hasError = this->sendContextualization(contextualizationPath, username_, "1234");
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
        ///< Remove temporal image.
        QFile file(model_->getImage());
        if (file.exists())
            file.remove();

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
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        tr("Contextualization File (*.json)")
    );

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        path = dialog.selectedFiles().first();
        path = path.endsWith(QString(".json")) ? path : path + ".json";
        this->exportToJsonFile(path);
    }
}

void GuiContextualizationController::open()
{
    QFileDialog dialog(
        Q_NULLPTR,
        tr("Open Image"),
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        tr("Contextualization File (*.json)")
    );

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        this->importProjectFromJsonFile(dialog.selectedFiles().first());
    }
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
            SIGNAL(captureRequested()),
            this,
            SLOT(capture())
        );
        QObject::connect(
            view_,
            SIGNAL(loadImageRequested()),
            this,
            SLOT(load())
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
    }
}
