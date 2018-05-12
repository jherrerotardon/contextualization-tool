#ifndef CONTEXTUALIZATIONCONTROLLER_H
#define CONTEXTUALIZATIONCONTROLLER_H

#include <QObject>
#include <QApplication>
#include <QWindow>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QProcess>
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
    void captureArea();
    void loadImage();
    void detectStringsOnImage();
    void send();
    void cancel();

private:
    ContextualizationModel model;
    StringsTableModel *tableModel;
    QString fpFile;
    QString username;
    QObject *view;

    int validateModel();
    int generatePackage(QString const &path);
    FirmwareString * fragmentFpLine(QString &line, int lineNumber);

signals:

};

#endif // CONTEXTUALIZATIONCONTROLLER_H
