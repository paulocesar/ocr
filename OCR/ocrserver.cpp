#include "ocrserver.h"
#include <QtNetwork>
#include <iostream>
#include "ocrserverhandler.h"
using namespace std;

OCRServer::OCRServer()
{
    _tcpServer = new QTcpServer(this);
    if(!_tcpServer->listen(QHostAddress::Any,6666))
        cout << "Unable to start the server." <<endl;
    connect(_tcpServer,SIGNAL(newConnection()),this,SLOT(getConnection()));
}

void OCRServer::getConnection()
{
    _clientConnection = _tcpServer->nextPendingConnection();
    new OCRServerHandler(_clientConnection);
}
