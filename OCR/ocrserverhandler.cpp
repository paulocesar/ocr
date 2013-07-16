#include "ocrserverhandler.h"
#include <QtNetwork>

OCRServerHandler::OCRServerHandler(QTcpSocket *sock)
{
    _clientSocket = sock;
    connect(_clientSocket,SIGNAL(readyRead()),this,SLOT(interpret()));
    connect(_clientSocket, SIGNAL(disconnected()),_clientSocket,SLOT(deleteLater()));
}


void OCRServerHandler::interpret()
{
    QDataStream ds(_clientSocket);
    QString cmd;
    ds >> cmd;
    qDebug() << cmd;
    ds << cmd;
    _clientSocket->waitForBytesWritten(1000);
    _clientSocket->disconnectFromHost();
//    _clientSocket->close();
//    this->~OCRServerHandler();
}

void OCRServerHandler::download(QString url)
{
    QUrl qurl = QUrl::fromEncoded(url.toLocal8Bit());
    QNetworkRequest request(qurl);
    QObject::connect(_manager.get(request),SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
}

void OCRServerHandler::downloadFinished(QNetworkReply* data)
{
    //TODO: set path and to downloaded files and no override
    QFile localFile("localfile");
    if (!localFile.open(QIODevice::WriteOnly))
        return;

    const QByteArray sdata = data->readAll();
    localFile.write(sdata);
    qDebug() << sdata;
    localFile.close();

    emit done();
}

void OCRServerHandler::downloadProgress(qint64 received, qint64 total)
{
    qDebug() << received << total;
}
