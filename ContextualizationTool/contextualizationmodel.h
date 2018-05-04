#ifndef CONTEXTUALIZATIONMODEL_H
#define CONTEXTUALIZATIONMODEL_H

#include <QString>
#include "firmwarestring.h"


class ContextualizationModel
{
private:
    QString image;
    FirmwareString * stringsList;
    
public:
    ContextualizationModel();
    ~ContextualizationModel();

    int addNewString(const QString &id, const QString &value, const QString &description, int maxLength, const QString &state, bool selected);
    int deleteString(QString &id);
    FirmwareString * getStringsList();
    void setImagePath(QString &path);
    QString getImagePath();
    int emptyModel();

    
};

#endif // CONTEXTUALIZATIONMODEL_H
