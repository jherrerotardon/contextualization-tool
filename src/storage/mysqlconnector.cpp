#include "mysqlconnector.h"

MySqlConnector::MySqlConnector() : DatabaseConnectorAbstract()
{

}

QList<String *> MySqlConnector::getAllStrings()  {

}

QList<String *> MySqlConnector::getStringsWithValue(const QString &value, bool caseSensitive)  {

}

QList<String *> MySqlConnector::getStringsWithAproximateValue(const QString &value, bool caseSensitive) {

}

QList<String *> MySqlConnector::getStringWithId(const QString &id, bool caseSensitive)  {

}

bool MySqlConnector::insertString(const String &string)  {

}

bool MySqlConnector::insertStrings(const QList<String *> &strings)  {

}

int MySqlConnector::removeStringsWithValue(const QString &value, bool caseSensitive)  {

}

bool MySqlConnector::removeStringsWithId(const QString &id, bool caseSensitive)  {

}
