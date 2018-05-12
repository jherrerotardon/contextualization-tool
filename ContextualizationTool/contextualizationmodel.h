#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QString>
#include <QList>
#include "firmwarestring.h"


class ContextualizationModel
{
public:
    ContextualizationModel();
    ~ContextualizationModel();

    void addNewString(const QString &id, const QString &value, const QString &description, const QString &maxLength, const QString &state, const bool selected);
    void addNewString(FirmwareString * newString);
    void deleteString(QString &id);
    void deleteString(int pos);
    void clearStringsList();
    QList<FirmwareString *> & getStringsList();
    void setImagePath(QString &path);
    QString getImagePath();
    int emptyModel();

private:
    QString image;
    QList<FirmwareString *> stringsList;    
};

#endif // CONTEXTUALIZATIONMODEL_H
