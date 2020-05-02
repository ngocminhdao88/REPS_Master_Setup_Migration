#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpclient.h"
#include "datamodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onOpenButtonClicked();
    void onSaveButtonClicked();
    void onSaveAsButtonClicked();
    void onAutoPopulateButtonClicked();

private:
    /* Setup SIGNAL-SLOT for buttons */
    void buttonSignalSlotSetup();

    /* Setup SIGNAL-SLOT */
    void signalSlotSetup();

private slots:
    /* Handle reply from server */
    void onReplyReceived(const QByteArray data);

    /* Display error message from TCP client */
    void errorMessage(QString errorString);

private:
    Ui::MainWindow *ui;
    TCPClient *tcpClient;
    DataModel *m_model;
};
#endif // MAINWINDOW_H
