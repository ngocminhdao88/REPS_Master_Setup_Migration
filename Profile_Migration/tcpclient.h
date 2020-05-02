#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

enum Request {
    SetMasterFileRequest = 10,
    ReadMasterFileRequest = 20,
    WriteMasterFileRequest = 30
};

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = nullptr);

    void connectToHost();
    void sendRequest(const Request requestType, const QString data);

signals:
    void errorMessage(const QString errorString);
    void replyReceived(const QByteArray data);

public slots:

private slots:
    /* Display error when this client connect to server */
    void displayError(QAbstractSocket::SocketError socketError);

    /* Handle incomming data from server */
    void onReadyRead();

    /* Setup SIGNAL-SLOT connection */
    void signalSlotSetup();

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
};

#endif // TCPCLIENT_H
