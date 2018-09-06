/**
 * @file mysqlconnector.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class MySqlConnector
 *
 * @brief This is a class to access a MySQL database.
 */

#include "mysqlconnector.h"

MySqlConnector::MySqlConnector(
    const QString host,
    const QString user,
    const QString password,
    const QString database
) : DatabaseConnectorAbstract() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    bool ok;

    host_ = host;
    user_ = user;
    password_ = password;
    dataBase_ = database;

    db.setHostName(host_);
    db.setDatabaseName(dataBase_);
    db.setUserName(user_);
    db.setPassword(password_);

    ok = db.open();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " MySQL connection failed");
        Log::writeError(QString(Q_FUNC_INFO) + " " + db.lastError().databaseText());
    }
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

QList<String *> MySqlConnector::getAllStrings()
{
    QSqlQuery query;
    QList<String *> out;
    bool ok;

    ok = query.exec("SELECT * FROM String");
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << new FirmwareString(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }

    return out;
}

QList<String *> MySqlConnector::getStringsWithValue(const QString &value, bool caseSensitive)
{
    QSqlQuery query;
    QList<String *> out;
    bool ok;

    if(caseSensitive) {
        query.prepare("SELECT * FROM String WHERE upper(Value) = :value");
        query.bindValue(":value", value.toUpper());
    } else {
        query.prepare("SELECT * FROM String WHERE Value = :value");
        query.bindValue(":value", value);
    }

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << new FirmwareString(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }

    return out;
}

QList<String *> MySqlConnector::getStringsWithApproximateValue(const QString &value, bool caseSensitive)
{
    QList<String *> out;
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    foreach (String *string, getStringsWithValue(value, caseSensitive)) {
        /**
         * If the value belongs to a string, the fwString is saved, otherwise relsease memory of fwString.
         * If size of both strings is longer than MIN_LENGTH_FOR_APPROXIMATE and their size difference is less than
         * MAX_LENGTH_DIFFERENCE, a value is considered valid if it is contained within the fwString value or
         * vice versa.
         * If size of any strings is less than MIN_LENGTH_FOR_APPROXIMATE, a value is considered valid only if it
         * is equals than the value of fwString.
         */
        if (value.size() > MIN_LENGTH_FOR_APPROXIMATE && string->getValue().size() > MIN_LENGTH_FOR_APPROXIMATE &&
            qAbs(value.size() - string->getValue().size()) < MAX_LENGTH_DIFFERENCE) {
            if (value.contains(string->getValue(), isCaseSensitive) || string->getValue().contains(value, isCaseSensitive)) {
                out << string;
            } else {
                delete string;
            }
        } else {
            if (value.compare(string->getValue(), isCaseSensitive) == 0) {
                out << string;
            } else {
                delete string;
            }
        }
    }

    return out;
}

QList<String *> MySqlConnector::getStringWithId(const QString &id, bool caseSensitive)
{
    QSqlQuery query;
    QList<String *> out;
    bool ok;

    if(caseSensitive) {
        query.prepare("SELECT * FROM String WHERE upper(ID) = :id");
        query.bindValue(":value", id.toUpper());
    } else {
        query.prepare("SELECT * FROM String WHERE ID = :id");
        query.bindValue(":value", id);
    }

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << new FirmwareString(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }

    return out;
}

QList<String *> MySqlConnector::getStringsWithState(const QString state)
{
    QSqlQuery query;
    QList<String *> out;
    bool ok;

    query.prepare("SELECT * FROM String WHERE State = :state");
    query.bindValue(":value", state);

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << new FirmwareString(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString()
        );
    }

    return out;
}

bool MySqlConnector::insertString(const String &string, const QString language)
{
    QSqlQuery query;

    query.prepare("INSERT INTO String (ID, Value, Description, MaxLenght, State, LanguageID) "
                  "VALUES (:id, :value, :description, :maxLength, :state, :languageId)");
    query.bindValue(":id", string.getId());
    query.bindValue(":value", string.getValue());
    query.bindValue(":description", string.getDescription());
    query.bindValue(":maxLength", string.getMaxLength());
    query.bindValue(":state", string.getState());
    query.bindValue(":languageId", getKeyFromLanguage(language));

    if (query.exec()) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
        return true;
    }

    return false;
}

int MySqlConnector::insertStrings(const QList<String *> &strings, const QString language)
{
    int count = 0;

    foreach (String *string, strings) {
        count = insertString(*string, language) == true ? count+1 : count;
    }

    return count;
}

int MySqlConnector::removeStringsWithValue(const QString &value, bool caseSensitive)
{
    QSqlQuery query;
    bool ok;

    if(caseSensitive) {
        query.prepare("DELETE FROM String WHERE upper(Value) = :value");
        query.bindValue(":value", value.toUpper());
    } else {
        query.prepare("SELECT * FROM String WHERE Value = :value");
        query.bindValue(":value", value);
    }

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());

        return 0;
    }

    return query.numRowsAffected();
}

int MySqlConnector::removeStringsWithId(const QString &id, bool caseSensitive)
{
    QSqlQuery query;
    bool ok;

    if(caseSensitive) {
        query.prepare("DELETE FROM String WHERE upper(ID) = :id");
        query.bindValue(":value", id.toUpper());
    } else {
        query.prepare("SELECT * FROM String WHERE ID = :id");
        query.bindValue(":value", id);
    }

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());

        return 0;
    }

    return query.numRowsAffected();
}

QStringList MySqlConnector::getLanguages()
{
    QSqlQuery query;
    QStringList out;
    bool ok;

    ok = query.exec("SELECT Name FROM Language");
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << query.value(0).toString();
    }

    return out;
}

QStringList MySqlConnector::getLanguageIds()
{
    QSqlQuery query;
    QStringList out;
    bool ok;

    ok = query.exec("SELECT ID FROM Language");
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << query.value(0).toString();
    }

    return out;
}

QString MySqlConnector::getKeyFromLanguage(const QString language)
{
    QSqlQuery query;
    QStringList out;
    bool ok;

    query.prepare("SELECT ID FROM Language where Name = :language");
    query.bindValue(":language", language);

    ok = query.exec();
    if (!ok) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + query.lastError().databaseText());
    }

    while (query.next()) {
        out << query.value(0).toString();
    }

    return out.isEmpty() ? QString() : out.first();
}

bool MySqlConnector::isAnyImportantFieldEmpty() {
    if (user_.isEmpty() || password_.isEmpty() || dataBase_.isEmpty()) {
        return true;
    }

    return false;
}
