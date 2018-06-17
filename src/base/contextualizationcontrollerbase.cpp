#include "contextualizationcontrollerbase.h"

const QString ContextualizationControllerBase::IMAGES_FOLDER = QDir("../storage/images").absolutePath() + '/';
const QString ContextualizationControllerBase::PROJECTS_FOLDER = QDir("../storage/projects").absolutePath() + '/';

ContextualizationControllerBase::ContextualizationControllerBase(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);

    model_ = new ContextualizationModel();

    username_ = qgetenv("USER");
    loadConfig();

    //Set default values for class members that could not be loaded from the configuration file.
    if (validStates_.isEmpty()) {
         validStates_ << "TODO" << "DONE" << "VALIDATED";
    }

    if (fpFile_.isEmpty()) {
        //By default, english.fp should be in home of user.
         fpFile_ = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/english.fp";
    }

    //Make storage directory if not exists
    QDir(IMAGES_FOLDER).mkpath(".");
    QDir(PROJECTS_FOLDER).mkpath(".");
}

ContextualizationControllerBase::~ContextualizationControllerBase()
{
    delete model_;
}

int ContextualizationControllerBase::importProjectFromJsonFile(const QString &path)
{
    ContextualizationModel *modelTmp;
    QByteArray projectData;

    projectData = Utils::readAllFile(path);
    if (projectData.isEmpty()) {
        Log::writeError("Fail to import. File doesn't exist: " + path);

        return NoImportFile;
    }
    modelTmp = ContextualizationModel::fromJson(projectData);

    if (!modelTmp || modelTmp->isEmpty()) {
        Log::writeError("Not valid contextualization format to import: " + path);

        return ImportFileFormat;
    }

    *(model_) = *modelTmp;

    delete modelTmp;

    return NoError;
}

int ContextualizationControllerBase::exportToJsonFile(const QString &path)
{    
    return Utils::writeFile(path, model_->toJson());
}

ContextualizationControllerBase::ModelError ContextualizationControllerBase::validateModel()
{
    QFile image(model_->getImage());

    if (!model_->hasImage()) {
        //No image path
        return NoImage;
    }

    if (!image.exists()) {
        //Image not exists
        return ImageNotExist;
    }

    if (!model_->hasStrings()) {
        //Error, there isn't strings in the model.
        return NoStrings;
    }

    //All OK
    return OkModel;
}

