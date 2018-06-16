#ifndef GUICONTEXTUALIZATIONCONTROLLER_H
#define GUICONTEXTUALIZATIONCONTROLLER_H

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QQuickWindow>

#include "base/contextualizationcontrollerbase.h"
#include "stringstablemodel.h"

class GuiContextualizationController : public ContextualizationControllerBase
{
    Q_OBJECT

    Q_PROPERTY(QString image READ getImageOfModel NOTIFY imageChanged)
    Q_PROPERTY(QList<QObject *> tableModel READ getTableModel NOTIFY stringsListChanged)
    Q_PROPERTY(QQuickWindow *view READ getView WRITE setView NOTIFY viewChanged)

public:
    GuiContextualizationController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);
    ~GuiContextualizationController();

public slots:
    void add(QString newString, int findType);
    void remove(QString stringId);
    void clear();
    void capture();
    void load();
    void detect();
    void send();
    void cancel();
    void save();
    void saveAs();
    void open();

private:
    QQuickWindow *view_;

    void connectSignalsAndSlots();
    QQuickWindow *getView();
    void setView(QQuickWindow *view);

signals:
    void viewChanged();
};

#endif // GUICONTEXTUALIZATIONCONTROLLER_H
