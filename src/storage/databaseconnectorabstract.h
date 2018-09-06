/**
 * @file databaseconnectorabstract.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class DatabaseConnectorAbstract
 *
 * @brief This is an interface to access a different data bases.
 */

#ifndef DATABASEABSTRACT_H
#define DATABASEABSTRACT_H

#include "contextualization/model/string.h"
#include "tools/log.h"
#include "tools/utils.h"

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
    virtual QList<String *> getStringsWithValue(const QString &value, bool caseSensitive = true) = 0;

    /**
     * @brief Returns all strigns in database with a similar value received by argument.
     * @param value String value.
     * @return List with strings.
     */
    virtual QList<String *> getStringsWithApproximateValue(const QString &value, bool caseSensitive = true) = 0;

    /**
     * @brief Returns all strigns in database with the identifier received by argument.
     * @param value String identifier.
     * @return List with strings.
     */
    virtual QList<String *> getStringWithId(const QString &id, bool caseSensitive = true) = 0;

    /**
     * @brief Returns strings with state received by parameter.
     * @param state State to be found.
     * @return String list eith indicated state.
     */
    virtual QList<String *> getStringsWithState(const QString state) = 0;
    /**
     * @brief Inserts a new string in database.
     *
     * Returns true if the insertion was succesfull, otherwise, returns false.
     * @param string String instance to be inserted.
     * @return bool
     */
    virtual bool insertString(const String &string, const QString language) = 0;

    /**
     * @brief Inserts a set of strings into the database.
     *
     * Returns the number of inserted stirngs.
     * @param strings List with strings to be added into databse.
     * @return bool
     */
    virtual int insertStrings(const QList<String *> &strings, const QString language) = 0;

    /**
     * @brief Removes from databse all strigns with the value received by argument.
     *
     * Returns the number of removed strings.
     * @param value String value.
     * @return Number of removed strings
     */
    virtual int removeStringsWithValue(const QString &value, bool caseSensitive = true) = 0;

    /**
     * @brief Removes from databse the strign with the identifier received by argument.
     *
     * Returns the number of removed stirngs.
     * @param value String identifier.
     * @return bool
     */
    virtual int removeStringsWithId(const QString &id, bool caseSensitive = true) = 0;

    /**
     * @brief Returns all languages stored in database.
     * @return Languages used.
     */
    virtual QStringList getLanguages() = 0;

    /**
     * @brief Returns all language identifiers stored in database.
     * @return Languages used.
     */
    virtual QStringList getLanguageIds() = 0;

protected:
    const int MIN_LENGTH_FOR_APPROXIMATE;       ///< Minimun length for string to do an approximate find.
    const int MAX_LENGTH_DIFFERENCE;            ///< Greatest difference between value and string values found.
};

#endif // DATABASEABSTRACT_H
