#ifndef OCRSERVERHANDLER_H
#define OCRSERVERHANDLER_H

#include <QObject>

class QTcpSocket;

class OCRServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit OCRServerHandler(QTcpSocket *);

private slots:
    void interpret();

private:
    QTcpSocket *_clientSocket;
};

#endif // OCRSERVERHANDLER_H
