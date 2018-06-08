#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QString>
#include <QList>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include "firmwarestring.h"
#include "src/tools/log.h"

class ContextualizationModel
{
public:
    const static QString NO_IMAGE_URL;
    const static QString NO_IMAGE_PATH;

    ContextualizationModel(
        QString image = QString(),
        QList<FirmwareString *> list = QList<FirmwareString *>()
    );
    ~ContextualizationModel();

    void addString(
        const QString &id,
        const QString &value,
        const QString &description,
        const QString &maxLength,
        const QString &state,
        const bool selected
    );
    void addString(FirmwareString * newString);
    void addStrings(QList<FirmwareString *> &list);
    bool removeString(QString &id);
    bool removeString(int pos);
    void clearStringsList();
    QList<FirmwareString *> & getStringsList();
    void setImage(QString path);
    QString getImage();
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
