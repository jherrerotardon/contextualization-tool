#ifndef GUICONTEXTUALIZATIONCONTROLLER_H
#define GUICONTEXTUALIZATIONCONTROLLER_H

#include "base/contextualizationcontrollerbase.h"
#include "stringstablemodel.h"

class GuiContextualizationController : public QObject, public ContextualizationControllerBase
{
    Q_OBJECT
    //Q_PROPERTY(StringsTableModel * tableModel READ getTableModel WRITE setTableModel)
    Q_PROPERTY(StringsTableModel * tableModel MEMBER tableModel)

public:
    GuiContextualizationController(QObject *view = nullptr, QObject *parent = nullptr);
    ~GuiContextualizationController();

    StringsTableModel *getTableModel();
    void setTableModel(StringsTableModel *tableModel);

public slots:
    void add(QString newString);
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

private:
    StringsTableModel *tableModel;
    QObject *view;

    int addString(FirmwareString *&fwString) override;
    bool setImage(const QString &image) override;

};

#endif // GUICONTEXTUALIZATIONCONTROLLER_H
