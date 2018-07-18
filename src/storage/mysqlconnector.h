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
     * @copydoc DatabaseConnectorAbstract::getStringsWithValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithAproximateValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithAproximateValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringWithId(const QString &id, bool caseSensitive)
     */
    QList<String *> getStringWithId(const QString &id, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string)
     */
    bool insertString(const String &string) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings)
     */
    bool insertStrings(const QList<String *> &strings) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value, bool caseSensitive)
     */
    int removeStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithId(const QString &id, bool caseSensitive)
     */
    bool removeStringsWithId(const QString &id, bool caseSensitive = true) override;
};

#endif // MYSQLCONNECTOR_H
