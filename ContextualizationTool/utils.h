#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QMessageBox>

class Utils
{
public:
    Utils();

    static void errorMessage(const QString &text, const QString &informativeText);
    static int warningMessage(const QString &text, const QString &informativeText);
};

#endif // UTILS_H
