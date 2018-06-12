#ifndef LOG_H
#define LOG_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>

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
    const static QString debugFile; ///< Path of file for debug messages.
    const static QString logFile;   ///< Path of file for log messages.
    const static QString errorFile; ///< Path of file for errors messages.

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
