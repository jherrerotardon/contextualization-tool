#include "log.h"

const QString Log::debugFile = "/tmp/contextualization.debug";
const QString Log::logFile = "/tmp/contextualization.log";
const QString Log::errorFile = "/tmp/contextualization.err";

Log::Log()
{

}

void Log::writeDebug(QString &text)
{
    write(debugFile, text);
}

void Log::writeLog(QString &text)
{
    write(logFile, text);
}

void Log::writeError(QString &text)
{
    write(errorFile, text);
}

inline void Log::write(const QString &path, QString &text)
{
    QFile file(path);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " " + qgetenv("USER") << " " << text << '\n';
        file.close();
    }
}
