#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include <QString>

class FirmwareString
{
public:
    FirmwareString(QString &id, QString &value, QString &description, QString maxLength, QString &state, bool selected);
    /*********/
    FirmwareString(QString &value);
    /***********/
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
    //enum TraductionState {TODO, DONE, VALIDATED};
    QString id;
    QString value;
    QString description;
    QString maxLength;
    QString state;
    bool selected;
};

#endif // FIRMWARESTRING_H
