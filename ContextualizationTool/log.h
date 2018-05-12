#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>

class Log
{
public:
    Log();

    static void writeDebug(QString &text);
    static void writeLog(QString &text);
    static void writeError(QString &text);

private:
    const static QString debugFile;
    const static QString logFile;
    const static QString errorFile;

    static void write(const QString &path, QString &text);
};

#endif // LOG_H
