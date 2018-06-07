#ifndef FIRMWARESTRING_H
#define FIRMWARESTRING_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include "log.h"

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
    FirmwareString();
    FirmwareString(FirmwareString &other);

    QString getId() const;
    QString getValue() const;
    QString getDescription() const;
    QString getMaxLength() const;
    QString getState() const;
    void setValue(const QString &value);
    bool isEmpty();
    bool isSelected() const;
    void select();
    void unselect();
    QString toFpFileFormat();
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact);
    QJsonObject toJsonObject();
    static FirmwareString * fromJson(QString &json);
    static FirmwareString * fromJson(QByteArray &json);
    FirmwareString & operator=(const FirmwareString &other);

private:
    QString id;
    QString value;
    QString description;
    QString maxLength;
    QString state;
    bool selected;
};

#endif // FIRMWARESTRING_H
