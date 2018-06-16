#include "contextualizationmodel.h"

const QString ContextualizationModel::NO_IMAGE_PATH = QDir("../resources/images").absoluteFilePath("imageNotAvailable.png");

ContextualizationModel::ContextualizationModel(QString image, QList<FirmwareString *> list) : QObject()
{
    image_ = image;

    if (!list.isEmpty()) {
        addStrings(list);
    }
}

ContextualizationModel::~ContextualizationModel(){

    foreach (QObject *fwString, stringsList_) {
        delete fwString;
        fwString = Q_NULLPTR;
    }
}

void ContextualizationModel::addString(
    const QString &id,
    const QString &value,
    const QString &description,
    const QString &maxLength,
    const QString &state,
    const bool selected
) {
    FirmwareString *newString = new FirmwareString(id, value, description, maxLength, state, selected);
    addString(newString);
}

void ContextualizationModel::addString(FirmwareString *newString)
{
    stringsList_.append(newString);
}

void ContextualizationModel::addStrings(QList<FirmwareString *> &strings)
{
    foreach (QObject *fwString, strings) {
        addString(static_cast<FirmwareString *>(fwString));
    }
}

bool ContextualizationModel::removeString(QString &id)
{
    FirmwareString * string;

    foreach (QObject *fwString, stringsList_) {
        string = static_cast<FirmwareString *>(fwString);
        if (string->getId() == id) {
            stringsList_.removeOne(string);
            delete string;

            return true;
        }
    }

    return false;
}

bool ContextualizationModel::removeString(int pos)
{
    FirmwareString *stringToRemove;

    if (pos < stringsList_.size()) {
        stringToRemove = static_cast<FirmwareString *>(stringsList_.at(pos));
        stringsList_.removeAt(pos);
        delete stringToRemove;

        return true;
    }

    return false;
}

void ContextualizationModel::removeAllStrings()
{
    foreach (QObject *fwString, stringsList_) {
        delete fwString;
        fwString = Q_NULLPTR;
    }

    stringsList_.clear();
}

QList<QObject *> &ContextualizationModel::getStringsList()
{
    return stringsList_;
}

void ContextualizationModel::setImage(QString path)
{
    image_ = path;
}

QString ContextualizationModel::getImage()
{
    return image_;
}

bool ContextualizationModel::isEmpty()
{
    if (hasImage() && hasStrings()) {
        return true;
    }

    return false;
}

bool ContextualizationModel::hasImage()
{
    return !image_.isEmpty() && image_ != NO_IMAGE_PATH;
}

bool ContextualizationModel::hasStrings()
{
    return !stringsList_.isEmpty();
}

void ContextualizationModel::clear()
{
    image_ = "";
    removeAllStrings();
}

QString ContextualizationModel::toJson(QJsonDocument::JsonFormat format)
{
    return QString(QJsonDocument(toJsonObject()).toJson(format));
}

QJsonObject ContextualizationModel::toJsonObject()
{
    QJsonObject root;
    QJsonObject stringsList;
    FirmwareString *fwString;

    for (int count = 0; count < stringsList_.size(); ++count) {
        fwString = static_cast<FirmwareString *>(stringsList_.at(count));
        stringsList.insert(QString::number(count),  fwString->toJsonObject());
    }

    root.insert("image", QJsonValue(image_));
    root.insert("strings", stringsList);

    return root;
}

ContextualizationModel * ContextualizationModel::fromJson(QString &json)
{
   QByteArray jsonData = json.toUtf8();

   return ContextualizationModel::fromJson(jsonData);
}

ContextualizationModel * ContextualizationModel::fromJson(QByteArray &json)
{
    bool hasError = false;
    QJsonObject root;
    QString image;
    QJsonObject stringsListJsonObject;
    QList<FirmwareString *> stringsList;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(json, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        Log::writeError(jsonError.errorString());
        return Q_NULLPTR;
    }

    root = document.object();
    if (root.isEmpty()) {
        return Q_NULLPTR;
    }

    image = root.value("image").toString();
    if (image.isEmpty()) {
        return Q_NULLPTR;
    }

    stringsListJsonObject = root.value("strings").toObject();
    foreach (QString key, stringsListJsonObject.keys()) {
        FirmwareString *fwString;
        QJsonObject stringJsonObject;
        QByteArray data;

        stringJsonObject = stringsListJsonObject.value(key).toObject();
        if (stringJsonObject.isEmpty()) {
            hasError = true;
            break;
        }

        data = QJsonDocument(stringJsonObject).toJson();
        fwString = FirmwareString::fromJson(data);
        if (fwString == Q_NULLPTR) {
            hasError = true;
            break;
        }

        stringsList.append(fwString);
    }

    if (hasError) {
        foreach (QObject *string, stringsList) {
            delete string; ///< Release memory because of an error on json decode process.
            string = Q_NULLPTR;
        }
        return Q_NULLPTR;
    }

    return new ContextualizationModel(image, stringsList);
}

ContextualizationModel & ContextualizationModel::operator=(ContextualizationModel &other)
{
    FirmwareString *newString;;

    if (this != &other) {
        if (!isEmpty()) {
            clear();
        }

        setImage(other.getImage());
        foreach (QObject *fwString, other.getStringsList()) {
            newString = static_cast<FirmwareString *>(fwString);
            addString(new FirmwareString(*newString));
        }
    }

    return *this;
}
