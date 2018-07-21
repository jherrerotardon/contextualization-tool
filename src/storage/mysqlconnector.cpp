#include "mysqlconnector.h"

MySqlConnector::MySqlConnector() : DatabaseConnectorAbstract()
{

}

MySqlConnector::MySqlConnector(
    const QString user,
    const QString password,
    const QString database,
    const QString tableName
) {
    user_ = user;
    password_ = password;
    dataBase_ = database;
    tableName_ = tableName;
}

QString MySqlConnector::getUser()
{
    return user_;
}

void MySqlConnector::setUser(const QString user)
{
    user_ = user;
}

void MySqlConnector::setPassword(const QString password)
{
    password_ = password;
}

QString MySqlConnector::getDataBase()
{
    return dataBase_;
}

void MySqlConnector::setDataBase(const QString dataBase)
{
    dataBase_ = dataBase;
}

QString MySqlConnector::getTableName()
{
    return tableName_;
}

void MySqlConnector::setTableName(const QString tableName)
{
    tableName_ = tableName;
}

QList<String *> MySqlConnector::getAllStrings()
{

}

QList<String *> MySqlConnector::getStringsWithValue(const QString &value, bool caseSensitive)
{

}

QList<String *> MySqlConnector::getStringsWithApproximateValue(const QString &value, bool caseSensitive)
{

}

QList<String *> MySqlConnector::getStringWithId(const QString &id, bool caseSensitive)
{

}

bool MySqlConnector::insertString(const String &string)
{

}

int MySqlConnector::insertStrings(const QList<String *> &strings)
{

}

int MySqlConnector::removeStringsWithValue(const QString &value, bool caseSensitive)
{

}

int MySqlConnector::removeStringsWithId(const QString &id, bool caseSensitive)
{

}

bool MySqlConnector::isAnyImportantFieldEmpty() {
    if (user_.isEmpty() || password_.isEmpty() || dataBase_.isEmpty() || tableName_.isEmpty()) {
        return true;
    }

    return false;
}
