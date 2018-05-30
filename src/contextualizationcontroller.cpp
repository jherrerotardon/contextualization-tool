#include "contextualizationcontroller.h"
/************/
#include <QDebug>
/***************/

ContextualizationController::ContextualizationController(QObject *view, QObject *parent) : QObject(parent)
{
    QObject *stringsTable;

    this->model = new ContextualizationModel();
    this->validStates << "TODO" << "DONE" << "VALIDATED";
    this->fpFile = "/home/jorge/Descargas/english.fp";
    this->username = qgetenv("USER");
    this->view = view;

    if (view != nullptr) {
        //Initialize TableView and his model.
        this->tableModel = new StringsTableModel(this->model->getStringsList());
        stringsTable = view->findChild<QObject *>("stringsTable");
        if (stringsTable)
            stringsTable->setProperty("model", QVariant::fromValue(this->tableModel));

        //Connect signals and slots
        QObject::connect(
            view->findChild<QObject *>("clearButton"),
            SIGNAL(clicked()),
            this,
            SLOT(clearTable())
        );
        QObject::connect(
            view->findChild<QObject *>("addStringButton"),
            SIGNAL(customClicked(QString)),
            this,
            SLOT(addString(QString))
        );
        QObject::connect(
            view->findChild<QObject *>("buttonsColumn"),
            SIGNAL(buttonClicked(int)),
            this,
            SLOT(deleteString(int))
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
            SLOT(loadCaptureArea())
        );
        QObject::connect(
            view->findChild<QObject *>("loadImageButton"),
            SIGNAL(clicked()),
            this,
            SLOT(loadImage())
        );
        QObject::connect(
            view->findChild<QObject *>("detectStringsButton"),
            SIGNAL(clicked()),
            this,
            SLOT(detectStringsOnImage())
        );
        QObject::connect(
            view->findChild<QObject *>("importButton"),
            SIGNAL(triggered()),
            this,
            SLOT(importProject())
        );
        QObject::connect(
            view->findChild<QObject *>("exportButton"),
            SIGNAL(triggered()),
            this,
            SLOT(exportProject())
        );
    }
}

ContextualizationController::~ContextualizationController()
{
    //TODO: delete de null peta?
    delete model;
    delete tableModel;
}

StringsTableModel * ContextualizationController::getTableModel()
{
    return tableModel;
}

void ContextualizationController::setTableModel(StringsTableModel *tableModel)
{
    this->tableModel = tableModel;
}

void ContextualizationController::addString(QString newString)
{
    int response;
    QString emptyString("");
    QString state;
    FirmwareString *fwString;

    fwString = this->findString(newString);    
    if (fwString) {
        this->addNewString(fwString);

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
            this->addNewString(fwString);
        }
    }
}

void ContextualizationController::deleteString(int row)
{
    this->tableModel->removeRows(row, 1);
    this->model->deleteString(row);
}

void ContextualizationController::clearTable()
{
    this->tableModel->removeRows(0, tableModel->rowCount());
    this->model->clearStringsList();
}

void ContextualizationController::loadCaptureArea()
{
    QString path;

    ((QWindow *)view)->setVisible(false);
    path = this->captureArea();
    this->setImage(path);
    ((QWindow *)view)->setVisible(true);
}

