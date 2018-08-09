#ifndef CONTEXTUALIZATIONCONTROLLER_H
#define CONTEXTUALIZATIONCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QImage>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "contextualization/model/contextualizationmodel.h"
#include "tools/utils.h"
#include "tools/log.h"
#include "optical_character_recognition/tesseractocr.h"
#include "storage/fpfileconnector.h"

class ContextualizationController : public QObject
{
    Q_OBJECT

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
     *
     * WARNING!! Don't change the default values of enum. Binary program returns this number values.
     */
    enum CodeError {
        NoError = 0,                ///< Indicates that there aren't any error during the process.
        NullPointer,                ///< Indicates that a null pointer has been received.
        StringAlreadyExists,        ///< Indicates that the string to process already exists in the model.
        NoImportFile,               ///< Indicates that file to import can't ber readed.
        ImportFileFormat,           ///< Indicates that the file to import has not a correct format.
        FileNotExists,              ///< Indicates that the file to read doesn't exist.
        NoRemoteHost,               ///< Indicates that there is no host to send the file.
        NoValidIp,                  ///< Indicates that the IP to use is not valid.
        SshpassError = 254,         ///< Indicates that an error ocurred in sshpass process.
        SshError = 255,             ///< Indicates that an error ocurred in ssh process.
        WriteFile = -1,             ///< Indicates that the last try to write in disk wrong.
    };

    /**
     * @brief The MatchType enum
     *
     * Contains all match types by which you can search a string.
     */
    enum MatchType {
        ByID = 0,           ///< The match will be done taking strings with the same idetifier.
        ByValue,            ///< The match will be done taking strings with the same value.
        ByApproximateValue, ///< The match will be done taking strings with similar value to the one you are looking for.
    };

    /**
     * @brief Constructs a controller setting his member variables.
     *
     * Reads the configuration file and sets the values. If controller can't read the configuration file or hasn't
     * a correct formart, controller sets a default values on member vaiables.
     * @param parent
     */
    ContextualizationController(QObject *parent = Q_NULLPTR);

    /**
     * @brief Destroys the controller.
     */
    ~ContextualizationController();

protected:
    const static int CHUNK_WIDTH;           ///< Width of each chunk when a image is splitted.
    const static int CHUNK_HEIGHT;          ///< Height of each chunk when a image is splitted.
    const static QString IMAGES_FOLDER;     ///< Directory where will save project images.
    const static QString PROJECTS_FOLDER;   ///< Directory where will save projects.
    ContextualizationModel *model_;         ///< Pointer to the contextualization model.
    QString englishFpFile_;                 ///< Original file where be all firmware strings.
    const static QString DONE_FP_FILE;      ///< File path where the firmware strings will be found.
    QString username_;                      ///< Username who run the app.
    QStringList validStates_;               ///< Valid states of firmware strings.
    QString remoteHost_;                    ///< Host where the contextualization will be sent.
    bool onlyDoneStrings_;                  ///< If is true, only string with state DONE will be found.
    bool caseSensitive_;                    ///< Indicates if searches will be case sensitive or not.

    /**
     * @brief Exports projet to json file.
     *
     * Decodes JSON format of the file received by parameter and try to convert it in a ContextualizationModel object.
     * Returns a #Error code.
     * @param path File path where be readed data project.
     * @return int
     */
    CodeError importProjectFromJsonFile(const QString &path);

    /**
     * @brief Imports projet from json file.
     *
     * Codes the actual model to JSON format and sabe data in the file received by parameter.
     * Returns true if project was exported succesfully, otherwise, returns false.
     * @param path File path where be saved data project.
     * @return bool
     */
    bool exportToJsonFile(const QString &path);

    /**
     * @brief Checks the actual state of the model.
     *
     * Returns the error code of the actual state of the model.
     * @return #ModelError
     */
    ContextualizationController::ModelError validateModel();

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
    CodeError sendContextualization(QString const &path, QString user, QString password);

    /**
     * @brief Extracts the strings contained in the image set in the model.
     *
     * Returns a QList of FirmwareString containing all of strings extracted converted in FirmwareString objects if are
     * in the fp file. Each firmware string on QList is a FirmwareString Object extracted from the image.
     * @param image Path of image where strings will be detected.
     * @return List of FirmwareString found on image.
     */
    QList<FirmwareString *> detectStringsOnImage(QString image);

    /**
     * @brief Processes received strings.
     *
     * Tries to find the strings value in fp file and strings that are in the file are returned converted in
     * FirmwareString object.
     * Returns QList of FirmwareString containing strings found in fp file..
     * @param strings String value to be processed.
     * @return List of FirmwareStirng found in fp file.
     */
    QList<FirmwareString *> processExtractedStrings(QStringList strings);

    /**
     * @brief Find the text received by parameter in fp file.
     *
     * There are diferrent find types. The second parameter indicates the member class of #FirmwareString that the text
     * received by parameter have to be compared.
     * Returns a firmware string list containing all strings that be found.
     * @param text Text to be found.
     * @param matchType Type of find to be done.
     * @return
     */
    QList<FirmwareString *> findString(const QString &text, const MatchType matchType = ByID);

    /**
     * @brief Checks the parameter state is a valid state.
     *
     * Valid states are stored in a private QStringList validstates.
     * Return true if the state is a valid state and return false if not.
     * @param state QString state to check.
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
     * @return Error code.
     */
    CodeError addString(FirmwareString *fwString);

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
     * @brief Removes the string in the model with the id received by parameter.
     *
     * Returns true if the string was removed succesfully or false if the string is not in the model.
     * @param row Row number of the string to be removed.
     * @return bool
     */
    bool removeString(QString stringId);