QString ContextualizationControllerBase::generateContextualization()
{
    QDir tmpDir("/tmp/" + getDateTime() + '-' + username_);
    QFileInfo image(model_->getImage());
    QString text("");
    QString contextualizationPackage("");

    if(tmpDir.exists()) {
        Log::writeError("Send process in progress. The contextualization zip file already exists.");
        return QString("");
    }

    if (tmpDir.mkpath(".")) { // Create temporal folder.
        QFile::copy(image.absoluteFilePath(), tmpDir.absoluteFilePath(image.fileName()));

        // Save only selected strings
        foreach (QObject *fwString, model_->getStringsList()) {
            if (static_cast<FirmwareString *>(fwString)->isSelected()) {
                text += static_cast<FirmwareString *>(fwString)->toFpFileFormat();
                text += '\n';
            }
        }

        Utils::writeFile(tmpDir.absoluteFilePath("FirmwareStrings.fp"), text);
        contextualizationPackage = Utils::zipCompressDirectoryContents(tmpDir.absolutePath(), "/tmp", tmpDir.dirName());

        // Delete temporal folder with the contextualization.
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
    int errorCode;;

    if (remoteHost_.isEmpty()) {
        return NoRemoteHost;
    }

    if (!QFile::exists(path)) {
        return FileNotExists;
    }

    Utils::writeFile(batch.fileName(), "put " + path); // Create temporal batch file

    arguments << "-p" << password;
    arguments << "sftp" << "-oBatchMode=no" << "-b" << batch.fileName()
              << user + '@' + remoteHost_ + ":Contextualizations";

    //TODO: poner un QProgressDialog top para el progreso

    errorCode = Utils::executeProgram("sshpass", arguments);

    //TODO:Tratar error

    batch.remove();

    return errorCode;
}

QStringList * ContextualizationControllerBase::detectStringsOnImage()
{
    Ocr ocr(model_->getImage());

    ocr.setDataPath(QDir("../tesseract/tessdata").absolutePath());

    return ocr.run();
}

int ContextualizationControllerBase::processStrings(const QStringList &strings)
{
    int count = 0;

    foreach (QString string, strings) {
        count += addStrings(findString(string, ByValue));
    }

    return count;
}

QList<FirmwareString *> ContextualizationControllerBase::findString(const QString &text, const FindType findType)
{
    switch (findType) {
        case ByID:
            return findStringById(text);
        case ByValue:
            return findStringByValue(text);
        default:
            return QList<FirmwareString *>();
    }
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

    // Fragment fp line.
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
                            if (isValidState(state)) {
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
                                return Q_NULLPTR;
                            }

                    } else {
                        message = "englis.fp: Error format in LOCALIZATION column on line "
                                + QString::number(lineNumber)
                                + ".";
                        Log::writeError(message);
                        return Q_NULLPTR;
                    }

                } else {
                    message = "englis.fp: Error format in MAX_FIELD_WIDTH column on line "
                            + QString::number(lineNumber)
                            + ".";
                    Log::writeError(message);
                    return Q_NULLPTR;
                }

            } else {
                message = "englis.fp: Error format in TEXT_DESCRIPTION column on line "
                        + QString::number(lineNumber)
                        + ".";
                Log::writeError(message);
                return Q_NULLPTR;
            }

        } else {
            message = "englis.fp: Error format in MESSAGE_ID column on line "
                    + QString::number(lineNumber)
                    + ".";
            Log::writeError(message);
            return Q_NULLPTR;
        }

    } else {
        message = "englis.fp: Error format by splitting the string in columns by the separator '||' on line "
                + QString::number(lineNumber)
                + ".";
        Log::writeError(message);
        return Q_NULLPTR;
    }
}

bool ContextualizationControllerBase::isValidState(QString &state)
{
    if (validStates_.contains(state))
        return true;

    return false;
}

int ContextualizationControllerBase::addString(FirmwareString *fwString)
{
    if (fwString == Q_NULLPTR) {
        return NullPointer;
    }

    if (isFwStringAlreadyExists(*fwString)) {
        delete fwString;
        fwString = Q_NULLPTR;

        return StringAlreadyExists;
    }

    model_->addString(fwString);

    emit stringsListChanged();

    return NoError;
}

int ContextualizationControllerBase::addStrings(const QList<FirmwareString *> &strings)
{
    int count = 0;

    foreach (FirmwareString *fwString, strings) {
        if (addString(fwString) == NoError) {
            count++;
        }
    }

    if (count < 0) { // Only emit signal if strings have been added.
        emit stringsListChanged();
    }

    return count;
}

bool ContextualizationControllerBase::removeString(QString stringId)
{
    if(model_->removeString(stringId)) { // Only emit signal if string has been added.
        emit stringsListChanged();

        return true;
    }

    return false;
}

bool ContextualizationControllerBase::removeString(int row)
{
    if(model_->removeString(row)) { // Only emit signal if string has been added.
        emit stringsListChanged();

        return true;
    }

    return false;
}

bool ContextualizationControllerBase::removeAllStrings()
{
    if (!model_->getStringsList().isEmpty()) {
        model_->removeAllStrings();

        emit stringsListChanged();

        return true;
    }

    return false;
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
    QString imageName("capture-" + getDateTime() + '-' + username_ + '.' + imageInfo.suffix());
    bool exists = imageInfo.exists();

    if (exists) {
        //Save image in non volatil folder.
        QFile::copy(image, IMAGES_FOLDER + imageName);
    } else {
        Log::writeError("Image to set not exists: " + image);
    }

    //Remove old image in the model (only if has it) before set the new image.
    if (model_->hasImage()) {
        QFile::remove(model_->getImage());
    }

    model_->setImage(exists ? IMAGES_FOLDER + imageName : ContextualizationModel::NO_IMAGE_PATH);

    emit imageChanged();

    //TODO: poner cuando se haga manejadora de errores.
//    if (!exists) {
//        Utils::errorMessage("Can't set image.", "Not exists the image: " + image);
//    }

    return exists;
}