void ContextualizationController::loadImage()
{
    /*TODO:
     * La segunda vez se abre la ventana detrás de la principal.
     * Cuando está abierto en FileDialog, deja seleccionar cosas de la ventana principal
     * y se ejecutan cuando se cierra el FileDialog.
     */

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

void ContextualizationController::detectStringsOnImage()
{
    Ocr test;
    //test.setDataPath("/home/jorge/Projects/contextualization-tool/tesseract/tessdata/");
//    foreach (QString s, test.getAvailableLanguages()) {
//        qDebug() << s;
//    }

    test.setImage(this->model->getImagePath());
    QStringList *a = test.run();
    qDebug() << "Numero resultados = " << a->size();
    foreach (QString s, *a) {
        qDebug() << s;
    }

    delete a;
}

void ContextualizationController::send()
{
    int state = 0;
    QDir tmpDir("/tmp/" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss-") + this->username);

    //state = validateModel();
    switch (state) {
        case 0:
            if(!tmpDir.exists()) {
                tmpDir.mkpath(".");
            }
            else {
                Utils::warningMessage("Send process in progress.", "Wait until the current process end.");
            }
            break;
        case 1:
            Utils::errorMessage("Fail to send!!", "There isn't an image asociated.");
            break;
        case 2:
            Utils::errorMessage("Fail to send!!", "The image associated doesn't exist. Please reload the image.");
            break;
        case 3:
            Utils::errorMessage("Fail to send!!", "There aren't any string asociated.");
            break;
        default:
            Utils::errorMessage("Fail to send!!", "Unknown error.");
            break;
    }
}

void ContextualizationController::cancel()
{
    int response;

    response = Utils::warningMessage("Are you sure?", "If you not save the proyect it will be deleted.");
    if (response == QMessageBox::Yes) {
        //Remove temporal image.
        QFile file(this->model->getImagePath());
        if (file.exists())
            file.remove();

        QApplication::quit();
    }
}

void ContextualizationController::save()
{

}

void ContextualizationController::exportProject()
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

void ContextualizationController::importProject()
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
        this->tableModel->removeRows(0, this->tableModel->rowCount());
        this->importProjectFromFile(dialog.selectedFiles().first());
        this->tableModel->insertRows(0, this->tableModel->rowCount());
    }
}

int ContextualizationController::importProjectFromFile(QString path)
{
    ContextualizationModel *modelTmp;
    QByteArray projectData;

    projectData = Utils::readAllFile(path);
    modelTmp = ContextualizationModel::fromJson(projectData);

    if (modelTmp->isEmpty()) {
        Log::writeError("Fail to import project in file: " + path);
        return 1;
    }

    //TODO: mirar a ver que pasa con el igual. No se mantiene la referencia del modelo de la tabla.
    this->setImage(modelTmp->getImagePath());
    this->clearTable();
    this->model->addNewStrings(modelTmp->getStringsList());
    //this->model = model;
    delete modelTmp;

    return 0;
}

int ContextualizationController::validateModel()
{
    if (this->model->getImagePath().isEmpty()) {
        //No image path
        return 1;
    }

    QFile file(this->model->getImagePath());
    if (!file.exists()) {
        //Image not exists
        return 2;
    }

    if (this->model->getStringsList().size() <= 0) {
        //Error, there isn't strings in the model.
        return 3;
    }

    //All OK
    return 0;
}

int ContextualizationController::generatePackage(const QString &path)
{

    return 0;
}

FirmwareString * ContextualizationController::findString(const QString &text)
{
    int numberOfLine = 0;
    QString line;
    QString message;
    QFile file(fpFile);
    FirmwareString *fwString = nullptr;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            numberOfLine ++;
            fwString = this->fragmentFpLine(line, numberOfLine);
            if (fwString) {
                if (text == fwString->getValue()) {
                    break; ///< Stop to read file
                } else {
                    delete fwString;
                    fwString = nullptr;
                }
            }
        }

        file.close();
    } else {
        message = " Fail to open file: " + fpFile;
        Log::writeError(message);
    }

    return fwString;
}

