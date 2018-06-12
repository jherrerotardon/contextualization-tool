#include "firmwarestring.h"

FirmwareString::FirmwareString()
{
    this->id = "";
    this->value = "";
    this->description = "";
    this->maxLength = "";
    this->state = "";
    this->selected = false;
}

FirmwareString::FirmwareString(FirmwareString &other)
{
    this->id = other.id;
    this->value = other.value;
    this->description = other.description;
    this->maxLength = other.maxLength;
    this->state = other.state;
    this->selected = other.selected;
}

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

void FirmwareString::setId(const QString &id)
{
    this->id = id;
}

QString FirmwareString::getValue() const
{
    return this->value;
}

void FirmwareString::setValue(const QString &value)
{
    this->value = value;
}

QString FirmwareString::getDescription() const
{
    return this->description;
}

void FirmwareString::setDescription(const QString &description)
{
    this->description = description;
}

QString FirmwareString::getMaxLength() const
{
    return this->maxLength;
}

void FirmwareString::setMaxLength(const QString &maxLength)
{
    this->maxLength = maxLength;
}

QString FirmwareString::getState() const
{
    return this->state;
}

void FirmwareString::setState(const QString &state)
{
    this->state = state;
}

bool FirmwareString::isEmpty()
{
    if (this->value.isEmpty()) {
        return true;
    }

    return false;
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

QString FirmwareString::toFpFileFormat()
{
    QString out;

    out += "MESSAGE_ID  " + this->id + "  \"" + this->value + '"';
    out += " || ";
    out += "TEXT_DESCRIPTION  \"" + this->description + '"';
    out += " || ";
    out += "MAX_FIELD_WIDTH  " + this->maxLength;
    out += " || ";
    out += "LOCALIZATION  " + this->state;

    return out;
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

FirmwareString * FirmwareString::fromJson(QString &json)
{
    QByteArray jsonData = json.toUtf8();

    return FirmwareString::fromJson(jsonData);
}

FirmwareString * FirmwareString::fromJson(QByteArray &json)
{
    QString value;
    QJsonObject root;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(json, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        Log::writeError("Error decoding FirmwareString json: " + jsonError.errorString());
        return Q_NULLPTR;
    }

    root = document.object();
    if (root.isEmpty()) {
        Log::writeError("FirmwareString fromJson(): error format on root object.");
        return Q_NULLPTR;
    }

    value = root.value("value").toString();
    if (value.isEmpty()) {
        Log::writeError("FirmwareString fromJson(): key value is empty.");
        return Q_NULLPTR;
    }

    return new FirmwareString(
        root.value("id").toString(),
        value,
        root.value("description").toString(),
        root.value("maxLength").toString(),
        root.value("state").toString(),
        root.value("selected").toBool()
    );
}

FirmwareString & FirmwareString::operator=(const FirmwareString &other)
{
    this->id = other.id;
    this->value = other.value;
    this->description = other.description;
    this->maxLength = other.maxLength;
    this->state = other.state;
    this->selected = other.selected;

    return *this;
}
