#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QString>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include "firmwarestring.h"
#include "log.h"

class ContextualizationModel
{
public:
    ContextualizationModel(
        QString image = "./Resources/noImage.png",
        QList<FirmwareString *> list = QList<FirmwareString *>()
    );
    ~ContextualizationModel();

    void addNewString(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected
    );
    void addNewString(FirmwareString * newString);
    void addNewStrings(QList<FirmwareString *> &list);
    void deleteString(QString &id);
    void deleteString(int pos);
    void clearStringsList();
    QList<FirmwareString *> & getStringsList();
    void setImagePath(QString path);
    QString getImagePath();
    bool isEmpty();
    void clear();
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::Compact);
    QJsonObject toJsonObject();
    static ContextualizationModel * fromJson(QString &json);
    static ContextualizationModel * fromJson(QByteArray &json);
    ContextualizationModel & operator=(ContextualizationModel &other);

private:
    QString image;
    QList<FirmwareString *> stringsList;
};

#endif // CONTEXTUALIZATIONMODEL_H
