#ifndef STRINGSTABLEMODEL_H
#define STRINGSTABLEMODEL_H

#include "firmwarestring.h"
#include <QAbstractTableModel>
#include <QList>

enum Role {
    CheckboxsColumn=Qt::UserRole,
    StringKey,
    String,
    ButtonsColumn
};

class StringsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    StringsTableModel();
    ~StringsTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertRow(int row, FirmwareString *newString, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QList<FirmwareString *> strings;

};

#endif // STRINGSTABLEMODEL_H
