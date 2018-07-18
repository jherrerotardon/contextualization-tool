#ifndef MYSQLCONNECTOR_H
#define MYSQLCONNECTOR_H

#include "databaseconnectorabstract.h"

class MySqlConnector : public DatabaseConnectorAbstract
{
public:

    /**
     * @brief Creates an empty MySqlConnector.
     */
    MySqlConnector();

    /**
     * @copydoc DatabaseConnectorAbstract::getAllStrings(const QString &value)
     */
    QList<String *> getAllStrings() override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithValue()
     */
    QList<String *> getStringsWithValue(const QString &value) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringWithId(const QString &id)
     */
    QList<String *> getStringWithId(const QString &id) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string)
     */
    bool insertString(const String &string) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings)
     */
    bool insertStrings(const QList<String *> &strings) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value)
     */
    int removeStringsWithValue(const QString &value) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringWithId(const QString &id)
     */
    bool removeStringWithId(const QString &id) override;
};

#endif // MYSQLCONNECTOR_H
