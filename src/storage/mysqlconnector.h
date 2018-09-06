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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "contextualization/model/firmwarestring.h"
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
        const QString host,
        const QString user,
        const QString password,
        const QString database = QString()
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
     * @copydoc DatabaseConnectorAbstract::getStringsWithState(const QString state)
     */
    QList<String *> getStringsWithState(const QString state) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string, const QString language)
     */
    bool insertString(const String &string, const QString language) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings, const QString language)
     */
    int insertStrings(const QList<String *> &strings, const QString language) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value, bool caseSensitive)
     */
    int removeStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithId(const QString &id, bool caseSensitive)
     */
    int removeStringsWithId(const QString &id, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getLanguages()
     */
    QStringList getLanguages() override;

    /**
     * @copydoc DatabaseConnectorAbstract::getLanguageIds()
     */
    QStringList getLanguageIds() override;

    /**
     * @brief Returns the key of language received by parameter
     * @param language Language to match key
     * @return Language ID
     */
    QString getKeyFromLanguage(const QString language);

private:
    QString host_;
    QString user_;
    QString password_;
    QString dataBase_;

    /**
     * @brief Checks if user, password, dataBase or tableName is empty.
     *
     * Returns true if any of them is empty, else returns false.
     * @return
     */
    bool isAnyImportantFieldEmpty();
};

#endif // MYSQLCONNECTOR_H
