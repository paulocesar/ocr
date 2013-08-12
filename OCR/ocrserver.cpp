#include "ocrserver.h"
#include <QtNetwork>
#include <iostream>
#include "ocrserverhandler.h"
#include "ocrdocument.h"
using namespace std;

OCRServer::OCRServer()
{
    _tcpServer = new QTcpServer(this);
    int PORT = 6666;
    cout << "creating server on port " << PORT << endl;
    if(!_tcpServer->listen(QHostAddress::Any,PORT))
        cout << "Unable to start the server." <<endl;

    OCRDocument::startup();
    connect(_tcpServer,SIGNAL(newConnection()),this,SLOT(getConnection()));
}

void OCRServer::getConnection()
{
    _clientConnection = _tcpServer->nextPendingConnection();
    new OCRServerHandler(_clientConnection);
}
