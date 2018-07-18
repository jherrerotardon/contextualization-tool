#ifndef DATABASEABSTRACT_H
#define DATABASEABSTRACT_H

#include "contextualization/model/string.h"
#include "tools/log.h"

class DatabaseConnectorAbstract
{
public:

    /**
     * @brief Creates an empty DatabaseConnectorAbstract.
     */
    DatabaseConnectorAbstract();

    /**
     * @brief Returns a list containing all strings in the dababase.
     * @return List with strings.
     */
    virtual QList<String *> getAllStrings() = 0;

    /**
     * @brief Returns all strigns in database with the value received by argument.
     * @param value String value.
     * @return List with strings.
     */
    virtual QList<String *> getStringsWithValue(const QString &value) = 0;

    /**
     * @brief Returns all strigns in database with the identifier received by argument.
     * @param value String identifier.
     * @return List with strings.
     */
    virtual QList<String *> getStringWithId(const QString &id) = 0;

    /**
     * @brief Inserts a new string in database.
     *
     * Returns true if the insertion was succesfull, otherwise, returns false.
     * @param string String instance to be inserted.
     * @return bool
     */
    virtual bool insertString(const String &string) = 0;

    /**
     * @brief Inserts a set of strings into the database.
     *
     * Returns true if the insertion was succesfull, otherwise, returns false.
     * @param strings List with strings to be added into databse.
     * @return bool
     */
    virtual bool insertStrings(const QList<String *> &strings) = 0;

    /**
     * @brief Removes from databse all strigns with the value received by argument.
     *
     * Returns the number of removed strings.
     * @param value String value.
     * @return Number of removed strings
     */
    virtual int removeStringsWithValue(const QString &value) = 0;

    /**
     * @brief Removes from databse the strign with the identifier received by argument.
     * @param value String identifier.
     * @return bool
     */
    virtual bool removeStringWithId(const QString &id) = 0;
};

#endif // DATABASEABSTRACT_H
