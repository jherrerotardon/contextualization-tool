#include "contextualizationcontrollerbase.h"
// TODO: remove includes on bottom of this comment
#include <QDebug>

ContextualizationControllerBase::ContextualizationControllerBase(QObject *parent)
{
    this->model = new ContextualizationModel();
    this->validStates << "TODO" << "DONE" << "VALIDATED";
    this->fpFile = "/home/jorge/Descargas/english.fp";
    this->username = qgetenv("USER");
    this->sendingHost = "192.168.1.100";
}

ContextualizationControllerBase::~ContextualizationControllerBase()
{
    delete this->model;
}

int ContextualizationControllerBase::importProjectFromJsonFile(QString path)
{
    ContextualizationModel *modelTmp;
    QByteArray projectData;

    projectData = Utils::readAllFile(path);
    modelTmp = ContextualizationModel::fromJson(projectData);

    if (!modelTmp || modelTmp->isEmpty()) {
        Log::writeError("Fail to import project in file: " + path);
        return 1;
    }

    this->model->clear();
    //TODO: mirar a ver que pasa con el igual. No se mantiene la referencia del modelo de la tabla.
    this->setImage(modelTmp->getImage());
    this->model->addStrings(modelTmp->getStringsList());
    //this->model = model;
    delete modelTmp;

    return 0;
}

int ContextualizationControllerBase::validateModel()
{
    if (this->model->getImage().isEmpty()) {
        //No image path
        return NoImage;
    }

    QFile file(this->model->getImage());
    if (!file.exists()) {
        //Image not exists
        return ImageNotExist;
    }

    if (this->model->getStringsList().size() <= 0) {
        //Error, there isn't strings in the model.
        return NoStrings;
    }

    //All OK
    return NoError;
}

QString ContextualizationControllerBase::generateContextualization()
{
    QDir tmpDir("/tmp/" + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss-") + this->username);
    QFileInfo image(this->model->getImage());
    QString text("");
    QString contextualizationPackage("");

    if(tmpDir.exists()) {
        Log::writeError("Send process in progress. The contextualization zip file already exists.");
        return QString("");
    }

    if (tmpDir.mkpath(".")) { ///< Create temporal folder.
        QFile::copy(image.absoluteFilePath(), tmpDir.absoluteFilePath(image.fileName()));

        ///< Save strings
        foreach (FirmwareString *fwString, this->model->getStringsList()) {
            text += fwString->toFpFileFormat();
        }

        Utils::writeFile(tmpDir.absoluteFilePath("FirmwareStrings.fp"), text);
        contextualizationPackage = Utils::zipCompressDirectoryContents(tmpDir.absolutePath(), "/tmp", tmpDir.dirName());

        ///< Delete temporal folder with the contextualization.
        tmpDir.removeRecursively();
    } else {
        Log::writeError("Contextualization could not be packaged. Error creating directory " + tmpDir.absolutePath());
    }

    return contextualizationPackage;
}

int ContextualizationControllerBase::sendContextualization(const QString &path, QString user, QString password)
{
    QStringList arguments;
    QFile batch("/tmp/batch");
    int errorCode = -1;

    if (QFile(path).exists()) {
        Utils::writeFile(batch.fileName(), "put " + path); ///< Create temporal batch file

        arguments << "-p" << password;
        arguments << "sftp" << "-oBatchMode=no" << "-b" << batch.fileName()
            << user + '@' + this->sendingHost + ":Contextualizations";

        //TODO: poner un QProgressDialog top para el progreso

        errorCode = Utils::executeProgram("sshpass", arguments);

        //TODO:Tratar error

        batch.remove();
    }

    return errorCode;
}

FirmwareString * ContextualizationControllerBase::findString(const QString &text)
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

FirmwareString * ContextualizationControllerBase::fragmentFpLine(QString &line, int lineNumber)
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

bool ContextualizationControllerBase::isValidState(QString &state)
{
    if (this->validStates.contains(state))
        return true;

    return false;
}

int ContextualizationControllerBase::addString(FirmwareString *&fwString)
{
    if (fwString == nullptr) {
        return NullPointer;
    }

    if (this->isFpStringAlreadyExists(*fwString)) {
        delete fwString;
        fwString = nullptr;

        return StringAlreadyExists;
    }

    this->model->addString(fwString);

    return NoError;
}

bool ContextualizationControllerBase::removeString(int row)
{
    return this->model->removeString(row);
}

void ContextualizationControllerBase::removeAllStrings()
{
    this->model->clearStringsList();
}

QString ContextualizationControllerBase::takeCaptureArea()
{
    QStringList arguments;
    QString path("/tmp/capture.png");

    arguments << path;

    return Utils::executeProgram("import", arguments, QString(), 30000) ? QString("") : path;
}

bool ContextualizationControllerBase::setImage(const QString &image)
{
    QFileInfo imageInfo(image);
    QString destination("/tmp/contextualizationCapture." + imageInfo.suffix());
    bool exists = imageInfo.exists();

    if (exists) {
        ///< If exists copy image in /tmp with "contextualizationCapture" name and the extension of the orginal file.
        if (QFile::exists(destination)) {
            QFile::remove(destination);
        }

        QFile::copy(image, destination);
    } else {
        Log::writeError("Image to set not exists: " + image);
    }

    this->model->setImage(exists ? destination : QString());

    return exists;
}

bool ContextualizationControllerBase::isFpStringAlreadyExists(FirmwareString &fwString)
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