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
    QByteArray data;

    ds >> data;
    QString command(QByteArray::fromBase64(data));
    ds << QByteArray().append(this->interpret(command)).toBase64();
}

QString OCRServerHandler::interpret(QString &command)
{
    /**
      * TODO: fix command validation and split
      */
    //check if it is a valid command structure
    if(!(QRegExp("^(add\\||remove\\||page\\||info\\||quit\\|)[\\w\\d./:+-_\\|]*$").indexIn(command)!= -1))
        return "fail";

    QStringList cmd = command.split("|");

    if(cmd[0] == "add")
    {
        OCRLog::put("adding document: " + cmd[1]);
        QString code = OCRDocument::documentAdd(cmd[1]);
        OCRLog::put("document added: " + code + " " + cmd[1]);
        return code;
    }

    else if (cmd[0] == "remove")
    {
        OCRLog::put("removing document: " + cmd[1]);
        if(OCRDocument::documentRemove(cmd[1]))
            return "ok";
    }

    else if (cmd[0] == "page")
    {
        OCRLog::put("sending document page "+ cmd[2] +": " + cmd[1]);
        return OCRDocument::documentPage(cmd[1],cmd[2].toInt());
    }

    else if (cmd[0] == "info")
    {
        OCRLog::put("sending document info: " + cmd[1]);
        QString result = OCRDocument::documentInfo(cmd[1]);
        if(result != "")
            return result;
    }

    else if (cmd[0] == "quit")
    {
        OCRLog::put("closing socket handler");
        _clientSocket->waitForBytesWritten(1000);
        _clientSocket->disconnectFromHost();
        _clientSocket->close();
        delete this;
    }

    return "fail";
}
