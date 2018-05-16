#include "stringstablemodel.h"
#include <QDebug>
#include "log.h"

StringsTableModel::StringsTableModel(QList<FirmwareString *> &newstrings) : strings(newstrings)
{

}

StringsTableModel::~StringsTableModel()
{
    foreach (FirmwareString *s, strings)
        delete s;
}

int StringsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return strings.size();
}

int StringsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant StringsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= strings.size() || index.row() < 0)
        return QVariant();

    switch (role) {
        case StringKey:
            return strings.value(index.row())->getId();
        case String:
            return strings.value(index.row())->getValue();
        default:
            return QVariant();
    }

}

bool StringsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= strings.size())
        return false;

    FirmwareString *string = strings.value(index.row());

    switch (role) {
        case String:
            string->setValue(value.toString());
            emit(dataChanged(index, index));
            return true;
        default:
            return false;
    }

}

Qt::ItemFlags StringsTableModel::flags(const QModelIndex &index) const
{
    qDebug() << "flags:  " << index.row() << " , " << index.column();

    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 2)
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;

    return QAbstractTableModel::flags(index);
}

bool StringsTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool StringsTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

QHash<int, QByteArray> StringsTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CheckboxsColumn] = "checkboxColumn";
    roles[StringKey] = "stringkey";
    roles[String] = "string";
    roles[ButtonsColumn] = "ButtonsColumn";
    return roles;
}

