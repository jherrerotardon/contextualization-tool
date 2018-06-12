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
    enum ModelError { OkModel = 0, NoImage, ImageNotExist, NoStrings };
    enum Error { NoError = 0, NullPointer, StringAlreadyExists };
    enum FindType { ByID = 0, ByValue };

    explicit ContextualizationControllerBase(QObject *parent = Q_NULLPTR);
    ~ContextualizationControllerBase();

protected:
    ContextualizationModel *model;
    QString fpFile;
    QString username;
    QStringList validStates;
    QString sendingHost;

    int importProjectFromJsonFile(QString path);
    int validateModel();
    QString generateContextualization();
    int sendContextualization(QString const &path, QString user, QString password);

    /**
     * @brief Extracts the strings contained in the image set in the model.
     *
     * Returns a pointer to QListString that contains all of strings extracted. Each string on QStringList is a line
     * found in the image.
     * @return QStringList *
     */
    QStringList * detectStringsOnImage();

    /**
     * @brief Processes received strings.
     *
     * Tries to find the strings value in fp file and strings that are in the file are added in the model.
     * Returns the number of strings added in the model.
     * @param strings QList<FirmwareString *> to be processed.
     * @return int
     */
    int processStrings(const QStringList &strings);
    QList<FirmwareString *> findString(const QString &text, const FindType findType = ByID);

    /**
     * @brief Converts a line of fp file in a FirmwareString if is possible.
     *
     * Return null if there is a format error in the line.
     * @param line QString Contains string to fragment.
     * @param lineNumber int Contains the number of line on the file.
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
     * The FirmwareString is added on the model of the tool. Also strings TableView model is notified because is
     * necessary to refresh the view of TableView.
     * If the parameter fwString is null, nothing is done.
     * Return true if FirmwareString is added successfully and false if not.
     * @param fwString FirmwareString * The string to add on the model.
     * @return bool
     */
    int addString(FirmwareString *fwString);

    int addStrings(const QList<FirmwareString *> &strings);

    bool removeString(int row);

    void removeAllStrings();

    /**
     * @brief Starts a process that allow user capture an area of the screen.
     *
     * The user have to select the area to capture with the mouse.
     * Return the path where the capture is stored or an empty QString if an error ocurred.
     * @return QString
     */
    QString takeCaptureArea();

    virtual bool setImage(const QString &image);

    /**
     * @brief Checks that the FirmwareString is not already in the model.
     *
     * If the FirmwareString has no empty id, checks that there is not any FirmwareString with the same id. If the
     * FirmwareString id is empty, then checks that there is not any FirmwareString with the same value.
     * @param fwString
     * @return bool
     */
    bool isFwStringAlreadyExists(FirmwareString &fwString);

    int eraseExistStrings(QList<FirmwareString *> *strings);

private:
    QList<FirmwareString *> findStringById(const QString &id);
    QList<FirmwareString *> findStringByValue(const QString &value);
};

#endif // CONTEXTUALIZATIONCONTROLLER_H
