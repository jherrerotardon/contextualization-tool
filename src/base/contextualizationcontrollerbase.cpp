#include "contextualizationcontrollerbase.h"

const int ContextualizationControllerBase::CHUNK_WIDTH = 300;
const int ContextualizationControllerBase::CHUNK_HEIGHT = 150;
const QString ContextualizationControllerBase::DONE_FP_FILE = "/tmp/doneFpFile.fp";
const QString ContextualizationControllerBase::IMAGES_FOLDER = QDir("../storage/images").absolutePath() + '/';
const QString ContextualizationControllerBase::PROJECTS_FOLDER = QDir("../storage/projects").absolutePath() + '/';
const int ContextualizationControllerBase::MIN_LENGTH_FOR_APPROXIMATE = 6;

ContextualizationControllerBase::ContextualizationControllerBase(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);

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

    emit imageChanged();
    emit stringsListChanged();

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

QList<FirmwareString *> ContextualizationControllerBase::detectStringsOnImage()
{
    Ocr *worker;
    QList<Ocr *> workers; // Used to save workers because after must be released.
    QList<QFuture<QList<FirmwareString *>>> futures;
    QList<FirmwareString *> fwStrings;
    QStringList imageChunks;
    QString rootCopy;

    // Added root image copy to work it too.
    rootCopy = "/tmp/rootCopy." + QFileInfo(model_->getImage()).suffix();
    if (QFile::copy(model_->getImage(), "/tmp/rootCopy." + QFileInfo(model_->getImage()).suffix())) {
        imageChunks << rootCopy;
    }

    // Split image.
    imageChunks << splitImage(model_->getImage(), CHUNK_WIDTH, CHUNK_HEIGHT);

    // Create and start workers.
    foreach (QString image, imageChunks) {
        worker = new Ocr(image);
        workers << worker;

       futures <<  QtConcurrent::run(
            [this, worker]() {
                QList<FirmwareString *> out;

                out = this->processStrings(worker->extract());

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
    foreach (Ocr *worker, workers) {
        delete worker;
        worker = Q_NULLPTR;
    }

    //Delete image chunks from disk.
    foreach (QString path, imageChunks) {
        QFile::remove(path);
    }


    return fwStrings;
}

QList<FirmwareString *> ContextualizationControllerBase::processStrings(QStringList strings)
{
    QList<FirmwareString *> stringsFound;
    QList<FirmwareString *> out;

    foreach (QString string, strings) {
        stringsFound = findString(string, ByApproximateValue);
        if (!stringsFound.isEmpty()) {
            out << stringsFound;
        }
    }

    return out;
}

QList<FirmwareString *> ContextualizationControllerBase::findString(const QString &text, const FindType findType)
{
    QString line;
    QFile file;
    QList<FirmwareString *> stringsFound;
    FirmwareString *fwString = Q_NULLPTR;

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

    // Begin read file.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file.fileName());

        return stringsFound;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        line = in.readLine();
        fwString = fragmentFpLine(line);
        if (fwString) {
            //If the text belongs to a string the fwString is saved, otherwise relsease memory of fwString.
            if (isOnFwString(*fwString, text, findType))
            {
                stringsFound << fwString;
            } else {
                delete fwString;
            }
        }
    }

    file.close();

    // If only have to get DONE strings and find was not in DONE_FP_FILE is necessary filer strings.
    if (onlyDoneStrings_ && file.fileName() != DONE_FP_FILE) {
        filterStringsByState(&stringsFound, "DONE");
    }

    return stringsFound;
}

FirmwareString * ContextualizationControllerBase::fragmentFpLine(QString &fpLine)
{
    QString id;
    QString value;
    QString description;
    QString maxLength;
    QString state;
    bool selected;
    bool hasError = false;

    if (fpLine.isEmpty()) {
        return Q_NULLPTR;
    }

    // Amazing regular expresion to fragment fp line.
    QRegularExpression regex(
        QString("MESSAGE_ID  (?<id>\\w+)  \"(?<value>.+)\"") +
        QString(" \\|\\| ") +
        QString("TEXT_DESCRIPTION  \"(?<description>.*)\"") +
        QString(" \\|\\| ") +
        QString("MAX_FIELD_WIDTH  (?<maxLength>\\d+( \\+ \\d+)*)") +
        QString(" \\|\\| ") +
        QString("LOCALIZATION  (?<state>\\w+)")
    );
    QRegularExpressionMatch match = regex.match(fpLine);

    if (match.hasMatch()) {
        id = match.captured("id");
        value = match.captured("value");
        description = match.captured("description");
        maxLength = match.captured("maxLength");
        state = match.captured("state");
    }

    // If any attribute was not taken, an error is indicated.
    if (id.isNull()) {
        hasError = true;

        Log::writeError("Error format in MESSAGE_ID column. Cannot extract the id: " + fpLine);
    }

    if (value.isNull()) {
        hasError = true;

        Log::writeError("Error format in MESSAGE_ID column. Cannot extract the value: " + fpLine);
    }

    if (maxLength.isNull()) {
        hasError = true;

        Log::writeError("Error format in MAX_FIELD_WIDTH column. Cannot extract the max width: " + fpLine);
    }

    if (!isValidState(state)) {
        hasError = true;

        Log::writeError("Error format in LOCALIZATION column. Cannot extract state or isn't a valid state: "+ fpLine);
    }

    if (hasError) {
        return Q_NULLPTR;
    }

    selected = state == "DONE" ? true : false;

    return new FirmwareString(id, value, description, maxLength, state, selected);
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
    int hasError;
    QStringList arguments;
    QString path("/tmp/capture.png");

    arguments << path;
    hasError = Utils::executeProgram("import", arguments, QProcess::nullDevice(), QString(), 30000);

    if (hasError) {
        Log::writeError("Error taking capture. Code exit of import process: " + hasError);
    }

    return hasError ? QString() : path;
}

bool ContextualizationControllerBase::setImage(const QString &image)
{
    QFileInfo imageInfo(image);
    QString imageName("capture-" + getDateTime() + '-' + username_ + '.' + imageInfo.suffix());
    bool exists = imageInfo.exists();

    if (exists) {
        // Save image in non volatil folder.
        QFile::copy(image, IMAGES_FOLDER + imageName);
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
    }
}

void ContextualizationControllerBase::saveConfig()
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
    Utils::writeFile(configurationFile, QString(QJsonDocument(root).toJson(QJsonDocument::Indented)));
}

int ContextualizationControllerBase::generateDoneFpFile()
{
    QStringList grepArguments;

    grepArguments << "DONE$" << englishFpFile_;

    return Utils::executeProgram("grep", grepArguments, DONE_FP_FILE);
}

int ContextualizationControllerBase::filterStringsByState(QList<FirmwareString *> *list, const QString &state)
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

QStringList ContextualizationControllerBase::splitImage(
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

    // Save on disk images.
    for (int i = 0; i < imageChunks.size(); ++i) {
        fileName = "/tmp/" + rootImage.baseName() + "_chunk_" + QString::number(i+1) + '.' + rootImage.suffix();

        // Only returns chunks saved succesfully.
        if (imageChunks.at(i).save(fileName, Q_NULLPTR, 100)) {
            chunks << fileName;
        } else {
            if (ok) {
                *ok = false;
            }
        }
    }

    return chunks;
}

bool ContextualizationControllerBase::isOnFwString(const FirmwareString &fwString,
        const QString &value,
        FindType findType
) {
    Qt::CaseSensitivity caseSensitive = caseSensitive_ ? Qt::CaseSensitive : Qt::CaseInsensitive;
    QString text = value.simplified();  // Remove extra whitespaces.

    // If is a common word, always filter by ID.
    findType = isCommonWord(text) ? ByValue : findType;

    switch (findType) {
    case ByID:
        // A identifier is considered valid only if it is equals than the identifier of fwString.
        return text.compare(fwString.getId(), caseSensitive) == 0 ? true : false;

    case ByValue:
        // A value is considered valid only if it is equals than the value of fwString.
        return text.compare(fwString.getValue(), caseSensitive) == 0 ? true : false;

    case ByApproximateValue:
        /**
         * If size of both strings is longer than MIN_LENGTH_FOR_APPROXIMATE, a value is considered valid if
         * it is contained within the fwString value or vice versa.
         * If size of any strings is not longer than MIN_LENGTH_FOR_APPROXIMATE, a value is considered valid only if it
         * is equals than the value of fwString.
         **/
        if (text.size() > MIN_LENGTH_FOR_APPROXIMATE && fwString.getValue().size() > MIN_LENGTH_FOR_APPROXIMATE) {
            return text.contains(fwString.getValue(), caseSensitive) || fwString.getValue().contains(text, caseSensitive);
        } else {
            return text.compare(fwString.getValue(), caseSensitive) == 0 ? true : false;
        }

    default:
        return false;
    }
}

bool ContextualizationControllerBase::isCommonWord(const QString &word)
{
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
