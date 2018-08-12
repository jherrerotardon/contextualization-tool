/**
 * @file log.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class Log
 *
 * @brief This is a static class to write logs in different channels.
 */

#ifndef LOG_H
#define LOG_H

// Comment this define when when the application will be ran in a production environment.
#ifndef DEVELOP
#define DEVELOP
#endif


#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QStandardPaths>
#include <QDebug>

class Log
{
public:

    /**
     * @brief Constructs an empty log. Doesn't do anything.
     */
    Log();

    /**
     * @brief Appends a debug mensage containig text received by parameter in the debug file.
     * @param text Text to write in file.
     */
    static void writeDebug(QString text);

    /**
     * @brief Appends a log mensage containig text received by parameter in the log file.
     * @param text Text to write in file.
     */
    static void writeLog(QString text);

    /**
     * @brief Appends an error mensage containig text received by parameter in the error file.
     * @param text Text to write in file.
     */
    static void writeError(QString text);

private:
    const static QString debugFile_; ///< Path of file for debug messages.
    const static QString logFile_;   ///< Path of file for log messages.
    const static QString errorFile_; ///< Path of file for errors messages.

    /**
     * @brief Appends a text in a file.
     *
     * The content of the file is not deleted or overwritten.
     * @param path File path where the text will be added.
     * @param text Text to append in file.
     */
    static void write(const QString &path, QString &text);
};

#endif // LOG_H
