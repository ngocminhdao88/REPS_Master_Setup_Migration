#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QMouseEvent>

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void rightMouseClicked(const QModelIndex &index);
};

#endif // TABLEVIEW_H
