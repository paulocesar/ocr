#include "ocrserverhandler.h"
#include <QtNetwork>

OCRServerHandler::OCRServerHandler(QTcpSocket *sock)
{
    _clientSocket = sock;
    connect(_clientSocket,SIGNAL(readyRead()),this,SLOT(process()));
    connect(_clientSocket, SIGNAL(disconnected()),_clientSocket,SLOT(deleteLater()));
}

void OCRServerHandler::process() {
    QDataStream ds(_clientSocket);
    QString cmd;
    ds >> cmd;
    qDebug() << cmd;
    ds << cmd;
    _clientSocket->waitForBytesWritten(1000);
    _clientSocket->disconnectFromHost();
//    _clientSocket->close();
}