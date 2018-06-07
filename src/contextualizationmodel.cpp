#include "contextualizationmodel.h"

const QString ContextualizationModel::NO_IMAGE_URL = "qrc:/images/imageNotAvailable.png";
const QString ContextualizationModel::NO_IMAGE_PATH =
    QDir("../resources/images").absoluteFilePath("imageNotAvailable.png");

ContextualizationModel::ContextualizationModel(QString image, QList<FirmwareString *> list)
{
    this->image = image;
    this->stringsList = list;
}

ContextualizationModel::~ContextualizationModel(){

    foreach (FirmwareString *fwString, this->stringsList) {
        delete fwString;
        fwString = nullptr;
    }
}

void ContextualizationModel::addNewString(
    const QString &id,
    const QString &value,
    const QString &description,
    const QString &maxLength,
    const QString &state,
    const bool selected
) {
    FirmwareString *newString = new FirmwareString(id, value, description, maxLength, state, selected);
    this->stringsList.append(newString);
}

void ContextualizationModel::addNewString(FirmwareString *newString)
{
    this->stringsList.append(newString);
}

void ContextualizationModel::addNewStrings(QList<FirmwareString *> &list)
{
    foreach (FirmwareString *fwString, list) {
        this->addNewString(new FirmwareString(*fwString));
    }
}

void ContextualizationModel::deleteString(QString &id)
{
    foreach (FirmwareString *fwString, this->stringsList)
        if (fwString->getId() == id) {
            this->stringsList.removeOne(fwString);
            delete fwString;
            break;
        }
}

void ContextualizationModel::deleteString(int pos)
{
    FirmwareString *stringToRemove;

    stringToRemove = this->stringsList.at(pos);
    this->stringsList.removeAt(pos);
    delete stringToRemove;
}

void ContextualizationModel::clearStringsList()
{
    foreach (FirmwareString *fwString, this->stringsList) {
        delete fwString;
        fwString = nullptr;
    }

    this->stringsList.clear();
}

QList<FirmwareString *> &ContextualizationModel::getStringsList()
{
    return this->stringsList;
}

void ContextualizationModel::setImagePath(QString path)
{
    this->image = path;
}

QString ContextualizationModel::getImagePath()
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
    this->image = "./Resources/noImage.png";
    this->clearStringsList();
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
        return nullptr;
    }

    root = document.object();
    if (root.isEmpty()) {
        return nullptr;
    }

    image = root.value("image").toString();
    if (image.isEmpty()) {
        return nullptr;
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
        if (fwString == nullptr) {
            hasError = true;
            break;
        }

        stringsList.append(fwString);
    }

    if (hasError) {
        foreach (FirmwareString *string, stringsList) {
            delete string; ///< Release memory because of an error on json decode process.
            string = nullptr;
        }
        return nullptr;
    }

    return new ContextualizationModel(image, stringsList);
}

ContextualizationModel & ContextualizationModel::operator=(ContextualizationModel &other)
{
    if (this != &other) {
        this->clear();
        this->image = other.image;
        this->addNewStrings(other.getStringsList());
//        foreach (FirmwareString *fwString, other.getStringsList()) {
//            this->addNewString(fwString);
//        }
        //this->stringsList.append(other.getStringsList());
    }

    return *this;
}
