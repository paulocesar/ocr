#include "ocrserverhandler.h"
#include <QtNetwork>
#include <QStringList>
#include "ocrlog.h"
#include "ocr.h"

OCRServerHandler::OCRServerHandler(QTcpSocket *sock)
{
    _clientSocket = sock;
    connect(_clientSocket,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(_clientSocket, SIGNAL(disconnected()),_clientSocket,SLOT(deleteLater()));
}

void OCRServerHandler::receiveData()
{
    QDataStream ds(_clientSocket);
    QString data;

    ds >> data;
    ds << this->interpret(data);

//    _clientSocket->waitForBytesWritten(1000);
//    _clientSocket->disconnectFromHost();
//    _clientSocket->close();
//    this->~OCRServerHandler();
    if(data == "quit") {
        _clientSocket->waitForBytesWritten(1000);
        _clientSocket->disconnectFromHost();
        delete this;
    }
}

QString OCRServerHandler::interpret(QString &command) {

    //check if is a valid command
    if(!(QRegExp("^(add\\||remove\\||page\\||info\\|)[\\w\\d./:+-_\\|]*$").indexIn(command)!= -1))
        return "fail";

    QStringList cmd = command.split("|");

    if(cmd[0] == "add")
    {
        OCRLog::put("adding document...");
    }

    else if (cmd[0] == "remove")
    {

    }

    else if (cmd[0] == "page")
    {

    }

    else if (cmd[0] == "info")
    {

    }

    return "ok";
}
