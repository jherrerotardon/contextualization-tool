#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include "string.h"

class FirmwareString : public String
{
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
     * @brief Converts a line of fp file in a FirmwareString if is possible.
     *
     * Return null if there is a format error in the line.
     * @param line Contains string to fragment.
     * @param lineNumber Contains the number of line on the file.
     * @return FirmwareString *|null
     */
    static FirmwareString * fromFpLine(QString &fpLine);

    /**
     * @brief Assigns other to firmware string and returns a reference to this firmware string.
     * @param other Model to be copied.
     * @return Reference to this firmware string.
     */
    FirmwareString & operator=(const FirmwareString &other);
};

#endif // FIRMWARESTRING_H
