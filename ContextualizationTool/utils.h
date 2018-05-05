#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

class Utils
{
public:
    Utils();

    static void errorMessage(const QString &text, const QString &informativeText);
    static int warningMessage(const QString &text, const QString &informativeText);
    static int appendFile(QString &path, QString &text);

};

#endif // UTILS_H
