#include "contextualizationcontroller.h"

const int ContextualizationController::CHUNK_WIDTH = 300;
const int ContextualizationController::CHUNK_HEIGHT = 150;
const QString ContextualizationController::DONE_FP_FILE = "/tmp/doneFpFile.fp";
const QString ContextualizationController::IMAGES_FOLDER = QDir("../storage/images").absolutePath() + '/';
const QString ContextualizationController::PROJECTS_FOLDER = QDir("../storage/projects").absolutePath() + '/';

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
        Log::writeLog("Created " + IMAGES_FOLDER);
    } else {
        Log::writeError("Impossible create " + IMAGES_FOLDER);
    }

    if(QDir(PROJECTS_FOLDER).mkpath(".")) {
        Log::writeLog("Created " + PROJECTS_FOLDER);
    } else {
        Log::writeError("Impossible create " + PROJECTS_FOLDER);
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
    Log::writeLog("Imported project from: " + path);

    emit imageChanged();
    emit stringsListChanged();

    delete modelTmp;

    return NoError;
}

bool ContextualizationController::exportToJsonFile(const QString &path)
{    
    if (Utils::writeFile(path, model_->toJson())) {
        Log::writeLog("Exported project in: " + path);

        return true;
    }

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
    QDir tmpDir("/tmp/" + Utils::getDateTime() + '-' + username_);
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
                text += static_cast<FirmwareString *>(fwString)->getId();
                text += '\n';
            }
        }

        Utils::writeFile(tmpDir.absoluteFilePath("FirmwareStrings.fp"), text);
        contextualizationPackage = Utils::zipCompressDirectoryContents(tmpDir.absolutePath(), "/tmp", tmpDir.dirName());
        if (!contextualizationPackage.isEmpty()) {
            Log::writeLog("Contextualization package generate in: " + contextualizationPackage);
        } else {
            Log::writeError("Could not create contextualization package " + contextualizationPackage);
        }

        // Delete temporal folder with the contextualization.
        tmpDir.removeRecursively();
    } else {
        Log::writeError("Contextualization could not be packaged. Error creating directory " + tmpDir.absolutePath());
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
        Log::writeError("No remote host (IP or hostname) received when the contextualizacion was going to be sent.");

        return NoRemoteHost;
    }

    if (!Utils::isValidIp(remoteHost_)) {
        Log::writeError("No valid IP received when the contextualizacion was going to be sent. IP: " + remoteHost_);

        return NoValidIp;
    }

    if (!QFile::exists(path)) {
        Log::writeError("Contextualization file to be sent not exists. Contextualization path: " + path);

        return FileNotExists;
    }

    Utils::writeFile(batch, "put " + path); // Create temporal batch file

    arguments << "-p" << password;
    arguments << "sftp" << "-oBatchMode=no" << "-b" << batch
              << user + '@' + remoteHost_ + ":Contextualizations";

    out = Utils::executeProgram("sshpass", arguments, sshpassEvidences);

    // If there are some error, write ouput process in error log.
    if (out != NoError) {
        Log::writeError(QString(Utils::readAllFile(sshpassEvidences)).replace("\n", ". "));

        errorCode = out == SshError ? SshError : SshpassError;
    } else {
        Log::writeLog("Contextualization " + path + " sent to " + remoteHost_);
    }

    // Remove temporal files.
    QFile::remove(sshpassEvidences);
    QFile::remove(batch);

    return errorCode;
}

QList<FirmwareString *> ContextualizationController::detectStringsOnImage()
{
    TesseractOcr *worker;
    QList<TesseractOcr *> workers; // Used to save workers because after must be released.
    QList<QFuture<QList<FirmwareString *>>> futures;
    QList<FirmwareString *> fwStrings;
    QStringList imageChunks;
    QString rootCopy;

    // Added root image copy to work it too.
    rootCopy = Utils::getTmpDirectory() + "/rootCopy_" + Utils::getDateTime() + "." + QFileInfo(model_->getImage()).suffix();
    if (QFile::copy(model_->getImage(), rootCopy)) {
        imageChunks << rootCopy;
    }

    // Split image.
    imageChunks << splitImage(model_->getImage(), CHUNK_WIDTH, CHUNK_HEIGHT);

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

    emit stringsListChanged();

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

    if (count < 0) { // Only emit signal if strings have been added.
        emit stringsListChanged();
    }

    return count;
}

