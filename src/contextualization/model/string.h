#ifndef STRING_H
#define STRING_H
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include "tools/log.h"

class String : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString value READ getValue WRITE setId NOTIFY valueChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString maxLength READ getId WRITE setMaxLength NOTIFY maxLengthChanged)
    Q_PROPERTY(QString state READ getState WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool selected READ isSelected WRITE setSelected NOTIFY idChanged)

public:

    /**
     * @brief Constructs an empty firmware string.
     */
    String();

    /**
     * @brief Constructs a firmware string replic from another received by parameter.
     * @param other Firmware string to be duplicated.
     */
    String(String &other);

    /**
     * @brief Constructs a firmware string with diferents value members received by parameter.
     * @param id Firmware string identifier.
     * @param value Firmware string value.
     * @param description Firmware string description.
     * @param maxLength Firmware string maxLength.
     * @param state Firmware string state.
     * @param selected Firmware string selected.
     */
    String(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected
    );

    /**
     * @brief Returns the identifier of the string.
     * @return Identifier of the string.
     */
    QString getId() const;

    /**
     * @brief Sets the identifier of the firmware string.
     * @param id New identifier of the firmware string.
     */
    void setId(const QString &id);

    /**
     * @brief Returns the value of the firmware string.
     * @return Value of the firmware string.
     */
    QString getValue() const;

    /**
     * @brief Sets the value of the firmware string.
     * @param value New value of the firmware string.
     */
    void setValue(const QString &value);

    /**
     * @brief Returns the description of the firmware string.
     * @return Description of the firmware string.
     */
    QString getDescription() const;

    /**
     * @brief Sets the description of the firmware string.
     * @param description New description of the firmware string.
     */
    void setDescription(const QString &description);

    /**
     * @brief Returns the max length of the firmware string.
     * @return Max length of the firmware string.
     */
    QString getMaxLength() const;

    /**
     * @brief Sets the max length of the firmware string.
     * @param maxLength New max length of the firmware string.
     */
    void setMaxLength(const QString &maxLength);

    /**
     * @brief Returns the state of the firmware string.
     * @return State of the firmware string.
     */
    QString getState() const;

    /**
     * @brief Sets the state of the firmware string.
     * @param state New state of the firmware string.
     */
    void setState(const QString &state);

    /**
     * @brief Returns true if firmware string is empty.
     *
     * A firmware string is empty if has a null o empty value.
     * @return bool
     */
    bool isEmpty();

    /**
     * @brief Returns true if firmware string is selected, else returns false.
     * @return bool
     */
    bool isSelected() const;

    /**
     * @brief Changes the firmware string to selected.
     */
    void select();

    /**
     * @brief Changes the firmware string to unselected.
     */
    void unselect();

    /**
     * @brief Changes the firmware string to selected or unselected depending on the value received by parameter.
     * @param selected New selection state of the firmware string..
     */
    void setSelected(bool selected);

    /**
     * @brief Returns the model converted to a string with the format in file fp.
     * @return A string.
     */
    QString toFpFileFormat();

    /**
     * @brief Returns the firmware string converted to a JSON QString.
     * @param format Exit format of JSON.
     * @return A JSON string,
     */
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact);

    /**
     * @brief Returns the firmware string converted to a #QJsonObject.
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
     * @brief Assigns other to firmware string and returns a reference to this firmware string.
     * @param other Model to be copied.
     * @return Reference to this firmware string.
     */
    String & operator=(const String &other);

protected:
    QString id_;            //< Identifier of the firmware string.
    QString value_;         //< Value of the firmware string.
    QString description_;   //< Description of the firmware string.
    QString maxLength_;     //< Max length of the firmware string.
    QString state_;         //< State of the firmware string.
    bool selected_;         //< Indicates if the string is selected by user.

private:
signals:

    /**
     * @brief The signal is emitted when the identifier of the firmware string has changed.
     */
    void idChanged();

    /**
     * @brief The signal is emitted when the value of the firmware string has changed.
     */
    void valueChanged();

    /**
     * @brief The signal is emitted when the description of the firmware string has changed.
     */
    void descriptionChanged();

    /**
     * @brief The signal is emitted when the max length of the firmware string has changed.
     */
    void maxLengthChanged();

    /**
     * @brief The signal is emitted when the state of the firmware string has changed.
     */
    void stateChanged();

    /**
     * @brief The signal is emitted the firmware string is selected or unselected.
     */
    void selectedChanged();
};

#endif // STRING_H
