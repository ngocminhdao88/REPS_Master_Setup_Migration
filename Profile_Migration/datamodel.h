#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef struct {
        QString description;
        QString extension;
        QString path;
        int status;
    } SetupFile_t;

    DataModel(QObject *parent = nullptr);

    /* QT Model-View */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void setSetupFilePaths(const QString filePaths);

    /* Update the master file path */
    void setMasterFile(const QString masterFile);

private:
    const int COLUMNS_COUNT = 4;

    enum SetupFileColumns {
        Description,
        Extension,
        Path,
        Status,
    };

    enum FileStatus {
        FileExistAndSamePathAsMasterFile,
        FileExistNotSamePathAsMasterFile,
        FileNotExist
    };

    QVector<SetupFile_t> m_data;
    QString m_masterFile;

private:
    /*
     * Initiate data model
     * @brief: Populate Description, Extension and Status columns of data model
     */
    void initDataModel();

    /*
     * Check setup file status
     * @brief: Check to see if file exists or in the same path as master file
     */
    void checkFileStatus(SetupFile_t &setupFile);

    /*
     * Check all setup files status
     * @brief: Check to see if file exists or in the same path as master file
     */
    void checkAllFilesStatus();
};

#endif // DATAMODEL_H
