/**
 * @file contextualizationcontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class ContectualizationController
 *
 * @brief This is the controller base class.
 */

#include "contextualizationcontroller.h"

const int ContextualizationController::CHUNK_WIDTH = 300;
const int ContextualizationController::CHUNK_HEIGHT = 150;
const QString ContextualizationController::DONE_FP_FILE = Utils::getTmpDirectory() + "/doneFpFile.fp";
const QString ContextualizationController::IMAGES_FOLDER = QDir("../storage/images").absolutePath() + '/';
const QString ContextualizationController::PROJECTS_FOLDER = QDir("../storage/projects").absolutePath() + '/';
const QString ContextualizationController::CONFIG_FOLDER = QDir("../config").absolutePath() + '/';

ContextualizationController::ContextualizationController(QObject *parent) : QObject(parent)
{
    onlyDoneStrings_ = true;
    caseSensitive_ = true;

    model_ = new ContextualizationModel();

    username_ = QDir::home().dirName();

    loadConfig();

    // Set default values for class members that could not be loaded from the configuration file.
    if (validStates_.isEmpty()) {
         validStates_ << "TODO" << "DONE" << "VALIDATED";
    }

    if (englishFpFile_.isEmpty()) {
        // By default, english.fp should be in home of user.
         englishFpFile_ = QDir::homePath() + "/english.fp";
    }

    // Make storage directory if not exists
    if(QDir(IMAGES_FOLDER).mkpath(".")) {
        Log::writeLog(QString(Q_FUNC_INFO) + "  Created " + IMAGES_FOLDER);
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Impossible create " + IMAGES_FOLDER);
    }

    if(QDir(PROJECTS_FOLDER).mkpath(".")) {
        Log::writeLog(QString(Q_FUNC_INFO) + " Created " + PROJECTS_FOLDER);
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Impossible create " + PROJECTS_FOLDER);
    }

    if(QDir(CONFIG_FOLDER).mkpath(".")) {
        Log::writeLog(QString(Q_FUNC_INFO) + " Created " + CONFIG_FOLDER);
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Impossible create " + CONFIG_FOLDER);
    }
}

ContextualizationController::~ContextualizationController()
{
    delete model_;
}

ContextualizationController::CodeError ContextualizationController::importProjectFromJsonFile(const QString &path)
{
    ContextualizationModel *modelTmp;
    QByteArray projectData;

    if (!QFile::exists(path)) {
        Log::writeError(QString(Q_FUNC_INFO) + " Fail to import. File doesn't exist: " + path);

        return NoImportFile;
    }

    projectData = Utils::readAllFile(path);
    modelTmp = ContextualizationModel::fromJson(projectData);

    if (!modelTmp) {
        Log::writeError(QString(Q_FUNC_INFO) + " Not valid contextualization format to import: " + path);

        return ImportFileFormat;
    }

    *(model_) = *modelTmp;
    Log::writeLog(QString(Q_FUNC_INFO) + " Imported project from: " + path);

    delete modelTmp;

    return NoError;
}

bool ContextualizationController::exportToJsonFile(const QString &path)
{

    QFileInfo imageInfo(model_->getImage());
    QString imageName("capture-" + Utils::getDateTime() + '-' + username_ + '.' + imageInfo.suffix());

    // Save image in non volatil folder if exists or is empty.
    if (!model_->hasImage() || QFile::copy(model_->getImage(), IMAGES_FOLDER + imageName)) {
        model_->setImage(model_->hasImage() ? IMAGES_FOLDER + imageName : "");

        // Save project in JSON format
        if (Utils::writeFile(path, model_->toJson())) {
            Log::writeLog(QString(Q_FUNC_INFO) + " Exported project in: " + path);

            return true;
        }
    } else {
        Log::writeError(QString(Q_FUNC_INFO) +  "Impossible to copy " + model_->getImage() + " in " + IMAGES_FOLDER + imageName);
    }

    Log::writeError("Can not save current project in " + path);

    return false;
}

