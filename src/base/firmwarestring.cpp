#include "firmwarestring.h"

FirmwareString::FirmwareString() : QObject()
{
    id_ = "";
    value_ = "";
    description_ = "";
    maxLength_ = "";
    state_ = "";
    selected_ = false;
}

FirmwareString::FirmwareString(FirmwareString &other) : QObject()
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
) : QObject() {
    id_ = id;
    value_ = value;
    description_ = description;
    maxLength_ = maxLength;
    state_ = state;
    selected_ = selected;
}

QString FirmwareString::getId() const
{
    return id_;
}

void FirmwareString::setId(const QString &id)
{
    id_ = id;

    emit idChanged();
}

QString FirmwareString::getValue() const
{
    return value_;
}

void FirmwareString::setValue(const QString &value)
{
    value_ = value;

    emit valueChanged();
}

QString FirmwareString::getDescription() const
{
    return description_;
}

void FirmwareString::setDescription(const QString &description)
{
    description_ = description;

    emit descriptionChanged();
}

QString FirmwareString::getMaxLength() const
{
    return maxLength_;
}

void FirmwareString::setMaxLength(const QString &maxLength)
{
    maxLength_ = maxLength;

    emit maxLengthChanged();
}

QString FirmwareString::getState() const
{
    return state_;
}

void FirmwareString::setState(const QString &state)
{
    state_ = state;

    emit stateChanged();
}

bool FirmwareString::isEmpty()
{
    if (value_.isEmpty()) {
        return true;
    }

    return false;
}

bool FirmwareString::isSelected() const
{
    return selected_;
}

void FirmwareString::select()
{
    selected_ = true;

    emit selectedChanged();
}

void FirmwareString::unselect()
{
    selected_ = false;

    emit selectedChanged();
}

void FirmwareString::setSelected(bool selected)
{
    selected_ = selected;

    emit selectedChanged();
}

QString FirmwareString::toFpFileFormat()
{
    QString out;

    out += "MESSAGE_ID  " + id_ + "  \"" + value_ + '"';
    out += " || ";
    out += "TEXT_DESCRIPTION  \"" + description_ + '"';
    out += " || ";
    out += "MAX_FIELD_WIDTH  " + maxLength_;
    out += " || ";
    out += "LOCALIZATION  " + state_;

    return out;
}

QString FirmwareString::toJson(QJsonDocument::JsonFormat format)
{
    return QString(QJsonDocument(this->toJsonObject()).toJson(format));
}

QJsonObject FirmwareString::toJsonObject()
{
    QJsonObject string;

    string.insert("id", QJsonValue(id_));
    string.insert("value", QJsonValue(value_));
    string.insert("description", QJsonValue(description_));
    string.insert("maxLength", QJsonValue(maxLength_));
    string.insert("state", QJsonValue(state_));
    string.insert("selected", QJsonValue(selected_));

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
    id_ = other.getId();
    value_ = other.getValue();
    description_ = other.getDescription();
    maxLength_ = other.getMaxLength();
    state_ = other.getState();
    selected_ = other.isSelected();

    return *this;
}
