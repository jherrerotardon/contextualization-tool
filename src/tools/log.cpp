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
}

void Log::writeLog(QString text)
{
    write(logFile_, text);
}

void Log::writeError(QString text)
{
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