ContextualizationController::ModelError ContextualizationController::validateModel()
{
    QFile image(model_->getImage());

    if (!model_->hasImage()) {
        // No image path
        return NoImage;
    }

    if (!image.exists()) {
        // Image not exists
        return ImageNotExist;
    }

    if (!model_->hasStrings()) {
        // Error, there isn't strings in the model.
        return NoStrings;
    }

    // All ok
    return OkModel;
}

QString ContextualizationController::generateContextualization()
{
    QDir tmpDir(Utils::getTmpDirectory() + "/" + Utils::getDateTime() + '-' + username_);
    QFileInfo image(model_->getImage());
    QString text("");
    QString contextualizationPackage("");

    if(tmpDir.exists()) {
        Log::writeError(QString(Q_FUNC_INFO) + " Send process in progress. The contextualization zip file already exists.");
        return QString("");
    }

    if (tmpDir.mkpath(".")) { // Create temporal folder.
        QFile::copy(image.absoluteFilePath(), tmpDir.absoluteFilePath(image.fileName()));

        // Save only selected strings
        foreach (QObject *fwString, model_->getStringsList()) {
            if (static_cast<FirmwareString *>(fwString)->isSelected()) {
                text += static_cast<FirmwareString *>(fwString)->getId();
                text += '\n';
            }
        }

        Utils::writeFile(tmpDir.absoluteFilePath("FirmwareStrings.fp"), text);
        contextualizationPackage = Utils::zipCompressDirectoryContents(tmpDir.absolutePath(), "/tmp", tmpDir.dirName());
        if (!contextualizationPackage.isEmpty()) {
            Log::writeLog(QString(Q_FUNC_INFO) + " Contextualization package generate in: " + contextualizationPackage);
        } else {
            Log::writeError(QString(Q_FUNC_INFO) + " Could not create contextualization package " + contextualizationPackage);
        }

        // Delete temporal folder with the contextualization.
        tmpDir.removeRecursively();
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Contextualization could not be packaged. Error creating directory " + tmpDir.absolutePath());
    }

    return contextualizationPackage;
}

ContextualizationController::CodeError ContextualizationController::sendContextualization(
    const QString &path,
    QString user,
    QString password
) {
    QStringList arguments;
    QString batch(Utils::getTmpDirectory() + "/batch");
    QString sshpassEvidences(Utils::getTmpDirectory() + "/sshpassEvidenceError_" + Utils::getDateTime() + ".txt");
    CodeError errorCode = NoError;
    int out;

    if (remoteHost_.isEmpty()) {
        Log::writeError(QString(Q_FUNC_INFO) + " No remote host (IP or hostname) received when the contextualizacion was going to be sent.");

        return NoRemoteHost;
    }

    if (!Utils::isValidIp(remoteHost_)) {
        Log::writeError(QString(Q_FUNC_INFO) + " No valid IP received when the contextualizacion was going to be sent. IP: " + remoteHost_);

        return NoValidIp;
    }

    if (!QFile::exists(path)) {
        Log::writeError(QString(Q_FUNC_INFO) + " Contextualization file to be sent not exists. Contextualization path: " + path);

        return FileNotExists;
    }

    Utils::writeFile(batch, "put " + path); // Create temporal batch file

    arguments << "-p" << password;
    arguments << "sftp" << "-oBatchMode=no" << "-b" << batch
              << user + '@' + remoteHost_ + ":Contextualizations";

    out = Utils::executeProgram("sshpass", arguments, sshpassEvidences);

    // If there are some error, write ouput process in error log.
    if (out != NoError) {
        Log::writeError(QString(Q_FUNC_INFO) + QString(Utils::readAllFile(sshpassEvidences)).replace("\n", ". "));

        errorCode = out == SshError ? SshError : SshpassError;
    } else {
        Log::writeLog(QString(Q_FUNC_INFO) + " Contextualization " + path + " sent to " + remoteHost_);
    }

    // Remove temporal files.
    QFile::remove(sshpassEvidences);
    QFile::remove(batch);

    return errorCode;
}

