#include "guicontextualizationcontroller.h"
// TODO: remove includes on bottom of this comment
#include <QDebug>

GuiContextualizationController::GuiContextualizationController(QObject *view, QObject *parent)
    : QObject(parent), ContextualizationControllerBase(parent)
{
    QObject *stringsTable;

    this->view = view;

    if (view != nullptr) {
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
            SIGNAL(customClicked(QString)),
            this,
            SLOT(add(QString))
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

void GuiContextualizationController::add(QString newString)
{
    int response;
    QString emptyString("");
    QString state;
    FirmwareString *fwString;

    fwString = this->findString(newString);
    if (fwString) {
        this->addString(fwString);
        //TODO: Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");

    } else {
        response = Utils::warningMessage(
            "Impossible to find the string in " + this->fpFile + " file.",
            "Are you sure to add the string?"
        );
        if (response == QMessageBox::Yes) {
            state = "TODO";
            fwString = new FirmwareString(
                emptyString,
                newString,
                emptyString,
                QString::number(newString.size()),
                state,
                false
            );
            this->addString(fwString);
            //TODO: Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");
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
    QString text;
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
        text = this->model->toJson();
        Utils::writeFile(path, text);
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



