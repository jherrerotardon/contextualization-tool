#include "stringstablemodel.h"
#include <QDebug>
#include "log.h"

StringsTableModel::StringsTableModel()
{
//    QString id("Creada");
//    QString value("Value");
//    QString des("DES");
//    QString state("OK");
//    FirmwareString *a  = new FirmwareString(id, value, des, 50, state, true);
//    /*FirmwareString b ("SEGUNDO", "Adios", "AAA", 50, "OK", true);
//    FirmwareString c ("ULTIMO", "Perro", "AAA", 50, "OK", true);*/

//    strings << a << a;
//    qDebug() << strings.at(0)->getId();
}

StringsTableModel::~StringsTableModel()
{
    foreach (FirmwareString *s, strings) {
        delete s;
    }
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

QHash<int, QByteArray> StringsTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[CheckboxsColumn] = "checkboxColumn";
    roles[StringKey] = "stringkey";
    roles[String] = "string";
    roles[ButtonsColumn] = "ButtonsColumn";
    return roles;
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
        return false;;
    }
}

bool StringsTableModel::insertRow(int row, FirmwareString *newString, const QModelIndex &parent)
{
    /*
     * Deberia comprobar que la fila no es negativa, puntero a null, etc, etc
     * igual en deleteRows()
     */
    beginInsertRows(parent, row, row);

    strings.insert(row, newString);

    endInsertRows();

    return true;
}
/******************************************************************************/
//bool StringsTableModel::insertRows(int row, int count, QList &list, const QModelIndex &parent)
//{

//    QString id("Insert");
//    QString value("Insert");
//    QString des("Insert 1");
//    QString state("OK");
//    FirmwareString *a  = new FirmwareString(id, value, des, 50, state, true);

//    beginInsertRows(parent, row, row + count - 1);

//    //for (int i = row; i < count; i++)
//        strings.insert(0, a);
//        //contacts.insert(position, { QString(), QString() });

//    endInsertRows();
//    return true;
//}

bool StringsTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
        strings.removeAt(row);

    endRemoveRows();
    return true;
}