QList<FirmwareString *> ContextualizationController::detectStringsOnImage(QString image)
{
    TesseractOcr *worker;
    QList<TesseractOcr *> workers; // Used to save workers because after must be released.
    QList<QFuture<QList<FirmwareString *>>> futures;
    QList<FirmwareString *> fwStrings;
    QStringList imageChunks;
    QString rootCopy;

    // Added root image copy to work it too.
    rootCopy = Utils::getTmpDirectory() + "/rootCopy_" + Utils::getDateTime() + "." + QFileInfo(image).suffix();
    if (QFile::copy(image, rootCopy)) {
        imageChunks << rootCopy;
    }

    // Split image.
    imageChunks << splitImage(image, CHUNK_WIDTH, CHUNK_HEIGHT);

    // Create and start workers.
    foreach (QString image, imageChunks) {
        worker = new TesseractOcr(image);
        workers << worker;

       futures <<  QtConcurrent::run(
            [this, worker]() {
                QList<FirmwareString *> out;

                out = this->processExtractedStrings(worker->extract());

                return out;
            }
        );
    }

    // Wait for all workers to finish and retrieve results of extraction
    foreach (QFuture<QList<FirmwareString *>> future, futures) {
        future.waitForFinished();
        fwStrings << future.result();
    }

    // Release memory of workers.
    foreach (TesseractOcr *worker, workers) {
        delete worker;
        worker = Q_NULLPTR;
    }

    //Delete image chunks from disk.
    foreach (QString path, imageChunks) {
        QFile::remove(path);
    }


    return fwStrings;
}

QList<FirmwareString *> ContextualizationController::fastDetectStringsOnImage(QString image)
{
    TesseractOcr worker(image);
    QList<FirmwareString *> fwStrings;

    return this->processExtractedStrings(worker.extract());
}

QList<FirmwareString *> ContextualizationController::processExtractedStrings(QStringList strings)
{
    QList<FirmwareString *> stringsFound;
    QList<FirmwareString *> out;

    foreach (QString string, strings) {
        stringsFound = findString(string.simplified(), ByApproximateValue);
        if (!stringsFound.isEmpty()) {
            out << stringsFound;
        }
    }

    return out;
}

QList<FirmwareString *> ContextualizationController::findString(const QString &text, const MatchType matchType)
{
    QFile file;
    QList<String *> stringsFound;
    QList<FirmwareString *> out;
    FpFileConnector database;

    /**
     * If only have to find strings with DONE state, tries to do search in DONE_FP_FIlE, else use englishFpFile_.
     *
     * Tries to use DONE_FP_FILE if is possible to do searches faster.
     */
    if (onlyDoneStrings_) {
        // If DONE_FP_FILE exists, uses it. Else, tries make it. Otherwise uses englishFpFile_.
        if (QFile::exists(DONE_FP_FILE)) {
            file.setFileName(DONE_FP_FILE);
        } else {
            file.setFileName(generateDoneFpFile() ? englishFpFile_ : DONE_FP_FILE);
        }
    } else {
        file.setFileName(englishFpFile_);
    }

    // Set file where strings will be searched.
    database.setPath(file.fileName());

    // Get strings from database.
    switch (matchType) {
        case ByID:
            stringsFound = database.getStringWithId(text, caseSensitive_);
            break;

        case ByValue:
            stringsFound = database.getStringsWithValue(text, caseSensitive_);
            break;

        case ByApproximateValue:
            stringsFound = database.getStringsWithApproximateValue(text, caseSensitive_);
            break;
    }

    // Convert String * in FirmwareString *.
    foreach (String *string, stringsFound) {
        out << static_cast<FirmwareString *>(string);
    }

    // If only have to get DONE strings and find was not in DONE_FP_FILE is necessary filer strings.
    if (onlyDoneStrings_ && file.fileName() != DONE_FP_FILE) {
        filterStringsByState(&out, "DONE");
    }

    return out;
}

