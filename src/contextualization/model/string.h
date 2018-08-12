/**
 * @file string.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class String
 *
 * @brief This is the representation of a string with their properties.
 */

#ifndef STRING_H
#define STRING_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include "tools/log.h"

class String : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString maxLength READ getId WRITE setMaxLength NOTIFY maxLengthChanged)
    Q_PROPERTY(QString state READ getState WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected NOTIFY idChanged)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable NOTIFY editableChanged)

public:

    /**
     * @brief Constructs an empty string.
     */
    String();

    /**
     * @brief Constructs a string replic from another received by parameter.
     * @param other string to be duplicated.
     */
    String(String &other);

    /**
     * @brief Constructs a string with diferents value members received by parameter.
     * @param id String identifier.
     * @param value String value.
     * @param description String description.
     * @param maxLength String maxLength.
     * @param state String state.
     * @param selected String selected.
     * @param editable String editable state.
     */
    String(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected = false,
        const bool editable = true
    );

    /**
     * @brief Returns the identifier of the string.
     * @return Identifier of the string.
     */
    QString getId() const;

    /**
     * @brief Sets the identifier of the string.
     * @param id New identifier of the string.
     */
    bool setId(const QString &id);

    /**
     * @brief Returns the value of the string.
     *
     * Returns true if the identifier was set, otherwise, returns false.
     * @return Value of the string.
     */
    QString getValue() const;

    /**
     * @brief Sets the value of the string.
     *
     * Returns true if the value was set, otherwise, returns false.
     * @param value New value of the string.
     */
    bool setValue(const QString &value);

    /**
     * @brief Returns the description of the string.
     * @return Description of the string.
     */
    QString getDescription() const;

    /**
     * @brief Sets the description of the string.
     *
     * Returns true if the description was set, otherwise, returns false.
     * @param description New description of the string.
     */
    bool setDescription(const QString &description);

    /**
     * @brief Returns the max length of the string.
     * @return Max length of the string.
     */
    QString getMaxLength() const;

    /**
     * @brief Sets the max length of the string.
     *
     * Returns true if the max length was set, otherwise, returns false.
     * @param maxLength New max length of the string.
     */
    bool setMaxLength(const QString &maxLength);

    /**
     * @brief Returns the state of the string.
     * @return State of the string.
     */
    QString getState() const;

    /**
     * @brief Sets the state of the string.
     *
     * Returns true if the state was set, otherwise, returns false.
     * @param state New state of the string.
     */
    bool setState(const QString &state);

    /**
     * @brief Returns true if string is empty.
     *
     * A string is empty if has a null o empty value.
     * @return bool
     */
    bool isEmpty();

    /**
     * @brief Returns true if string is selected, else returns false.
     * @return bool
     */
    bool isSelected() const;

    /**
     * @brief Changes the string to selected.
     */
    void select();

    /**
     * @brief Changes the string to unselected.
     */
    void unselect();

    /**
     * @brief Changes the string to selected or unselected depending on the value received by parameter.
     * @param selected New selection state of the string..
     */
    void setSelected(bool selected);

    /**
     * @brief Returns the editable state of the string.
     * @return Editable state of the string.
     */
    bool isEditable() const;

    /**
     * @brief Sets the editable parameter of the string.
     * @param state New editable state of the string.
     */
    void setEditable(bool editable);

    /**
     * @brief Returns the model converted to a string with the format in file fp.
     * @return A string.
     */
    QString toFpFileFormat();

    /**
     * @brief Returns the string converted to a JSON QString.
     * @param format Exit format of JSON.
     * @return A JSON string,
     */
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact);

    /**
     * @brief Returns the string converted to a #QJsonObject.
     * @return QJsonObject
     */
    QJsonObject toJsonObject();

    /**
     * @brief Returns a #String initializated with the JSON string json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json String in JSON format to be converted.
     * @return ContextualizationModel
     */
    static String * fromJson(QString &json);

    /**
     * @brief Returns a #String initializated with the #QByteArray data json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json QByteArray in JSON format to be converted.
     * @return String
     */
    static String * fromJson(QByteArray &json);

    /**
     * @brief Assigns other to string and returns a reference to this string.
     *
     * Overloads fromString(QString &json) function.
     * @param other Model to be copied.
     * @return Reference to this string.
     */
    String & operator=(const String &other);

protected:
    QString id_;            //< Identifier of the string.
    QString value_;         //< Value of the string.
    QString description_;   //< Description of the string.
    QString maxLength_;     //< Max length of the string.
    QString state_;         //< State of the string.
    bool selected_;         //< Indicates if the string is selected by user.
    bool editable_;         //< Indicates if the string is editable.

private:
signals:

    /**
     * @brief The signal is emitted when the identifier of the string has changed.
     */
    void idChanged();

    /**
     * @brief The signal is emitted when the value of the string has changed.
     */
    void valueChanged();

    /**
     * @brief The signal is emitted when the description of the string has changed.
     */
    void descriptionChanged();

    /**
     * @brief The signal is emitted when the max length of the string has changed.
     */
    void maxLengthChanged();

    /**
     * @brief The signal is emitted when the state of the string has changed.
     */
    void stateChanged();

    /**
     * @brief The signal is emitted when the string is selected or unselected.
     */
    void selectedChanged();

    /**
     * @brief The signal is emitted when the editable parameter is changed.
     */
    void editableChanged();
};

#endif // STRING_H
