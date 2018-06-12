#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include "tools/log.h"

class FirmwareString
{
public:

    /**
     * @brief Constructs an empty firmware string.
     */
    FirmwareString();

    /**
     * @brief Constructs a firmware string replic from another received by parameter.
     * @param other Firmware string to be duplicated.
     */
    FirmwareString(FirmwareString &other);

    /**
     * @brief Constructs a firmware string with diferents value members received by parameter.
     * @param id Firmware string identifier.
     * @param value Firmware string value.
     * @param description Firmware string description.
     * @param maxLength Firmware string maxLength.
     * @param state Firmware string state.
     * @param selected Firmware string selected.
     */
    FirmwareString(
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
     * @brief Returns a #FirmwareString initializated with the JSON string json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json String in JSON format to be converted.
     * @return ContextualizationModel
     */
    static FirmwareString * fromJson(QString &json);

    /**
     * @brief Returns a #FirmwareString initializated with the #QByteArray data json.
     *
     * If the JSON data received is not valid, return an empty model.
     * @param json QByteArray in JSON format to be converted.
     * @return FirmwareString
     */
    static FirmwareString * fromJson(QByteArray &json);

    /**
     * @brief Assigns other to firmware string and returns a reference to this firmware string.
     * @param other Model to be copied.
     * @return Reference to this firmware string.
     */
    FirmwareString & operator=(const FirmwareString &other);

private:
    QString id;             //< Identifier of the firmware string.
    QString value;          //< Value of the firmware string.
    QString description;    //< Description of the firmware string.
    QString maxLength;      //< Max length of the firmware string.
    QString state;          //< State of the firmware string.
    bool selected;          //< Indicates if the string is selected by user.
};

#endif // FIRMWARESTRING_H