bool ContextualizationController::removeString(QString stringId)
{
    if(model_->removeString(stringId)) { // Only emit signal if string has been added.
        emit stringsListChanged();

        return true;
    }

    return false;
}

bool ContextualizationController::removeString(int row)
{
    if(model_->removeString(row)) { // Only emit signal if string has been added.
        emit stringsListChanged();

        return true;
    }

    return false;
}

bool ContextualizationController::removeAllStrings()
{
    if (!model_->getStringsList().isEmpty()) {
        model_->removeAllStrings();

        emit stringsListChanged();

        return true;
    }

    return false;
}

void ContextualizationController::clearImage()
{
    model_->setImage(ContextualizationModel::NO_IMAGE_PATH);

    emit imageChanged();
}

bool ContextualizationController::setImage(const QString &image)
{
    QFileInfo imageInfo(image);
    QString imageName("capture-" + Utils::getDateTime() + '-' + username_ + '.' + imageInfo.suffix());
    bool exists = imageInfo.exists();

    if (exists) {
        // Save image in non volatil folder.
        if (QFile::copy(image, IMAGES_FOLDER + imageName)) {
            Log::writeLog("New image set in model: " + IMAGES_FOLDER + imageName);
        } else {
            Log::writeError("Could not save " + image + " in " + (IMAGES_FOLDER + imageName) + " to set it in the contextualization");
        }
    } else {
        Log::writeError("Image to set not exists: " + image);
    }

    // Remove old image in the model (only if has it) before set the new image.
    if (model_->hasImage()) {
        QFile::remove(model_->getImage());
    }

    model_->setImage(exists ? IMAGES_FOLDER + imageName : ContextualizationModel::NO_IMAGE_PATH);

    emit imageChanged();

    return exists;
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
    QFile configurationFile("../conf/general.conf");

    if (configurationFile.exists())
    {
        document = QJsonDocument::fromJson(Utils::readAllFile(configurationFile.fileName()), &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            Log::writeError("Error decoding configuration file: " + jsonError.errorString());

            return;
        }

        // Get object with the configuration.
        root = document.object();
        if (root.isEmpty()) {
            Log::writeError("Error format on configuration file. " + configurationFile.fileName());

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

        Log::writeLog("Loaded config from " + configurationFile.fileName());
    }
}

void ContextualizationController::saveConfig()
{
    QJsonObject root;
    QJsonArray validStates;
    QString configurationFile("../conf/general.conf");

    foreach (QString state, validStates_) {
        validStates << QJsonValue(state);
    }

    root.insert("english.fp", englishFpFile_);
    root.insert("remoteHost", remoteHost_);
    root.insert("validStates", validStates);

    // Write configuration in disk.
    if (Utils::writeFile(configurationFile, QString(QJsonDocument(root).toJson(QJsonDocument::Indented)))) {
        Log::writeLog("Configuration saved succesfully.");
    } else {
        Log::writeError("Could not save configuration in file " + configurationFile);
    }
}

int ContextualizationController::generateDoneFpFile()
{
    QStringList grepArguments;
    int codeError;

    grepArguments << "DONE$" << englishFpFile_;

    codeError = Utils::executeProgram("grep", grepArguments, DONE_FP_FILE);
    if (codeError == 0) {
        Log::writeLog("DONE fp file generated succesfully: " + DONE_FP_FILE);
    } else {
        Log::writeError("DONE fp file could not be generated: " + DONE_FP_FILE);
    }
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
            Log::writeLog("Saved image chunk: " + fileName);
        } else {
            Log::writeError("Could no saved image chunk: " + fileName);
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

bool ContextualizationController::isCommonWord(const QString &word)
{
    /**
     * Dictionary with the most used words that are longer than MIN_LENGTH_FOR_APPROXIMATE.
     */
    QStringList dictionary;

    dictionary << "Blueprints"
               << "Quickset"
               << "paper type"
               << "Modify quickset"
               << "Copy"
               << "Cancel"
               << "Output"
               << "Destination";

    foreach (QString text, dictionary) {
        if (text.contains(word, Qt::CaseInsensitive)) {
            return true;
        }
    }

    return false;
}
