#ifndef GUICONTEXTUALIZATIONCONTROLLER_H
#define GUICONTEXTUALIZATIONCONTROLLER_H

#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

#include "base/contextualizationcontrollerbase.h"
#include "stringstablemodel.h"

class GuiContextualizationController : public QObject, public ContextualizationControllerBase
{
    Q_OBJECT
    //Q_PROPERTY(StringsTableModel * tableModel READ getTableModel WRITE setTableModel)
    //Q_PROPERTY(StringsTableModel * tableModel MEMBER tableModel)

public:
    GuiContextualizationController(QObject *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);
    ~GuiContextualizationController();

    StringsTableModel *getTableModel();
    void setTableModel(StringsTableModel *tableModel);

public slots:
    void add(QString newString, int findType);
    void remove(int row);
    void clear();
    void capture();
    void load();
    void detect();
    void send();
    void cancel();
    void save();
    void saveAs();
    void open();

    /**
     * @brief Refresh completly the view with the most recently data in model.
     */
    void refreshView();

    /**
     * @brief Refresh only the image displayed in the view with the most recently data in model.
     */
    void refreshImageView();

    /**
     * @brief Refresh only the strings table in the view with the most recently data in model.
     */
    void refreshTableView();

private:
    StringsTableModel *tableModel_;
    QObject *view_;
};

#endif // GUICONTEXTUALIZATIONCONTROLLER_H
