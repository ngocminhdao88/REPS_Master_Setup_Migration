#include "tcpclient.h"
#include <QHostAddress>
#include <QDebug>

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    in.setDevice(tcpSocket);

    signalSlotSetup();
}

void TCPClient::connectToHost() {
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress::LocalHost, 6340);
}

void TCPClient::displayError(QAbstractSocket::SocketError socketError) {
    QString errorString;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        errorString = "The host was not found. Please check the host name and port setting";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorString = "The coonection was refused by the peer";
        break;
    default:
        errorString = "The following error occured: " + tcpSocket->errorString();
    }

    if (!errorString.isEmpty())
        emit errorMessage(errorString);
}

void TCPClient::onReadyRead() {
    QByteArray nextData;

    in.startTransaction();
    in >> nextData;

    if (!in.commitTransaction())
        return;

    emit replyReceived(nextData);
}

void TCPClient::signalSlotSetup() {
    connect(tcpSocket, &QIODevice::readyRead, this, &TCPClient::onReadyRead);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &TCPClient::displayError);
}

void TCPClient::sendRequest(const Request requestType, const QString data) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    quint32 size;

    size = 1 + data.toUtf8().size();

    out << size << (uint8_t)requestType;
    out.writeRawData(data.toUtf8().constData(), data.length());

//    qDebug() << block;

    tcpSocket->write(block);
}

