#include "firmwarestring.h"

FirmwareString::FirmwareString(
    const QString &id,
    const QString &value,
    const QString &description,
    const QString &maxLength,
    const QString &state,
    const bool selected
) {
    this->id = id;
    this->value = value;
    this->description = description;
    this->maxLength = maxLength;
    this->state = state;
    this->selected = selected;
}

QString FirmwareString::getId() const
{
    return this->id;
}

QString FirmwareString::getValue() const
{
    return this->value;
}

QString FirmwareString::getDescription() const
{
    return this->description;
}

QString FirmwareString::getMaxLength() const
{
    return this->maxLength;
}

QString FirmwareString::getState() const
{
    return this->state;
}

void FirmwareString::setValue(const QString &value)
{
    this->value = value;
}

bool FirmwareString::isSelected() const
{
    return this->selected;
}

void FirmwareString::select()
{
    this->selected = true;
}

void FirmwareString::unselect()
{
    this->selected = false;
}

QString FirmwareString::toJson(QJsonDocument::JsonFormat format)
{
    return QString(QJsonDocument(this->toJsonObject()).toJson(format));
}

QJsonObject FirmwareString::toJsonObject()
{
    QJsonObject string;

    string.insert("id", QJsonValue(this->id));
    string.insert("value", QJsonValue(this->value));
    string.insert("description", QJsonValue(this->description));
    string.insert("maxLength", QJsonValue(this->maxLength));
    string.insert("state", QJsonValue(this->state));
    string.insert("selected", QJsonValue(this->selected));

    return string;
}