bool ContextualizationController::isValidState(QString &state)
{
    if (validStates_.contains(state))
        return true;

    return false;
}

ContextualizationController::CodeError ContextualizationController::addString(FirmwareString *fwString)
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

    return NoError;
}

int ContextualizationController::addStrings(const QList<FirmwareString *> &strings)
{
    int count = 0;

    foreach (FirmwareString *fwString, strings) {
        if (addString(fwString) == NoError) {
            count++;
        }
    }

    return count;
}

bool ContextualizationController::removeString(QString stringId)
{
    return model_->removeString(stringId);
}

bool ContextualizationController::removeString(int row)
{
    return model_->removeString(row);
}

bool ContextualizationController::removeAllStrings()
{
    if (!model_->getStringsList().isEmpty()) {
        model_->removeAllStrings();

        return true;
    }

    return false;
}

bool ContextualizationController::selectString(const QString id, bool state)
{
    return state ? model_->selectString(id) : model_->unselectString(id);
}

void ContextualizationController::clearImage()
{
    model_->setImage("");
}

bool ContextualizationController::setImage(const QString &image)
{
    QFileInfo imageInfo(image);

    if (image.isEmpty() || QFile::exists(image)) {
        model_->setImage(image);
        Log::writeLog(QString(Q_FUNC_INFO) + " New image set in the project: " + image);

        return true;
    }

    model_->setImage("");
    Log::writeError(QString(Q_FUNC_INFO) + " Image to set not exists: " + image);

    return false;
}

bool ContextualizationController::isFwStringAlreadyExists(FirmwareString &fwString)
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

int ContextualizationController::eraseExistStrings(QList<FirmwareString *> *strings)
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

QString ContextualizationController::getImageOfModel()
{
    return model_->getImage();
}

QList<QObject *> ContextualizationController::getTableModel()
{
    return model_->getStringsList();
}

void ContextualizationController::loadConfig()
{
    QJsonObject root;
    QJsonParseError jsonError;
    QJsonDocument document;
    QFile configurationFile(CONFIG_FOLDER + "general.conf");

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.fileName()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error decoding configuration file: " + jsonError.errorString());

            return;
        }

        // Get object with the configuration.
        root = document.object();
        if (root.isEmpty()) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error format on configuration file. " + configurationFile.fileName());

            return;
        }

        // Sets class member.
        englishFpFile_ = root.value("english.fp").toString();
        if (englishFpFile_.startsWith("~")) {
            // Replace '~' by user home path.
            englishFpFile_.replace(0, 1, QDir::homePath());
        }

        remoteHost_ = root.value("remoteHost").toString();

        foreach (QJsonValue value, root.value("validStates").toArray()) {
            validStates_ << value.toString();
        }

        Log::writeLog(QString(Q_FUNC_INFO) + " Loaded config from " + configurationFile.fileName());
    }
}

void ContextualizationController::saveConfig()
{
    QJsonObject root;
    QJsonDocument document;
    QJsonParseError jsonError;
    QFileInfo configurationFile(CONFIG_FOLDER + "general.conf");

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.fileName()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error decoding configuration file: " + jsonError.errorString());

            return;
        }

        // Get object with the configuration.
        root = document.object();
    }

    // Replace or add atributtes.
    root.insert("english.fp", englishFpFile_);
    root.insert("remoteHost", remoteHost_);

    // Write configuration in disk.
    if (Utils::writeFile(configurationFile.absoluteFilePath(), QString(QJsonDocument(root).toJson(QJsonDocument::Indented)))) {
        Log::writeLog(QString(Q_FUNC_INFO) + " Configuration saved succesfully.");
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Could not save configuration in file " + configurationFile.absoluteFilePath());
    }
}

