#include "contextualizationmodel.h"

const QString ContextualizationModel::NO_IMAGE_URL = "qrc:/images/imageNotAvailable.png";
const QString ContextualizationModel::NO_IMAGE_PATH =
    QDir("../resources/images").absoluteFilePath("imageNotAvailable.png");

ContextualizationModel::ContextualizationModel(QString image, QList<FirmwareString *> list) : QObject()
{
    this->image = image;
    this->addStrings(list);
}

ContextualizationModel::~ContextualizationModel(){

    foreach (FirmwareString *fwString, this->stringsList) {
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
    this->addString(newString);
}

void ContextualizationModel::addString(FirmwareString *newString)
{
    this->stringsList.append(newString);

    emit stringsListChanged();
}

void ContextualizationModel::addStrings(QList<FirmwareString *> &strings)
{
    foreach (FirmwareString *fwString, strings) {
        this->addString(fwString);
    }
}

bool ContextualizationModel::removeString(QString &id)
{
    foreach (FirmwareString *fwString, this->stringsList) {
        if (fwString->getId() == id) {
            this->stringsList.removeOne(fwString);
            delete fwString;

            emit stringsListChanged();

            return true;
        }
    }

    return false;
}

bool ContextualizationModel::removeString(int pos)
{
    FirmwareString *stringToRemove;

    if (pos < this->stringsList.size()) {
        stringToRemove = this->stringsList.at(pos);
        this->stringsList.removeAt(pos);
        delete stringToRemove;

        emit stringsListChanged();

        return true;
    }

    return false;
}

void ContextualizationModel::clearStringsList()
{
    foreach (FirmwareString *fwString, this->stringsList) {
        delete fwString;
        fwString = Q_NULLPTR;
    }

    this->stringsList.clear();

    emit stringsListChanged();
}

QList<FirmwareString *> &ContextualizationModel::getStringsList()
{
    return this->stringsList;
}

void ContextualizationModel::setImage(QString path)
{
    this->image = path;

    emit imageChanged();
}

QString ContextualizationModel::getImage()
{
    return this->image;
}

bool ContextualizationModel::isEmpty()
{
    if (this->image.isEmpty() && this->stringsList.isEmpty()) {
        return true;
    }

    return false;
}

void ContextualizationModel::clear()
{
    this->image = "";
    this->clearStringsList();

    emit modelChanged();
}

QString ContextualizationModel::toJson(QJsonDocument::JsonFormat format)
{
    return QString(QJsonDocument(this->toJsonObject()).toJson(format));
}

QJsonObject ContextualizationModel::toJsonObject()
{
    QJsonObject root;
    QJsonObject stringsList;

    for (int count = 0; count < this->stringsList.size(); ++count) {
        stringsList.insert(QString::number(count),  this->stringsList.at(count)->toJsonObject());
    }

    root.insert("image", QJsonValue(this->image));
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
        foreach (FirmwareString *string, stringsList) {
            delete string; ///< Release memory because of an error on json decode process.
            string = Q_NULLPTR;
        }
        return Q_NULLPTR;
    }

    return new ContextualizationModel(image, stringsList);
}

ContextualizationModel & ContextualizationModel::operator=(ContextualizationModel &other)
{
    if (this != &other) {
        if (!this->isEmpty()) {
            this->clear();
        }

        this->setImage(other.image);
        foreach (FirmwareString *fwString, other.getStringsList()) {
            this->addString(new FirmwareString(*fwString));
        }
    }

    return *this;
}
