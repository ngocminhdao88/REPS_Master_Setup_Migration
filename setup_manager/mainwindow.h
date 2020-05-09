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
    /*
     * A file dialog to open Master file.
     */
    void onOpenButtonClicked();

    /*
     * Save all the changes to the opened Master file.
     */
    void onSaveButtonClicked();

    /*
     * Save all the changes to another Master file.
     */
    void onSaveAsButtonClicked();

    /*
     * Change directory of all setup files to selected directory.
     */
    void onChangeDirectoryButtonClicked();

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
