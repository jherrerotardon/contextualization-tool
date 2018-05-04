#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <QString>
#include <QFile>
#include <QTextStream>

using namespace std;


class Log
{
public:
    Log();
    static void writeDebug(string text);
    static void writeLog(QString text);

private:
    const static string debugFile;
};

#endif // LOG_H
