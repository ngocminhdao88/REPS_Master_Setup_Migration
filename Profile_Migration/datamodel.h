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
    } DataSet_t;

    DataModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;


private:
    QVector<DataSet_t> m_data;
};

#endif // DATAMODEL_H
