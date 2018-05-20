#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include <QString>

class FirmwareString
{
public:
    FirmwareString(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected
    );
    QString getId() const;
    QString getValue() const;
    QString getDescription() const;
    QString getMaxLength() const;
    QString getState() const;
    void setValue(const QString &value);
    bool isSelected() const;
    void select();
    void unselect();

private:
    QString id;
    QString value;
    QString description;
    QString maxLength;
    QString state;
    bool selected;
};

#endif // FIRMWARESTRING_H
