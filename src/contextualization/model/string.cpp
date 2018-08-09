#include "string.h"

String::String() : QObject()
{
    id_ = "";
    value_ = "";
    description_ = "";
    maxLength_ = "";
    state_ = "";
    selected_ = false;
}

String::String(String &other) : QObject()
{
    id_ = other.getId();
    value_ = other.getValue();
    description_ = other.getDescription();
    maxLength_ = other.getMaxLength();
    state_ = other.getState();
    selected_ = other.isSelected();
    editable_ = other.isEditable();
}

String::String(
    const QString &id,
    const QString &value,
    const QString &description,
    const QString &maxLength,
    const QString &state,
    const bool selected,
    const bool editable
) : QObject() {
    id_ = id;
    value_ = value;
    description_ = description;
    maxLength_ = maxLength;
    state_ = state;
    selected_ = selected;
    editable_ = editable;
}

QString String::getId() const
{
    return id_;
}

bool String::setId(const QString &id)
{
    if (editable_) {
        id_ = id;

        emit idChanged();
        return true;
    }

    return false;
}

QString String::getValue() const
{
    return value_;
}

bool String::setValue(const QString &value)
{
    if (editable_) {
        value_ = value;

        emit valueChanged();
        return true;
    }

    return false;
}

QString String::getDescription() const
{
    return description_;
}

bool String::setDescription(const QString &description)
{
    if (editable_) {
        description_ = description;

        emit descriptionChanged();
        return true;
    }

    return false;
}

QString String::getMaxLength() const
{
    return maxLength_;
}

bool String::setMaxLength(const QString &maxLength)
{
    if (editable_) {
        maxLength_ = maxLength;

        emit maxLengthChanged();
        return true;
    }

    return false;
}

QString String::getState() const
{
    return state_;
}

bool String::setState(const QString &state)
{
    if (editable_) {
        state_ = state;

        emit stateChanged();
        return true;
    }

    return false;
}

bool String::isEmpty()
{
    if (value_.isEmpty()) {
        return true;
    }

    return false;
}

bool String::isSelected() const
{
    return selected_;
}

void String::select()
{
    selected_ = true;

    emit selectedChanged();
}

void String::unselect()
{
    selected_ = false;

    emit selectedChanged();
}

void String::setSelected(bool selected)
{
    selected_ = selected;

    emit selectedChanged();
}

bool String::isEditable() const
{
    return editable_;
}

bool String::setEditable(const bool editable)
{
    editable_ = editable;

    emit editableChanged();
}

QString String::toFpFileFormat()
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

QString String::toJson(QJsonDocument::JsonFormat format)
{
    return QString(QJsonDocument(toJsonObject()).toJson(format));
}

QJsonObject String::toJsonObject()
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

String * String::fromJson(QString &json)
{
    QByteArray jsonData = json.toUtf8();

    return String::fromJson(jsonData);
}

String * String::fromJson(QByteArray &json)
{
    QString value;
    QJsonObject root;
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(json, &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        Log::writeError("Error decoding String json: " + jsonError.errorString());

        return Q_NULLPTR;
    }

    root = document.object();
    if (root.isEmpty()) {
        Log::writeError("String fromJson(): error format on root object.");

        return Q_NULLPTR;
    }

    value = root.value("value").toString();
    if (value.isEmpty()) {
        Log::writeError("String fromJson(): key value is empty.");

        return Q_NULLPTR;
    }

    return new String(
        root.value("id").toString(),
        value,
        root.value("description").toString(),
        root.value("maxLength").toString(),
        root.value("state").toString(),
        root.value("selected").toBool()
    );
}

String & String::operator=(const String &other)
{
    if (this != &other) {
        id_ = other.getId();
        value_ = other.getValue();
        description_ = other.getDescription();
        maxLength_ = other.getMaxLength();
        state_ = other.getState();
        selected_ = other.isSelected();
        editable_ = other.editable_;
    }

    return *this;
}
