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
    QString interpret(QString &command);

private slots:
    void receiveData();

private:
    QTcpSocket *_clientSocket;
    QNetworkAccessManager _manager;

};

#endif // OCRSERVERHANDLER_H
