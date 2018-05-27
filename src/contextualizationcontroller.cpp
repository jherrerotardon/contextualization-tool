#include "contextualizationcontroller.h"
/************/
#include <QDebug>
/***************/

ContextualizationController::ContextualizationController(QObject *view, QObject *parent) : QObject(parent)
{
    QObject *stringsTable;

    this->validStates << "TODO" << "DONE" << "VALIDATED";
    this->fpFile = "/home/jorge/Descargas/english.fp";
    this->username = qgetenv("USER");
    this->view = view;

    if (view != nullptr) {
        //Initialize TableView and his model.
        this->tableModel = new StringsTableModel(this->model.getStringsList());
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
    }
}

ContextualizationController::~ContextualizationController()
{
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
    if (fwString)
        this->addNewString(fwString);
    else {
        response = Utils::warningMessage(
            "Impossible to find the string in english.fp file.",
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
    this->model.deleteString(row);
    tableModel->removeRows(row, 1);
}

void ContextualizationController::clearTable()
{
    tableModel->removeRows(0, tableModel->rowCount());
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

    QStringList fileNames;
    QFileDialog dialog(
        Q_NULLPTR,
        tr("Open Image"),
        QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
        tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp)")
    );

    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if (!fileNames.isEmpty())
            this->setImage(fileNames.first());
    }
}

void ContextualizationController::detectStringsOnImage()
{
    Ocr test;
    //test.setDataPath("/home/jorge/Projects/contextualization-tool/tesseract/tessdata/");
//    foreach (QString s, test.getAvailableLanguages()) {
//        qDebug() << s;
//    }

    test.setImage("/home/jorge/Descargas/test.png");
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
        QFile file(this->model.getImagePath());
        if (file.exists())
            file.remove();

        QApplication::quit();
    }
}

int ContextualizationController::validateModel()
{
    if (this->model.getImagePath().isEmpty()) {
        //No image path
        return 1;
    }

    QFile file(this->model.getImagePath());
    if (!file.exists()) {
        //Image not exists
        return 2;
    }

    if (this->model.getStringsList().size() <= 0) {
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
        while (!file.atEnd()) {
            line = in.readLine();
            numberOfLine ++;
            fwString = this->fragmentFpLine(line, numberOfLine);
            if (fwString) {
                if (text == fwString->getValue())
                    break; //Stop to read file
                else
                    delete fwString;
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

void ContextualizationController::addNewString(FirmwareString *&fwString)
{
    this->model.addNewString(fwString);
    this->tableModel->insertRows(tableModel->rowCount()-1, 1);
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
        this->model.setImagePath(imagePath);
    } else {
        Log::writeError("Image to set not exists: " + imagePath);
        Utils::errorMessage("Impossible to set image.", "Try it again.");
    }
}