int ContextualizationController::generateDoneFpFile()
{
    QStringList grepArguments;
    int codeError;

    grepArguments << "DONE$" << englishFpFile_;

    codeError = Utils::executeProgram("grep", grepArguments, DONE_FP_FILE);
    if (codeError == 0) {
        Log::writeLog(QString(Q_FUNC_INFO) + " DONE fp file generated succesfully: " + DONE_FP_FILE);
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " DONE fp file could not be generated: " + DONE_FP_FILE);
    }

    return codeError;
}

int ContextualizationController::filterStringsByState(QList<FirmwareString *> *list, const QString &state)
{
    int count = 0;
    QList<FirmwareString *>::Iterator iterator = list->begin();
    QList<FirmwareString *>::Iterator end = list->end();

    while (iterator != end) {
        if((*iterator)->getState() != state) {
            delete *iterator;
            list->erase(iterator);
            count++;
        }

        ++iterator;
    }

    return count;
}

QStringList ContextualizationController::splitImage(
    const QString &imagePath,
    int chunkWidth,
    int chunkHeight,
    bool *ok
) {
    int width;
    int height;
    QImage image(imagePath);
    QFileInfo rootImage(imagePath);
    QList<QImage> imageChunks;
    QStringList chunks;
    QString fileName;

    // Create chunks of image.
    for (int xOffset = 0; xOffset < image.width(); xOffset += chunkWidth) {
        width = xOffset + chunkWidth > image.width() ? image.width() - xOffset : chunkWidth;

        for (int yOffset = 0; yOffset <= image.height(); yOffset += chunkHeight) {
            height = yOffset + chunkHeight > image.height() ? image.height() - yOffset : chunkHeight;

            imageChunks << image.copy(xOffset, yOffset, width, height);
        }
    }

    // Save images on disk.
    for (int i = 0; i < imageChunks.size(); ++i) {
        fileName = Utils::getTmpDirectory() + "/" + rootImage.baseName() + "_chunk_" + QString::number(i+1) + '.' + rootImage.suffix();

        // Only returns chunks saved succesfully.
        if (imageChunks.at(i).save(fileName, Q_NULLPTR, 100)) {
            chunks << fileName;
            Log::writeLog(QString(Q_FUNC_INFO) + " Saved image chunk: " + fileName);
        } else {
            Log::writeError(QString(Q_FUNC_INFO) + " Could no saved image chunk: " + fileName);
            if (ok) {
                *ok = false;
            }
        }
    }

    return chunks;
}

void ContextualizationController::refresh()
{
    generateDoneFpFile();

    emit imageChanged();
    emit stringsListChanged();
}

QString ContextualizationController::getParameterFromConfigFile(const QString parameter)
{
    QJsonObject root;
    QJsonParseError jsonError;
    QJsonDocument document;
    QFile configurationFile(CONFIG_FOLDER + "general.conf");
    QString value;

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.fileName()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error decoding configuration file: " + jsonError.errorString());

            return QString();
        }

        // Get object with the configuration.
        root = document.object();
        if (root.isEmpty()) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error format on configuration file. " + configurationFile.fileName());

            return QString();
        }

        value = root.value(parameter).toString();

        return value.isEmpty() ? QString() : value;

    }

    return QString();
}

bool ContextualizationController::setParameterInConfigFile(const QString parameter, const QString value)
{
    QJsonObject root;
    QJsonDocument document;
    QJsonParseError jsonError;
    QFileInfo configurationFile(CONFIG_FOLDER + "general.conf");

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.absoluteFilePath()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError(QString(Q_FUNC_INFO) + " Error decoding configuration file: " + jsonError.errorString());

            return false;
        }

        // Get object with the configuration.
        root = document.object();
    }

    // Replace or add atributtes.
    root.insert(parameter, value);

    // Write configuration in disk.
    if (Utils::writeFile(configurationFile.absoluteFilePath(), QString(QJsonDocument(root).toJson(QJsonDocument::Indented)))) {
        Log::writeLog(QString(Q_FUNC_INFO) + " Parameter " + parameter + " saved succesfully with value " + value +".");
        return true;
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " Could not save parameter " + parameter + " in file " + configurationFile.absoluteFilePath());
        return false;
    }
}
