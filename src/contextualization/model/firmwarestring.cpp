#include "firmwarestring.h"

FirmwareString::FirmwareString() : String()
{

}

FirmwareString::FirmwareString(FirmwareString &other) : String()
{
    id_ = other.getId();
    value_ = other.getValue();
    description_ = other.getDescription();
    maxLength_ = other.getMaxLength();
    state_ = other.getState();
    selected_ = other.isSelected();
}

FirmwareString::FirmwareString(
    const QString &id,
    const QString &value,
    const QString &description,
    const QString &maxLength,
    const QString &state,
    const bool selected
) : String() {
    id_ = id;
    value_ = value;
    description_ = description;
    maxLength_ = maxLength;
    state_ = state;
    selected_ = selected;
}

FirmwareString * FirmwareString::fromJson(QString &json)
{
    return static_cast<FirmwareString *>(String::fromJson(json));
}

FirmwareString * FirmwareString::fromJson(QByteArray &json)
{
    return static_cast<FirmwareString *>(String::fromJson(json));
}

FirmwareString & FirmwareString::operator=(const FirmwareString &other)
{
    if (this != &other) {
        id_ = other.getId();
        value_ = other.getValue();
        description_ = other.getDescription();
        maxLength_ = other.getMaxLength();
        state_ = other.getState();
        selected_ = other.isSelected();
    }

    return *this;
}
