#include "ocrdocument.h"

#include <QFile>
#include <QDebug>
#include "ocrlog.h"
#include "ocrexception.h"

QMap<QString,OCRDocument *> OCRDocument::_sDocuments;

OCRDocument::OCRDocument(QString documentId, QString url, QObject *parent) :
    QObject(parent)
{
    _documentId = documentId;
    _status = OCRDocument::STATUS_DOWNLOADING;
    _numberOfPages = -1;

    _url = url;

    //TODO: capture format
    _format = "pdf";

    QUrl qurl = QUrl::fromEncoded(getUrl().toLocal8Bit());
    QNetworkRequest request(qurl);
    connect(_manager.get(request),SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(downloadProgress(qint64,qint64)));
    connect(&_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(downloaded(QNetworkReply*)));
}

void OCRDocument::setNumberOfPages(int pages)
{
    if(pages > 0)
        _numberOfPages = pages;
    else
        throw new OCRException("cannot set empty number of pages");
}

void OCRDocument::setNextProcessedPage(QString content)
{
    if(this->is(OCRDocument::STATUS_COMPLETE))
    {
        QString log = "Cannot add more pages in this document: ";
        log += _documentId;
        OCRLog::put(log);
        return;
    }

    this->setStatus(OCRDocument::STATUS_PROCESSING);
    _pagesProcessed.push_back(content);

    if(_numberOfPages == _pagesProcessed.length())
        this->setStatus(OCRDocument::STATUS_COMPLETE);
}


//======================DOWNLOAD=FILE========================


void OCRDocument::downloaded(QNetworkReply* data)
{
    QString path(getFilename());
    QFile localFile(path);
//    if (!localFile.open(QIODevice::WriteOnly))
//        return;

    const QByteArray sdata = data->readAll();
    localFile.write(sdata);
    qDebug() << sdata;
    localFile.close();

    this->setStatus(OCRDocument::STATUS_FREE);
}

void OCRDocument::downloadProgress(qint64 received, qint64 total)
{
    qDebug() << received << total;
}



//===========DOCUMENT=GERENAL=INFORMATION===========

QString OCRDocument::getId()
{
    return _documentId;
}

QString OCRDocument::getUrl()
{
    return _url;
}

QString OCRDocument::getFilename()
{
    return getId() + "." + getFormat();
}


QString OCRDocument::getFormat()
{
    return _format;
}

QString OCRDocument::getPage(int number)
{
    if(number < 0 || number >= _pagesProcessed.length())
        return "";
    return _pagesProcessed[number];
}

QString OCRDocument::getInfo() {
    return "hello world";
}

int OCRDocument::getCompletePercent()
{
    return (int) (_pagesProcessed.length()/_numberOfPages)*100;
}

int OCRDocument::getLastPageReady()
{
    return _pagesProcessed.length() - 1;
}


//===================STATUS====================

bool OCRDocument::is(OCRDocument::Status status) {
    return (_status == status) ? true : false;
}

OCRDocument::Status OCRDocument::getStatus()
{
    return _status;
}

void OCRDocument::setStatus(OCRDocument::Status status)
{
    _status = status;
}

bool OCRDocument::isFree() {
    return (getStatus()  == OCRDocument::STATUS_FREE) ? true : false;
}


//============DOCUMENT=ACCESS=INTERFACE=========

void OCRDocument::startup(int limitOcrs)
{

}

void OCRDocument::update()
{

}

void OCRDocument::stop()
{

}


QString OCRDocument::documentAdd(QString filename)
{

}

bool OCRDocument::documentRemove(QString documentId)
{

}

QString OCRDocument::documentPage(QString documentId, int page)
{

}

QString OCRDocument::documentInfo(QString documentId)
{

}
