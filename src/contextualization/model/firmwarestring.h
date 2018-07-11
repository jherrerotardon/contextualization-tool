#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include "string.h"

class FirmwareString : public String
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
     * @copydoc String::String();
     */
    FirmwareString();

    /**
     * @copydoc String::String(String &other);
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
     * @copydoc String::fromJson(QString &json)
     */
    static FirmwareString * fromJson(QString &json);

    /**
     * @copydoc String::fromJson(QByteArray &json)
     */
    static FirmwareString * fromJson(QByteArray &json);

    /**
     * @brief Assigns other to firmware string and returns a reference to this firmware string.
     * @param other Model to be copied.
     * @return Reference to this firmware string.
     */
    FirmwareString & operator=(const FirmwareString &other);
};

#endif // FIRMWARESTRING_H
