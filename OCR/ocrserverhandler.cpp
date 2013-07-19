#include "ocrserverhandler.h"
#include <QtNetwork>
#include <QStringList>
#include "ocrlog.h"
#include "ocrdocument.h"

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
}

QString OCRServerHandler::interpret(QString &command) {

    //check if is a valid command
    if(!(QRegExp("^(add\\||remove\\||page\\||info\\||quit\\|)[\\w\\d./:+-_\\|]*$").indexIn(command)!= -1))
        return "fail";

    QStringList cmd = command.split("|");

    if(cmd[0] == "add")
    {
        OCRLog::put("adding document...");
        return OCRDocument::documentAdd(cmd[1]);
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

    else if (cmd[0] == "quit")
    {
        _clientSocket->waitForBytesWritten(1000);
        _clientSocket->disconnectFromHost();
        _clientSocket->close();
        delete this;
    }

    return "ok";
}