FirmwareString * ContextualizationController::fragmentFpLine(QString &line, int lineNumber)
{
    QStringList list;
    QStringList subListIdText;
    QStringList subListDescription;
    QStringList subListMaxWidth;
    QStringList subListLocalization;
    QString message;
    bool selected;

    //Fragment fp line.
    list = line.split(" || ");
    if (list.size() == 4) {
        subListIdText = list.at(0).split("  ").mid(0, 2);
        subListIdText << list.at(0).section('\"', 1, 1);
        if (subListIdText.size() == 3) {
            subListDescription = list.at(1).split("  ").mid(0, 1);
            subListDescription << list.at(1).section('\"', 1, 1);
            if (subListDescription.size() == 2) {
                subListMaxWidth = list.at(2).split("  ");
                if (subListMaxWidth.size() == 2) {
                    subListLocalization = list.at(3).split("  ");
                    if (subListLocalization.size() == 2) {
                            QString &state = (QString &)subListLocalization.at(1);
                            if (this->isValidState(state)) {
                                selected = (state == "TODO") ? true : false;
                                return new FirmwareString(
                                    (QString &)subListIdText.at(1),
                                    (QString &)subListIdText.at(2),
                                    (QString &)subListDescription.at(1),
                                    ((QString &)subListMaxWidth.at(1)),
                                    (QString &)subListLocalization.at(1),
                                    selected
                                );
                            } else {
                                message = " englis.fp: String state not valid on line "
                                        + QString::number(lineNumber)
                                        + ".";
                                Log::writeError(message);
                                return nullptr;
                            }

                    } else {
                        message = "englis.fp: Error format in LOCALIZATION column on line "
                                + QString::number(lineNumber)
                                + ".";
                        Log::writeError(message);
                        return nullptr;
                    }

                } else {
                    message = "englis.fp: Error format in MAX_FIELD_WIDTH column on line "
                            + QString::number(lineNumber)
                            + ".";
                    Log::writeError(message);
                    return nullptr;
                }

            } else {
                message = "englis.fp: Error format in TEXT_DESCRIPTION column on line "
                        + QString::number(lineNumber)
                        + ".";
                Log::writeError(message);
                return nullptr;
            }

        } else {
            message = "englis.fp: Error format in MESSAGE_ID column on line "
                    + QString::number(lineNumber)
                    + ".";
            Log::writeError(message);
            return nullptr;
        }

    } else {
        message = "englis.fp: Error format by splitting the string in columns by the separator '||' on line "
                + QString::number(lineNumber)
                + ".";
        Log::writeError(message);
        return nullptr;
    }
}

bool ContextualizationController::isValidState(QString &state)
{
    if (this->validStates.contains(state))
        return true;

    return false;
}

bool ContextualizationController::addNewString(FirmwareString *&fwString)
{
    if (fwString == nullptr) {
        return false;
    }

    if (this->isFpStringAlreadyExists(*fwString)) {
        delete fwString;
        fwString = nullptr;
        Utils::errorMessage("Duplicate string.", "Alredy exists an equal string in the contextualization.");
        return false;
    } else {
        this->model->addNewString(fwString);
        this->tableModel->insertRows(tableModel->rowCount()-1, 1);
        return true;
    }
}

QString ContextualizationController::captureArea()
{
    QProcess *captureProcess;
    QStringList arguments;
    QString path("/tmp/capture.png");

    captureProcess = new QProcess();
    arguments << path;
    captureProcess->start("import", arguments);
    path = captureProcess->waitForFinished(30000) ? path : "";
    delete captureProcess;

    return path;
}

void ContextualizationController::setImage(QString &imagePath)
{
    QObject *containerImage;
    QFile image(imagePath);

    if (image.exists()) {
        containerImage = view->findChild<QObject *>("containerImage");
        containerImage->setProperty("source", "");
        containerImage->setProperty("source", "file:" + imagePath);
        this->model->setImagePath(imagePath);
    } else {
        Log::writeError("Image to set not exists: " + imagePath);
        Utils::errorMessage("Impossible to set image.", "Try it again.");
    }
}

bool ContextualizationController::isFpStringAlreadyExists(FirmwareString &fwString)
{
    if (fwString.getId().isEmpty()) {
        //Check that there aren't strings with the same value.
        foreach (FirmwareString *string, this->model->getStringsList()) {
            if (string->getValue() == fwString.getValue()) {
                return true;
            }
        }
    } else {
        //Check that there aren't strings with the same id.
        foreach (FirmwareString *string, this->model->getStringsList()) {
            if (string->getId() == fwString.getId()) {
                return true;
            }
        }
    }

    return false;
}
