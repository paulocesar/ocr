#include "ocrdocument.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QDebug>
#include "ocrlog.h"
#include "ocrexception.h"


QMap<QString,OCRDocument *> OCRDocument::_sDocuments;
QList<OCRProcessor *> OCRDocument::_sProcessors;
bool OCRDocument::_sStarted = false;
int OCRDocument::_sId = 0;
QTimer OCRDocument::_sTimer;

/**
 * !!!! _sSingleton HAVE TO be called after OCRDocument::_sTimer
 */
OCRTimerResponse OCRTimerResponse::_sSingleton;

QString OCRDocument::PATH_FILES = "";

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

int OCRDocument::getNumberOfPages()
{
    return _numberOfPages;
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

int OCRDocument::getNextUnprocessedPage()
{
    if(_numberOfPages > _pagesProcessed.length())
        return _pagesProcessed.length();
    return -1;
}


//======================DOWNLOAD=FILE========================


void OCRDocument::downloaded(QNetworkReply* data)
{
    //http://www.tcpdf.org/examples/example_001.pdf

    QString path(OCRDocument::PATH_FILES + getFilename());
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    const QByteArray sdata = data->readAll();
    file.write(sdata);
    file.close();

    this->setStatus(OCRDocument::STATUS_FREE);
}

void OCRDocument::downloadProgress(qint64 received, qint64 total)
{
    int f = (float) received/total * 100;
    OCRLog::put(getId() + " download: " + QString::number(f)+"%");
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

QString OCRDocument::getPathAndFilename()
{
    return OCRDocument::PATH_FILES + getFilename();
}

QString OCRDocument::getPathAndPage(int page)
{
    return OCRDocument::PATH_FILES + "page_" + QString::number(page) + "_" + getId() + ".png";
}

QString OCRDocument::getPathAndTextTesseract(int page)
{
    return OCRDocument::PATH_FILES + "text_" + QString::number(page) + "_" + getId();
}

QString OCRDocument::getPathAndText(int page)
{
    return OCRDocument::PATH_FILES + "text_" + QString::number(page) + "_" + getId() + ".txt";
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

void OCRDocument::startup(int limitProcessors, int updateTime)
{
    if(_sStarted)
    {
        OCRLog::put("OCR is already started");
        return ;
    }

    if(PATH_FILES == "")
        PATH_FILES = QCoreApplication::applicationDirPath() + "/files/";

    if(limitProcessors < 1)
        throw new OCRException("OCR needs 1 processor at least");

    QDir(OCRDocument::PATH_FILES).removeRecursively();
    QDir().mkdir(OCRDocument::PATH_FILES);

    OCRLog::put("Starting OCR instance...");
    for(int i = 0; i < limitProcessors; i++)
    {
        _sProcessors.push_back(new OCRProcessor);
    }

    _sStarted = true;
    _sTimer.start(updateTime);
}

void OCRDocument::update()
{
    if(!_sStarted)
    {
        throw new OCRException("OCR is not started");
        return ;
    }

    OCRLog::put("updating data...");
    foreach(OCRProcessor *processor, _sProcessors)
    {
        if(processor->isFree())
        {
            OCRDocument *document = getNextFreeDocument();
            if(document)
                processor->process(document);
        }
    }
}


OCRDocument* OCRDocument::getNextFreeDocument()
{
    foreach(OCRDocument *document, _sDocuments)
    {
        if(document->isFree())
            return document;
    }
    return 0;
}

void OCRDocument::stop()
{
    if(!_sStarted)
    {
        OCRLog::put("OCR is already stopped");
        return ;
    }

    _sTimer.stop();

    foreach(OCRProcessor *processor, _sProcessors)
    {
        processor->waitForFinished(-1);
        delete processor;
    }
    _sProcessors.clear();

    _sStarted = false;
}


QString OCRDocument::documentAdd(QString url)
{
    //create documentId
    IT_DOC i;
    QString documentId = "";
    do
    {
        documentId = "asjdas8"+QString::number(++_sId);
        documentId = QString(QCryptographicHash::hash(documentId.toStdString().c_str(),QCryptographicHash::Md5).toHex());
        i = _sDocuments.find(documentId);
    }
    while(i != _sDocuments.end());

    //create document and download it
    OCRDocument *newDoc = new OCRDocument(documentId,url);
    _sDocuments.insert(documentId,newDoc);

    //return documentId to client
    return documentId;
}

bool OCRDocument::documentRemove(QString documentId)
{
    IT_DOC i = _sDocuments.find(documentId);
    if(i != _sDocuments.end())
    {
        delete i.value();
        _sDocuments.erase(i);
    }

    return false;
}

QString OCRDocument::documentPage(QString documentId, int page)
{
    IT_DOC i = _sDocuments.find(documentId);
    if(i != _sDocuments.end())
        return i.value()->getPage(page);
    return "";
}

QString OCRDocument::documentInfo(QString documentId)
{
    IT_DOC i = _sDocuments.find(documentId);
    if(i != _sDocuments.end())
        return i.value()->getInfo();
    return "";
}
