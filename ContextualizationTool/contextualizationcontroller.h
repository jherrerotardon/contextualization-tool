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
    FirmwareString * fragmentFpLine(QString &line, int lineNumber);
    bool isValidState(QString &state);
    void addNewString(FirmwareString *&fwString);
    QString captureArea();
    void setImage(QString &imagePath);

signals:

};

#endif // CONTEXTUALIZATIONCONTROLLER_H
