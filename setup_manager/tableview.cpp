#include "tableview.h"

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{

}

void TableView::mousePressEvent(QMouseEvent *event) {
    QPoint pos = event->pos();
    QPersistentModelIndex index = indexAt(pos);

    if (event->button() == Qt::RightButton)
        emit rightMouseClicked(index);
    else
        QTableView::mousePressEvent(event);
}
