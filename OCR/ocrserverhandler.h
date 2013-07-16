#ifndef OCRSERVERHANDLER_H
#define OCRSERVERHANDLER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class QTcpSocket;

class OCRServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit OCRServerHandler(QTcpSocket *);

private:
    void download(QString url);

signals:
    void done();

private slots:
    void interpret();
    void downloadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);

private:
    QTcpSocket *_clientSocket;
    QNetworkAccessManager _manager;

};

#endif // OCRSERVERHANDLER_H
