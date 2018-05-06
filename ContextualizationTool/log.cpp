#include "log.h"

const QString Log::debugFile = "/tmp/contextualization.debug";
const QString Log::errorFile = "/tmp/contextualization.err";
const QString Log::logFile = "/tmp/contextualization.log";

Log::Log()
{

}

void Log::writeDebug(QString &text)
{
    QFile file(debugFile);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " " + qgetenv("USER") << " " << text << '\n';
        file.close();
    }
}

void Log::writeLog(QString &text)
{
    QFile file(logFile);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " " + qgetenv("USER") << " " << text << '\n';
        file.close();
    }
}

void Log::writeError(QString &text)
{
    QFile file(errorFile);

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " " + qgetenv("USER") << " " << text << '\n';
        file.close();
    }
}
