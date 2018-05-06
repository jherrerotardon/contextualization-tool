#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QString>
#include <QList>
#include "firmwarestring.h"


class ContextualizationModel
{
private:
    QString image;
    QList<FirmwareString *> stringsList;
    
public:
    ContextualizationModel();
    ~ContextualizationModel();

    int addNewString(const QString &id, const QString &value, const QString &description, QString maxLength, const QString &state, bool selected);
    void addNewString(FirmwareString * newString);
    void deleteString(QString &id);
    void deleteString(int pos);
    void clearStringsList();
    QList<FirmwareString *> &getStringsList();
    void setImagePath(QString &path);
    QString getImagePath();
    int emptyModel();

    
};

#endif // CONTEXTUALIZATIONMODEL_H
