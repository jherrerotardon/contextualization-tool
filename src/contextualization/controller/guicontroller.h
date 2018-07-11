#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QInputDialog>
#include <QFileDialog>
#include <QApplication>
#include <QQuickWindow>
#include "contextualization/controller/contextualizationcontroller.h"

class GuiController : public ContextualizationController
{
    Q_OBJECT

    Q_PROPERTY(QString image READ getImageOfModel NOTIFY imageChanged)
    Q_PROPERTY(QList<QObject *> tableModel READ getTableModel NOTIFY stringsListChanged)
    Q_PROPERTY(QQuickWindow *view READ getView WRITE setView NOTIFY viewChanged)
    Q_PROPERTY(bool onlyDoneStrings MEMBER onlyDoneStrings_)
    Q_PROPERTY(bool caseSensitive MEMBER caseSensitive_)

public:
    GuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);
    ~GuiController();

private slots:
    void add(QString newString, int findType);
    void remove(QString stringId);
    void clear();
    void capture(bool detectStringsOnLoad);
    void load(bool detectStringsOnLoad);
    void detect();
    void send();
    void cancel();
    void save();
    void saveAs();
    void open();
    void configFpFile();
    void configRemoteHost();
    void configValidStates();
    void refresh();

private:
    QQuickWindow *view_;

    void connectSignalsAndSlots();
    QQuickWindow *getView();
    QString requestUsername();
    QString requestPassword();
    void setView(QQuickWindow *view);

signals:
    void viewChanged();
};

#endif // GUICONTROLLER_H
