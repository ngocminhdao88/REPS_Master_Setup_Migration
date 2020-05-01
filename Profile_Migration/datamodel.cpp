#include "datamodel.h"
#include <Qt>
#include <QColor>

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

int DataModel::rowCount(const QModelIndex & /*parent*/) const {

    return m_data.size();
}

int DataModel::columnCount(const QModelIndex & /*parent*/) const {
    return 4;
}

QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
        if (col == 0) return m_data.at(row).description;
        if (col == 1) return m_data.at(row).extension;
        if (col == 2) return m_data.at(row).path;
        if (col == 3) return m_data.at(row).status;
        break;

    case Qt::BackgroundRole:
        int status = m_data.at(row).status;

        /* Red - Green - Blue - Alpha*/
        if (status == 0) return QColor(0, 255, 0, 128); //light green
        if (status == 1) return QColor(255, 255, 0, 128); //light yellow
        if (status == 2) return QColor(255, 0, 0, 128); //light red
        break;
    }

    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Description");
        case 1:
            return QString("Extension");
        case 2:
            return QString("Path");
        case 3:
            return QString("Status");
        }
    }

    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        int row = index.row();
        int col = index.row();

        DataSet_t &item = m_data[row];

        switch (col) {
        case 0:
            item.description = value.toString();
        case 1:
            item.extension = value.toString();
        case 2:
            item.path = value.toString();
        case 3:
            item.status = value.toInt();
        }

        return true;
    }
    return false;
}
