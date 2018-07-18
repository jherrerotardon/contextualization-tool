#ifndef UTILS_H
#define UTILS_H

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QProgressDialog>
#include <QString>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QDateTime>

class Utils
{
public:

    /**
     * @brief Creates an empty Utils object.
     */
    Utils();

    /**
     * @brief Creates a dialog and displayed it as a modal dialog. The dialog shown is an error dialog.
     *
     * The user only can accept the dialog.
     *
     * The function is designed to display error messages to the user without the option of choosing anything.
     * @param text Message text to be displayed.
     * @param informativeText Informative text that provides a fuller description for the message.
     */
    static void errorMessage(const QString &text, const QString &informativeText);

    /**
     * @brief Creates a dialog and displayed it as a modal dialog. The dialog is a warning dialog. and only has to
     * possible responses: Yes or No.
     *
     * The dialog has only two buttons (Yes and No), which are the two possible options that the user can select.
     * Returns the option selected by the user.
     *
     * The function is designed to display warning messages to the user and whether he wants to take the risks.
     * @param text Message text to be displayed.
     * @param informativeText Informative text that provides a fuller description for the message.
     * @return User responser.
     */
    static int warningMessage(const QString &text, const QString &informativeText);

    /**
     * @brief Creates a dialog and displayed it as a modal dialog. The dialog shown is an informative dialog.
     *
     * The user only can accept the dialog.
     *
     * The function is designed to display informative messages to the user.
     * @param text Message text to be displayed.
     * @param informativeText Informative text that provides a fuller description for the message.
     */
    static void informativeMessage(const QString &text, const QString &informativeText);

    /**
     * @brief Appends a text on a file.
     *
     * If the file exists is not overwritten.
     * If the file does not exist is created.
     * Returns true if the text is added succesfully, otherwise returns false.
     * @param path File where will append the text.
     * @param text Text to be appended on the file.
     * @return bool
     */
    static bool appendFile(const QString &path, const QString &text);

    /**
     * @brief Write a text on a file.
     *
     * If the file exists is overwritten.
     * If the file does not exist is created.
     * Returns true if the text is written succesfully, otherwise returns false.
     * @param path File where will write the text.
     * @param text Text to be wroten on the file.
     * @return bool
     */
    static bool writeFile(const QString &path, const QString &text);

    /**
     * @brief Read completely the file received by parameter.
     *
     * Returns a QByteArray object with the data of the file or an empty QByteArray if something wrong.
     * @param path File to be read.
     * @return Data read from the file.
     */
    static QByteArray readAllFile(const QString &path);

    /**
     * @brief Executes binary program.
     *
     * Returns the code error returned by the execution.
     *
     * If an empty standard output file is received, standard output is not saved.
     *
     * If an empty working directory is received, the process is executed in current directory.
     *
     * A timeout equals to -1 indicates that process has not limit time.
     *
     * Some known code errors:
     * -> 255: binary file not found or can't be executed.
     * -> -1: binary file crashed during the execution.
     * @param program Name of binary file.
     * @param arguments Arguments of the program.
     * @param standardOutput File path where process standard output will be saved.
     * @param workDirectory Directory where the program must work.
     * @param timeout Execution timeout of the program.
     * @return Code error of the execution
     */
    static int executeProgram(const QString &program,
        const QStringList &arguments = QStringList(),
        const QString &standardOutput = QProcess::nullDevice(),
        const QString &workingDirectory = QString(),
        const int timeout = -1
    );

    /**
     * @brief Compresses all files and folders in the directory received by argument.
     *
     * If compressing was succesfull, returns the absolute path of the zip file. Otherwise returns an empty string.
     * @param directory Path of directory to be compressed.
     * @param zipDestination Path where de zip file will be saved.
     * @param zipName Name to be setted to the file.
     * @return Absolute path of the result zip file.
     */
    static QString zipCompressDirectoryContents(
        const QString &directory,
        const QString &zipDestination,
        const QString &zipName = "compressed"
    );

    /**
     * @brief Simulates a progress on a QProgressDialog.
     *
     * Counter only finish when hasFinished is true, otherwise progress count stop in 99%.
     * As the counter is created in other thread, out of function you have to make sure that thread finished calling
     * function QFuture::waitForFinished(). If you don't make sure of it, your program probably will finish unexpectedly
     * with an error.
     *
     * Returns a QFuture object for control the end of the thread.
     * @param dialog Dialog where exec the counter.
     * @param hasFinished Flag to finish the counter.
     * @param timeout Time between each increase in the percentage of progress.
     * @return QFuture object to know when the thread has finished.
     */
    static QFuture<void> startProgressDialogCounter(QProgressDialog *dialog, bool *hasFinished, int timeout = 40);

    /**
     * @brief Returns true if is a valid ip, otherwise returns false.
     * @param ip Ip to be checked.
     * @return bool
     */
    static bool isValidIp(const QString &ip);

    /**
     * @brief Returns a current date time with the format received by parameter.
     * @param format Format to returns the date time.
     * @return Date time.
     */
    static QString getDateTime(QString format = "yyyy_MM_dd_hh_mm_ss");
};

#endif // UTILS_H
