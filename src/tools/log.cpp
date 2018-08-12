/**
 * @file log.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class Log
 *
 * @brief This is a static class to write logs in different channels.
 */

#include "log.h"

const QString Log::debugFile_ = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first()
    + "/contextualization.debug";
const QString Log::logFile_ = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first()
    + "/contextualization.log";
const QString Log::errorFile_ = QStandardPaths::standardLocations(QStandardPaths::TempLocation).first()
    + "/contextualization.err";

Log::Log()
{

}

void Log::writeDebug(QString text)
{
    write(debugFile_, text);

    #ifdef DEVELOP
    qDebug() << text;
    #endif
}

void Log::writeLog(QString text)
{
    #ifdef DEVELOP
    qDebug() << text;
    #endif
    write(logFile_, text);
}

void Log::writeError(QString text)
{
    #ifdef DEVELOP
    qCritical() << text;
    #endif
    write(errorFile_, text);
}

inline void Log::write(const QString &path, QString &text)
{
    QFile file(path);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")
            << " "
            << qgetenv("USER")
            << " "
            << text
            << '\n';
        file.close();
    }
}
