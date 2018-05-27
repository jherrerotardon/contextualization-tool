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
#include "stringstablemodel.h"
#include "utils.h"
#include "log.h"
#include "ocr.h"

class ContextualizationController : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(StringsTableModel * tableModel READ getTableModel WRITE setTableModel)
    Q_PROPERTY(StringsTableModel * tableModel MEMBER tableModel)

public:
    explicit ContextualizationController(QObject *view = nullptr, QObject *parent = nullptr);
    ~ContextualizationController();

    StringsTableModel *getTableModel();
    void setTableModel(StringsTableModel *tableModel);

public slots:
    void addString(QString newString);
    void deleteString(int row);
    void clearTable();
    void loadCaptureArea();
    void loadImage();
    void detectStringsOnImage();
    void send();
    void cancel();

private:
    ContextualizationModel model;
    StringsTableModel *tableModel;
    QString fpFile;
    QString username;
    QStringList validStates;
    QObject *view;

    int validateModel();
    int generatePackage(QString const &path);
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
     * @param fwString FirmwareString * The string to add on the model.
     */
    void addNewString(FirmwareString *&fwString);

    /**
     * @brief Start a process that allow user capture an area of the screen.
     *
     * The user have to select the area to capture with the mouse.
     * Return the path where the capture is stored or an empty QString if an error ocurred.
     * @return QString
     */
    QString captureArea();
    void setImage(QString &imagePath);

signals:

};

#endif // CONTEXTUALIZATIONCONTROLLER_H
