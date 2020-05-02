#include "datamodel.h"
#include <Qt>
#include <QColor>
#include <QTextStream>
#include <QFile>
#include <QDebug>

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    initDataModel();
}

int DataModel::rowCount(const QModelIndex & /*parent*/) const {

    return m_data.size();
}

int DataModel::columnCount(const QModelIndex & /*parent*/) const {
    return COLUMNS_COUNT;
}

QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
        if (col == Description) return m_data.at(row).description;
        if (col == Extension) return m_data.at(row).extension;
        if (col == Path) return m_data.at(row).path;
        if (col == Status) return m_data.at(row).status;
        break;

    case Qt::BackgroundRole:
        int status = m_data.at(row).status;

        /* Red - Green - Blue - Alpha*/
        if (status == FileExistAndSamePathAsMasterFile) return QColor(0, 255, 0, 128); //light green
        if (status == FileExistNotSamePathAsMasterFile) return QColor(255, 255, 0, 128); //light yellow
        if (status == FileNotExist) return QColor(255, 0, 0, 128); //light red
        break;
    }

    return QVariant();
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case Description:
            return QString("Description");
        case Extension:
            return QString("Extension");
        case Path:
            return QString("Path");
        case Status:
            return QString("Status");
        }
    }

    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
    if (!checkIndex(index))
        return Qt::NoItemFlags;

    int col = index.column();

    if (col == Status) //status
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractTableModel::flags(index);
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        int row = index.row();
        int col = index.column();

        SetupFile_t &item = m_data[row];

        switch (col) {
        case Description:
            item.description = value.toString();
            break;
        case Extension:
            item.extension = value.toString();
            break;
        case Path:
            item.path = value.toString();
            break;
        case Status:
            item.status = value.toInt();
            break;
        }

        return true;
    }
    return false;
}

void DataModel::initDataModel() {
    QStringList tempList;
    SetupFile_t dataSet;
    QFile setupFile("setup_file_description.csv");

    if (setupFile.open(QFile::ReadOnly)) {
        QTextStream in(&setupFile);

        while (!in.atEnd()) {
            QString myString = in.readLine();
            tempList = myString.split(",");

            dataSet.description = tempList.at(Description);
            dataSet.extension = tempList.at(Extension);
            dataSet.path = tempList.at(Path);
            dataSet.status = tempList.at(Status).toInt();

            m_data.append(dataSet);
        }

        setupFile.close();

        QModelIndex topLeft = createIndex(0, 0);
        QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
        emit dataChanged(topLeft, bottomRight);
    }
    else {
        qDebug() << "Cannot open file";
    }
}

void DataModel::checkFileStatus(SetupFile_t &setupFile) {
}

void DataModel::checkAllFilesStatus() {

}

void DataModel::setSetupFilePaths(const QString filePaths) {
    QStringList tmpPaths;

    for (QString str : filePaths.split(',')) {
        tmpPaths.append(str.simplified());
    }

    for (int i = 0; i < m_data.size(); i++) {
        SetupFile_t &item = m_data[i];
        item.path = tmpPaths.at(i);
    }

    QModelIndex topLeft = createIndex(0, 2);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}

void DataModel::setMasterFile(const QString masterFile) {
    if (masterFile != m_masterFile)
        m_masterFile = masterFile;
}
