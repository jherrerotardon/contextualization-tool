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
    const bool selected,
    const bool editable
) : String(id, value, description, maxLength, state, selected, editable) {

}

FirmwareString * FirmwareString::fromJson(QString &json)
{
    return static_cast<FirmwareString *>(String::fromJson(json));
}

FirmwareString * FirmwareString::fromJson(QByteArray &json)
{
    return static_cast<FirmwareString *>(String::fromJson(json));
}

FirmwareString * FirmwareString::fromFpLine(const QString &fpLine)
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

    if (state.isNull()) {
        hasError = true;

        Log::writeError("Error format in LOCALIZATION column. Cannot extract state or isn't a valid state: "+ fpLine);
    }

    if (hasError) {
        return Q_NULLPTR;
    }

    selected = state == "DONE" ? true : false;

    return new FirmwareString(id, value, description, maxLength, state, selected);
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
