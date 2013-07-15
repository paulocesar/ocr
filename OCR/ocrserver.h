#ifndef OCRSERVER_H
#define OCRSERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;

class OCRServer : public QObject
{
    Q_OBJECT
public:
    OCRServer();

public slots:
    void getConnection();

private:
    QTcpSocket *_clientConnection;
    QTcpServer *_tcpServer;
};

#endif // OCRSERVER_H
