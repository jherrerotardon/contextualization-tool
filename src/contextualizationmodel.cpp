#include "contextualizationmodel.h"

ContextualizationModel::ContextualizationModel()
{
    image = "";
}

ContextualizationModel::~ContextualizationModel(){

    foreach (FirmwareString *fwString, this->stringsList)
        delete fwString;
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
    this->stringsList.clear();
}

QList<FirmwareString *> & ContextualizationModel::getStringsList()
{
    return stringsList;
}

void ContextualizationModel::setImagePath(QString &path)
{
    this->image = path;
}

QString ContextualizationModel::getImagePath()
{
    return this->image;
}

void ContextualizationModel::clear()
{
    this->image = "";
    foreach (FirmwareString *fwString, this->stringsList) {
        delete fwString;
        fwString = nullptr;
    }

    this->stringsList.clear();
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