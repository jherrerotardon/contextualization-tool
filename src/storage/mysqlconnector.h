/**
 * @file mysqlconnector.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class MySqlConnector
 *
 * @brief This is a class to access a MySQL database.
 */

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
     * @brief Creates a MySqlConnector setting values received by argument.
     * @param user Username to access database.
     * @param password Password for the username.
     * @param database Data base name.
     * @param table Table name.
     */
    MySqlConnector(
        const QString user,
        const QString password,
        const QString database = QString(),
        const QString tableName = QString()
    );

    /**
     * @brief Returns a user name to access database.
     * @return User name.
     */
    QString getUser();

    /**
     * @brief Sets username to access database.
     * @param user User name.
     */
    void setUser(const QString user);

    /**
     * @brief Sets password to access database.
     * @param user Passwrod.
     */
    void setPassword(const QString password);

    /**
     * @brief Returns a data base name to access database.
     * @return User name.
     */
    QString getDataBase();

    /**
     * @brief Sets data base to access database.
     * @param user User name.
     */
    void setDataBase(const QString dataBase);

    /**
     * @brief Returns a table name to access database.
     * @return User name.
     */
    QString getTableName();

    /**
     * @brief Sets table to access database.
     * @param user User name.
     */
    void setTableName(const QString tableName);

    /**
     * @copydoc DatabaseConnectorAbstract::getAllStrings(const QString &value)
     */
    QList<String *> getAllStrings() override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithApproximateValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithApproximateValue(const QString &value, bool caseSensitive = true) override;

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
    int insertStrings(const QList<String *> &strings) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value, bool caseSensitive)
     */
    int removeStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithId(const QString &id, bool caseSensitive)
     */
    int removeStringsWithId(const QString &id, bool caseSensitive = true) override;

private:
    QString user_;
    QString password_;
    QString dataBase_;
    QString tableName_;

    /**
     * @brief Checks if user, password, dataBase or tableName is empty.
     *
     * Returns true if any of them is empty, else returns false.
     * @return
     */
    bool isAnyImportantFieldEmpty();
};

#endif // MYSQLCONNECTOR_H
