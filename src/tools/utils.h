#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QProcess>

class Utils
{
public:
    Utils();

    static void errorMessage(const QString &text, const QString &informativeText);
    static int warningMessage(const QString &text, const QString &informativeText);
    static void informativeMessage(const QString &text, const QString &informativeText);
    static int appendFile(const QString &path, const QString &text);
    static int writeFile(const QString &path, const QString &text);
    static QByteArray readAllFile(const QString &path);
    static int executeProgram(
        const QString &program,
        const QStringList &arguments = QStringList(),
        const QString &workDirectory = QString(),
        const int timeout = -1
    );
    static QString zipCompressDirectoryContents(
        const QString &directory,
        const QString &zipDestination,
        const QString &zipName = "compressed"
    );
};

#endif // UTILS_H