bool ContextualizationControllerBase::isFwStringAlreadyExists(FirmwareString &fwString)
{
    if (fwString.getId().isEmpty()) {
        // Check that there aren't strings with the same value.
        foreach (QObject *string, model_->getStringsList()) {
            if (static_cast<FirmwareString *>(string)->getValue() == fwString.getValue()) {
                return true;
            }
        }
    } else {
        // Check that there aren't strings with the same id.
        foreach (QObject *string, model_->getStringsList()) {
            if (static_cast<FirmwareString *>(string)->getId() == fwString.getId()) {
                return true;
            }
        }
    }

    return false;
}

int ContextualizationControllerBase::eraseExistStrings(QList<FirmwareString *> *strings)
{
    int count = 0;
    QList<FirmwareString *>::Iterator iterator = strings->begin();
    QList<FirmwareString *>::Iterator end = strings->end();

    while (iterator != end) {
        if(isFwStringAlreadyExists(**iterator)) {
            delete *iterator;
            strings->erase(iterator);
            count++;
        }

        ++iterator;
    }

    return count;
}

QString ContextualizationControllerBase::getImageOfModel()
{
    return model_->getImage();
}

QList<QObject *> ContextualizationControllerBase::getTableModel()
{
    return model_->getStringsList();
}

QString ContextualizationControllerBase::getDateTime(QString format)
{
    return QDateTime::currentDateTime().toString(format);
}

void ContextualizationControllerBase::loadConfig()
{
    QJsonObject root;
    QJsonParseError jsonError;
    QJsonDocument document;
    QFile configurationFile("../configs/general.conf");

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.fileName()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError("Error decoding configuration file: " + jsonError.errorString());

            return;
        }

        //Get object with the configuration.
        root = document.object();
        if (root.isEmpty()) {
            Log::writeError("Error format on configuration file. " + configurationFile.fileName());

            return;
        }

        //Sets class member.
        fpFile_ = root.value("english.fp").toString();
        if (fpFile_.startsWith("~")) {
            //Replace '~' by user home path.
            fpFile_.replace(0, 1, QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first());
        }

        remoteHost_ = root.value("remoteHost").toString();
        foreach (QJsonValue value, root.value("validStates").toArray()) {
            validStates_ << value.toString();
        }
    }
}

QList<FirmwareString *> ContextualizationControllerBase::findStringById(const QString &id)
{
    int numberOfLine = 0;
    QString line;
    QFile file(fpFile_);
    QList<FirmwareString *> stringsFound;
    FirmwareString *fwString = Q_NULLPTR;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            numberOfLine ++;
            fwString = fragmentFpLine(line, numberOfLine);
            if (fwString) {
                if (id == fwString->getId()) {
                    stringsFound << fwString;
                    break; // Stop to read file
                }

                delete fwString;
            }
        }

        file.close();
    } else {
        Log::writeError(" Fail to open file: " + file.fileName());
    }

    return stringsFound;
}

QList<FirmwareString *> ContextualizationControllerBase::findStringByValue(const QString &value)
{
    int numberOfLine = 0;
    QString line;
    QFile file(fpFile_);
    QList<FirmwareString *> stringsFound;
    FirmwareString *fwString = Q_NULLPTR;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            numberOfLine ++;
            fwString = fragmentFpLine(line, numberOfLine);
            if (fwString) {
                if (value == fwString->getValue()) {
                    stringsFound << fwString;
                } else {
                    delete fwString;
                }
            }
        }

        file.close();
    } else {
        Log::writeError(" Fail to open file: " + file.fileName());
    }

    return stringsFound;
}
