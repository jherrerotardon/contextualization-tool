#ifndef CONTEXTUALIZATIONCONTROLLER_H
#define CONTEXTUALIZATIONCONTROLLER_H

#include <QWindow>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QFile>
#include <QStandardPaths>
#include <QDateTime>
#include <QDir>
#include "contextualizationmodel.h"
#include "tools/utils.h"
#include "tools/log.h"
#include "tools/ocr.h"

class ContextualizationControllerBase
{
public:

    /**
     * @brief The ModelError enum.
     *
     * Contains all errors that can has the model.
     */
    enum ModelError {
        OkModel = 0,    ///< Indicates that the model has not any error.
        NoImage,        ///< Indicates that the model has not setted a image.
        ImageNotExist,  ///< Indicates that the image setted in the model doesn't exist.
        NoStrings,      ///< Indicates that the model has not any image associated.
    };

    /**
     * @brief The CodeError enum
     *
     * Contains all general error that can happen during any process of the controller.
     */
    enum CodeError {
        NoError = 0,        ///< Indicates that there aren't any error during the process.
        NullPointer,        ///< Indicates that a null pointer has been received.
        StringAlreadyExists,///< Indicates that the string to process already exists in the model.
        NoImportFile,       ///< Indicates that file to import can't ber readed.
        ImportFileFormat,   ///< Indicates that the file to import has not a correct format.
        FileNotExists,      ///< Indicates that the file to read doesn't exist.
        WriteFile = -1,     ///< Indicates that the last try to write in disk wrong.
    };

    /**
     * @brief The FindType enum
     *
     * Contains all find types by which you can search a string.
     */
    enum FindType {
        ByID = 0,       ///< Indicates that the find will be by string identifier.
        ByValue,        ///< Indicates that the find will be by string value.
    };

    /**
     * @brief Constructs a controller setting his member variables.
     *
     * Reads the configuration file and sets the values. If controller can't read the configuration file or hasn't
     * a correct formart, controller sets a default values on member vaiables.
     * @param parent
     */
    ContextualizationControllerBase(QObject *parent = Q_NULLPTR);

    /**
     * @brief Destroys the controller.
     */
    ~ContextualizationControllerBase();

protected:
    ContextualizationModel *model_; ///< Pointer to the contextualization model.
    QString fpFile_;                ///< File path where find the firmware strings.
    QString username_;              ///< Username who run the app.
    QStringList validStates_;       ///< Valid states of firmware strings.
    QString sendingHost_;           ///< Host where the contextualization will be sent.

    /**
     * @brief Exports projet to json file.
     *
     * Decodes JSON format of the file received by parameter and try to convert it in a ContextualizationModel object.
     * Returns a #Error code.
     * @param path File path where be readed data project.
     * @return int
     */
    int importProjectFromJsonFile(const QString &path);

    /**
     * @brief Imports projet from json file.
     *
     * Codes the actual model to JSON format and sabe data in the file received by parameter.
     * Returns a #Error code.
     * @param path File path where be saved data project.
     * @return int
     */
    int exportToJsonFile(const QString &path);

    /**
     * @brief Checks the actual state of the model.
     *
     * Returns the error code of the actual state of the model.
     * @return #ModelError
     */
    ContextualizationControllerBase::ModelError validateModel();

    /**
     * @brief Generates a packet with the contextualization data.
     *
     * After verifying that the model contains no errors, it packages all the information in a zip file with the data
     * to be sent.
     * If there aren't errors, returns the path of the zip file, else returns an empty QString.
     * @return QString
     */
    QString generateContextualization();

    /**
     * @brief Process that is responsible for sending the file received by parameter.
     *
     * Using credentials received por parameters, sends by SFTP protocol the file. Return the code error returned by
     * SFTP process or a #CodeError If there were any errors during the process.
     * @param path File path to send.
     * @param user Username credential.
     * @param password Password credential.
     * @return
     */
    int sendContextualization(QString const &path, QString user, QString password);

    /**
     * @brief Extracts the strings contained in the image set in the model.
     *
     * Returns a pointer to QListString that contains all of strings extracted. Each string on QStringList is a line
     * found in the image. Returns null if there was any errors.
     * @return QStringList *
     */
    QStringList * detectStringsOnImage();

