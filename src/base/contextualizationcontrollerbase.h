#ifndef CONTEXTUALIZATIONCONTROLLER_H
#define CONTEXTUALIZATIONCONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QWindow>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>
#include <QFile>
#include <QFileDialog>
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

    explicit ContextualizationControllerBase(QObject *parent = nullptr);
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
    FirmwareString * findString(const QString &text);

    /**
     * @brief Convert a line of fp file in a FirmwareString if is possible.
     *
     * Return null if there is a format error in the line.
     * @param line QString Contains string to fragment.
     * @param lineNumber int Contains the number of line on the file.
     * @return FirmwareString *|null
     */
    FirmwareString * fragmentFpLine(QString &line, int lineNumber);

    /**
     * @brief Check the parameter state is a valid state.
     *
     * Valid states are stored in a private QStringList validStates.
     * Return true if the state is a valid state and return false if not.
     * @param state QString State to check.
     * @return bool
     */
    bool isValidState(QString &state);

    /**
     * @brief Add new string on the model.
     *
     * The FirmwareString is added on the model of the tool. Also strings TableView model is notified because is
     * necessary to refresh the view of TableView.
     * If the parameter fwString is null, nothing is done.
     * Return true if FirmwareString is added successfully and false if not.
     * @param fwString FirmwareString * The string to add on the model.
     * @return bool
     */
    virtual int addString(FirmwareString *&fwString);

    bool removeString(int row);

    void removeAllStrings();

    /**
     * @brief Start a process that allow user capture an area of the screen.
     *
     * The user have to select the area to capture with the mouse.
     * Return the path where the capture is stored or an empty QString if an error ocurred.
     * @return QString
     */
    QString takeCaptureArea();

    virtual bool setImage(const QString &image);

    /**
     * @brief Check that the FirmwareString is not already in the model.
     *
     * If the FirmwareString has no empty id, checks that there is not any FirmwareString with the same id. If the
     * FirmwareString id is empty, then checks that there is not any FirmwareString with the same value.
     * @param fwString
     * @return bool
     */
    bool isFpStringAlreadyExists(FirmwareString &fwString);

signals:

};

#endif // CONTEXTUALIZATIONCONTROLLER_H
