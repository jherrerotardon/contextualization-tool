#include "guicontextualizationcontroller.h"
// TODO: remove includes on bottom of this comment
#include <QDebug>

GuiContextualizationController::GuiContextualizationController(QObject *view, QObject *parent)
    : QObject(parent), ContextualizationControllerBase(parent)
{
    QObject *stringsTable;

    this->view = view;

    if (view != Q_NULLPTR) {
        ///< Connect signals on model to refresh the view.
        QObject::connect(
            this->model,
            SIGNAL(imageChanged()),
            this,
            SLOT(refreshImageView())
        );
        QObject::connect(
            this->model,
            SIGNAL(stringsListChanged()),
            this,
            SLOT(refreshTableView())
        );
        QObject::connect(
            this->model,
            SIGNAL(modelChanged()),
            this,
            SLOT(refreshView())
        );

        ///< Initialize TableView and his model.
        this->tableModel = new StringsTableModel(this->model->getStringsList());
        stringsTable = view->findChild<QObject *>("stringsTable");
        if (stringsTable) {
            stringsTable->setProperty("model", QVariant::fromValue(this->tableModel));
        }

        //< Connect signals and slots
        QObject::connect(
            view->findChild<QObject *>("clearButton"),
            SIGNAL(clicked()),
            this,
            SLOT(clear())
        );
        QObject::connect(
            view->findChild<QObject *>("addStringButton"),
            SIGNAL(customClicked(QString, int)),
            this,
            SLOT(add(QString, int))
        );
        QObject::connect(
            view->findChild<QObject *>("buttonsColumn"),
            SIGNAL(buttonClicked(int)),
            this,
            SLOT(remove(int))
        );
        QObject::connect(
            view->findChild<QObject *>("cancelButton"),
            SIGNAL(clicked()),
            this,
            SLOT(cancel())
        );
        QObject::connect(
            view->findChild<QObject *>("sendButton"),
            SIGNAL(clicked()),
            this,
            SLOT(send())
        );
        QObject::connect(
            view->findChild<QObject *>("captureAreaButon"),
            SIGNAL(clicked()),
            this,
            SLOT(capture())
        );
        QObject::connect(
            view->findChild<QObject *>("loadImageButton"),
            SIGNAL(clicked()),
            this,
            SLOT(load())
        );
        QObject::connect(
            view->findChild<QObject *>("detectStringsButton"),
            SIGNAL(clicked()),
            this,
            SLOT(detect())
        );
        QObject::connect(
            view->findChild<QObject *>("importButton"),
            SIGNAL(triggered()),
            this,
            SLOT(open())
        );
        QObject::connect(
            view->findChild<QObject *>("exportButton"),
            SIGNAL(triggered()),
            this,
            SLOT(saveAs())
        );
    }
}

GuiContextualizationController::~GuiContextualizationController()
{
    delete this->model;
    delete this->tableModel;
}

StringsTableModel * GuiContextualizationController::getTableModel()
{
    return tableModel;
}

void GuiContextualizationController::setTableModel(StringsTableModel *tableModel)
{
    this->tableModel = tableModel;
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
                    "Impossible to find the string in " + this->fpFile + " file.",
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

void GuiContextualizationController::remove(int row)
{
    this->removeString(row);
}

void GuiContextualizationController::clear()
{
    this->removeAllStrings();
}

void GuiContextualizationController::capture()
{
    QString path;

    ((QWindow *)view)->setVisible(false);
    path = this->takeCaptureArea();
    this->setImage(path);
    ((QWindow *)view)->setVisible(true);
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
            hasError = this->sendContextualization(contextualizationPath, this->username, "1234");
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
        QFile file(this->model->getImage());
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

void GuiContextualizationController::refreshView()
{
    this->refreshImageView();
    this->refreshTableView();
}

void GuiContextualizationController::refreshImageView()
{
    QObject *containerImage;
    bool exists;

    containerImage = this->view->findChild<QObject *>("containerImage");
    if (containerImage) {
        exists = QFile(this->model->getImage()).exists();

        containerImage->setProperty("source", "");
        containerImage->setProperty(
            "source",
            "file:" + (exists ? this->model->getImage() : ContextualizationModel::NO_IMAGE_PATH)
        );
    }
}

void GuiContextualizationController::refreshTableView()
{
    if (this->tableModel) {
        this->tableModel->refreshView();
    }
}