    /**
     * @brief Processes received strings.
     *
     * Tries to find the strings value in fp file and strings that are in the file are added in the model.
     * Returns the number of strings added in the model.
     * @param strings Firmware strings to be processed.
     * @return int
     */
    int processStrings(const QStringList &strings);

    /**
     * @brief Find the text received by parameter in fp file.
     *
     * There are diferrent find types. The second parameter indicates the member class of #FirmwareString that the text
     * received by parameter have to be compared.
     * Returns a firmware string list containing all strings that be found.
     * @param text Text to be found.
     * @param findType Type of find to be done.
     * @return
     */
    QList<FirmwareString *> findString(const QString &text, const FindType findType = ByID);

    /**
     * @brief Converts a line of fp file in a FirmwareString if is possible.
     *
     * Return null if there is a format error in the line.
     * @param line Contains string to fragment.
     * @param lineNumber Contains the number of line on the file.
     * @return FirmwareString *|null
     */
    FirmwareString * fragmentFpLine(QString &line, int lineNumber);

    /**
     * @brief Checks the parameter state is a valid state.
     *
     * Valid states are stored in a private QStringList validStates.
     * Return true if the state is a valid state and return false if not.
     * @param state QString State to check.
     * @return bool
     */
    bool isValidState(QString &state);

    /**
     * @brief Adds a new string on the model.
     *
     * The FirmwareString is added on the model of the tool if is not wrong or is already in the model.
     * If the parameter fwString is null, nothing is done.
     * Return true if FirmwareString is added successfully and false if not.
     * @param fwString The string to add on the model.
     * @return bool
     */
    int addString(FirmwareString *fwString);

    /**
     * @brief Adds new strings on the model.
     *
     * Append all strings in the list that are not wrong or are already in the model.
     * Returns the number of strings that have been added to the model.
     * @param strings Strings list to be added in the model.
     * @return
     */
    int addStrings(const QList<FirmwareString *> &strings);

    /**
     * @brief Remove the string in the model row number received by parameter.
     *
     * Returns true if the string was removed succesfully or false if the string is not in the model.
     * @param row Row number of the string to be removed.
     * @return bool
     */
    bool removeString(int row);

    /**
     * @brief Remove all strings in the model.
     */
    void removeAllStrings();

    /**
     * @brief Starts a process that allow user capture an area of the screen.
     *
     * The user have to select the area to capture with the mouse.
     * Return the path where the capture is stored or an empty QString if an error ocurred.
     * @return QString
     */
    QString takeCaptureArea();

    /**
     * @brief setImage
     * @param image Path of image that will be set to the model.
     * Returns true if the image is setted succesfully or returns false if the image can not setted.
     * @return bool
     */
    bool setImage(const QString &image);

    /**
     * @brief Checks that the FirmwareString is not already in the model.
     *
     * If the FirmwareString has no empty id, checks that there is not any FirmwareString with the same id. If the
     * FirmwareString id is empty, then checks that there is not any FirmwareString with the same value.
     * @param fwString
     * @return bool
     */
    bool isFwStringAlreadyExists(FirmwareString &fwString);

    /**
     * @brief Erases, from the string list received by parameter, strings that already are in the model.
     *
     * Return the number of strings erased from the strings list.
     * @param strings List of strings to be processed.
     * @return int
     */
    int eraseExistStrings(QList<FirmwareString *> *strings);

private:

    /**
     * @brief Finds in fp file the first string with the same ID.
     *
     * Returns a list with the #FirmwareString object found in fp file.
     * @param id ID to compare with the string ID from fp file.
     * @return QList<FirmwareString *>
     */
    QList<FirmwareString *> findStringById(const QString &id);

    /**
     * @brief Finds in fp file the strings with the same value.
     *
     * Returns a list with the #FirmwareString objects found in fp file.
     * @param value Value to compare with the string value from fp file.
     * @return QList<FirmwareString *>
     */
    QList<FirmwareString *> findStringByValue(const QString &value);
};

#endif // CONTEXTUALIZATIONCONTROLLER_H
