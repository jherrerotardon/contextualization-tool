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
     * Some known code errors:
     * -> 255: binary file not found or can't be executed.
     * -> -1: binary file crashed during the execution.
     * @param program Name of binary file.
     * @param arguments Arguments of the program.
     * @param workDirectory Directory where the program must work.
     * @param timeout Execution timeout of the program.
     * @return Code error of the execution
     */
    static int executeProgram(
        const QString &program,
        const QStringList &arguments = QStringList(),
        const QString &workDirectory = QString(),
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
};

#endif // UTILS_H
