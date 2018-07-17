#ifndef DATABASEABSTRACT_H
#define DATABASEABSTRACT_H

#include "contextualization/model/string.h"

class DatabaseConnectorAbstract
{
public:
    DatabaseConnectorAbstract();

    virtual QList<String *> getAllStrings() = 0;
};

#endif // DATABASEABSTRACT_H