    /**
     * @brief Removes the string in the model row number received by parameter.
     *
     * Returns true if the string was removed succesfully or false if the string is not in the model.
     * @param row Row number of the string to be removed.
     * @return bool
     */
    bool removeString(int row);

    /**
     * @brief Remove all strings in the model.
     */
    bool removeAllStrings();

    /**
     * @brief Sets a no image in the model.
     */
    void clearImage();

    /**
     * @brief Starts a process that allow user capture an area of the screen.
     *
     * The user have to select the area to capture with the mouse.
     * Return the path where the capture is stored or an empty QString if an error ocurred.
     * @return QString
     */
    virtual QString takeCaptureArea() = 0;

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

    /**
     * @brief Returns the actual image of the model.
     * @return Absolute image path.
     */
    QString getImageOfModel();

    /**
     * @brief Returns the model to be used by ListView, TableView or similar QML object.
     * @return A model.
     */
    QList<QObject *> getTableModel();

    /**
     * @brief Returns a current date time with the format received by parameter.
     * @param format Format to returns the date time.
     * @return Date time.
     */
    QString getDateTime(QString format = "yyyy_MM_dd_hh_mm_ss");

    /**
     * @brief Reads the configuration file, if it exists, and sets the class members to the values in the file.
     */
    void loadConfig();

    /**
     * @brief Saves config in configuration file to can be recuperated in the next run of contetualization tool.
     */
    void saveConfig();

    /**
     * @brief Creates a copy of englishFp file in /tmp with only firmware strings with DONE status.
     *
     * If copy was created succesfully returns 0, otherwise returns the code error.
     * @return Code error
     */
    virtual int generateDoneFpFile();

    /**
     * @brief Filters a list of firmware strings. Remove from the list all strings that have not the same state as the
     * one received by parameter.
     *
     * Returns number of strings removed.
     * @param list Firmware strings list to be filtered.
     * @param state State that will be filtered.
     * @return Number of removed strings.
     */
    int filterStringsByState(QList<FirmwareString *> *list, const QString &state);

    /**
     * @brief Splits an image in one or more chunks depending of chunk size received by argument.
     *
     * If the width of the image is not divisible by the chunkWidth respectively, the last chunk of the row will
     * narrower than all the others. Works the same way for height of image and chunkHeight.
     * Chunks are saved on disk.
     *
     * Returns a list with path of image chunks. Only returns chunks that have been saved on disk succesfully.
     *
     * If ok is nonnull pointer, ok will be set to true if all chunks are saved on disk succesfully.
     * @param image Image to split.
     * @param chunkWidth Width of each part of image.
     * @param chunkHeight Height of each part of image.
     * @return QStringList with chunks of image.
     */
    QStringList splitImage(const QString &image, int chunkWidth, int chunkHeight, bool *someError = Q_NULLPTR);

protected slots:

    /**
     * @brief Tries to add a new string into the model
     *
     * This slot interacts with the user through the screen showing messages.
     * @param newString New strign to add.
     * @param matchType Mode in which the string is to be searched for
     */
    virtual void add(QString newString, int matchType) = 0;

    /**
     * @brief Tries to remove the string in the model with the identifier receiven by parameter.
     * @param stringId Idetifier for string to remove.
     */
    virtual void remove(QString stringId) = 0;

    /**
     * @brief Removes all strings in the model.
     */
    virtual void clear() = 0;

    /**
     * @brief Make a screen capture.
     * @param detectStringsOnLoad Flag to know if is neccesary detect strings on capture.
     */
    virtual void capture(bool detectStringsOnLoad) = 0;

    /**
     * @brief Loads an image in the model.
     * @param detectStringsOnLoad Flag to know if is neccesary detect strings on capture.
     */
    virtual void load(bool detectStringsOnLoad) = 0;

    /**
     * @brief Detects strings in the current image of the model and tries to add them into.
     */
    virtual void detect() = 0;

    /**
     * @brief Tries to send the active contextualization to a remote host.
     */
    virtual void send() = 0;

    /**
     * @brief Ensures that the user wants to cancel the project and closes the application in an orderly manner.
     */
    virtual void cancel() = 0;

    /**
     * @brief Saves current project.
     */
    virtual bool save() = 0;

    /**
     * @brief Opens a dialog and saves current project in the path specied for the user.
     */
    virtual bool saveAs() = 0;

    /**
     * @brief Opens a project saves on disk.
     */
    virtual void open() = 0;

    /**
     * @brief Refreshes all components on current contextualization (DoneFpFile, Image, Strings...).
     */
    virtual void refresh();

    /**
     * @brief Creates a new empty project.
     */
    virtual void newProject() = 0;

signals:

    /**
     * @brief The signal is emitted when a new string is added to the model or a string is removed from the model.
     */
    void stringsListChanged();

    /**
     * @brief The signal is emitted when a image is setted on the model.
     */
    void imageChanged();

private:
    const static QStringList COMMON_WORDS;             ///< Dicctionary with the most common words in firmware string.

    /**
     * @brief Checks if the word is a common word used in firmare strings.
     *
     * The common words are saved in a dictionary.
     * @param word Word to be checked.
     * @return bool
     */
    bool isCommonWord(const QString &word);
};

#endif // CONTEXTUALIZATIONCONTROLLER_H
