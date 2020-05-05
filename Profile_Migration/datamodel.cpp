#include "datamodel.h"
#include <Qt>
#include <QColor>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

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
        if (col == DescriptionColumn) return m_data.at(row).description;
        if (col == ExtensionColumn) return m_data.at(row).extension;
        if (col == PathColumn) return m_data.at(row).path;
        if (col == StatusColumn) return m_data.at(row).status;
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
        case DescriptionColumn:
            return QString("Description");
        case ExtensionColumn:
            return QString("Extension");
        case PathColumn:
            return QString("Path");
        case StatusColumn:
            return QString("Status");
        }
    }

    return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
    if (!checkIndex(index))
        return Qt::NoItemFlags;

//    int col = index.column();

//    if (col == StatusColumn) //status
//        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
//    else
//        return QAbstractTableModel::flags(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role == Qt::EditRole) {
        if (!checkIndex(index))
            return false;

        int row = index.row();
        int col = index.column();

        SetupFile_t &item = m_data[row];

        switch (col) {
        case DescriptionColumn:
            item.description = value.toString();
            break;
        case ExtensionColumn:
            item.extension = value.toString();
            break;
        case PathColumn:
            item.path = value.toString();
            break;
        case StatusColumn:
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

            dataSet.description = tempList.at(DescriptionColumn);
            dataSet.extension = tempList.at(ExtensionColumn);
            dataSet.path = tempList.at(PathColumn);
            dataSet.status = tempList.at(StatusColumn).toInt();

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
    QFileInfo masterFileInfo(m_masterFile);
    QDir masterDir = masterFileInfo.absoluteDir();

    QFileInfo setupFileInfo(setupFile.path);
    QDir setupDir = setupFileInfo.absoluteDir();

    if (!setupFileInfo.exists() || !setupFileInfo.isFile()) {
        setupFile.status = FileStatus::FileNotExist;
        return;
    }

    if (setupDir != masterDir) {
        setupFile.status = FileStatus::FileExistNotSamePathAsMasterFile;
        return;
    }

    setupFile.status = FileStatus::FileExistAndSamePathAsMasterFile;
}

void DataModel::checkAllFilesStatus() {
    for (SetupFile_t &setupFile : m_data) {
        checkFileStatus(setupFile);
    }

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}

void DataModel::changeDirectory(QString dir) {
    if (dir.isEmpty())
        return;

    for (SetupFile_t &setupFile : m_data) {
        if (setupFile.path.isEmpty())
            continue;

        QFileInfo fileInfo(setupFile.path);
        QString fileName = fileInfo.fileName();
        QDir dirPath(dir);
        QString newFilePath = dirPath.absoluteFilePath(fileName);

        setupFile.path = newFilePath;
        checkFileStatus(setupFile);
    }

    QModelIndex topLeft = createIndex(0, SetupFileColumns::PathColumn);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}

void DataModel::setSetupFilePaths(const QString filePaths) {
    QStringList tmpPaths;

    for (QString str : filePaths.split(',')) {
        tmpPaths.append(str.simplified());
    }

    for (int i = 0; i < m_data.size(); i++) {
        SetupFile_t &setupFile = m_data[i];
        setupFile.path = tmpPaths.at(i);

        checkFileStatus(setupFile);
    }

    QModelIndex topLeft = createIndex(0, PathColumn);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}

QString DataModel::getSetupFilePaths() {
    QStringList list;

    for (SetupFile_t setupFile : m_data) {
        list.append(setupFile.path);
    }

    return list.join(',');
}

void DataModel::setMasterFile(const QString masterFile) {
    if (masterFile != m_masterFile)
        m_masterFile = masterFile;
}

void DataModel::clearSetupFilePath(const QModelIndex &index) {
    if (!checkIndex(index))
        return;

    int row = index.row();

    SetupFile_t &setupFile = m_data[row];
    setupFile.path.clear();
    setupFile.status = FileNotExist;

    QModelIndex topLeft = createIndex(row, PathColumn);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}

void DataModel::selectSetupFile(const QModelIndex &index) {
    if (!checkIndex(index))
        return;

    int row = index.row();

    SetupFile_t &setupFile = m_data[row];
    const QString descWithNumber = setupFile.description;
    int tempIndex = descWithNumber.indexOf('-');
    const QString descWithoutNumber = descWithNumber.right(descWithNumber.size() - (tempIndex + 2));
    const QString extension = setupFile.extension;

    const QString path = QFileDialog::getOpenFileName(
                nullptr,
                "Open " + descWithoutNumber,
                "",
                extension
                );

    if (path.isEmpty())
        return;

    setupFile.path = path;

    checkFileStatus(setupFile);

    QModelIndex topLeft = createIndex(row, PathColumn);
    QModelIndex bottomRight = createIndex(m_data.size() - 1, COLUMNS_COUNT - 1);
    emit dataChanged(topLeft, bottomRight);
}
