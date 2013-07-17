#include "ocr.h"

#include <QDebug>
#include <QCryptographicHash>
#include <iostream>
#include "ocrdocument.h"
#include "ocrexception.h"
#include "ocrlog.h"

bool OCR::sStarted = false;
QList<OCR *> OCR::_sOcrs;
QMap<QString, OCRDocument *> OCR::_sDocuments;
QTimer OCR::_sTimer;
int OCR::_sId = 0;

OCR::OCR(QObject *parent) :
    QProcess(parent)
{
    _status = OCR::STATUS_IDLE;
    connect(this,SIGNAL(finished(int)),this,SLOT(checkProcess(int)));
}

bool OCR::isDocumentComplete() {
    QMap<QString, OCRDocument *>::iterator i;
    i = _sDocuments.find(getDocumentId());
    if(i != _sDocuments.end()) {
        OCRDocument * doc = _sDocuments[_documentId];
        return (doc->getStatus() == OCRDocument::STATUS_COMPLETE) ? true : false;
    }
    return true;
}

QString OCR::getDocumentId()
{
    return _documentId;
}

void OCR::process(QString documentId)
{
    _status = OCR::STATUS_PROCESSING;
    if(documentId != "")
        _documentId = documentId;

    QMap<QString, OCRDocument *>::const_iterator it = _sDocuments.find(_documentId);
    if(it == _sDocuments.end()) {
        _status = OCR::STATUS_IDLE;
        return;
    }

    //TODO: apply conversion and image processing
//    OCRDocument *doc = it.value();
//    if(doc->is(OCRDocument::STATUS_FREE)) {
//
//    }
}

void OCR::checkProcess(int exitCode)
{
    _status = OCR::STATUS_IDLE;
}

OCR::Status OCR::getStatus() {
    return _status;
}



void OCR::update()
{
    if(!sStarted) {
        throw new OCRException("OCR is not started");
        return;
    }
    OCRLog::put("updating ocrs");
    foreach(OCR *ocr, _sOcrs) {
        if(ocr->getStatus() != OCR::STATUS_IDLE)
            continue;
        if(ocr->isDocumentComplete())
            ocr->process(ocr->getFreeDocument());
        else
            ocr->process();
    }
}


QString OCR::getFreeDocument() {
    QMap<QString, OCRDocument *>::const_iterator i;
     for (i = _sDocuments.constBegin(); i != _sDocuments.constEnd(); ++i)
         if(i.value()->isFree())
             return i.key();
     return "";
}

void OCR::startup(int limitInstances, int updateTime)
{
    if(limitInstances < 1) {
        throw new OCRException("OCR needs at least 1 instance to start");
    }

    if(!_sOcrs.empty()) {
        OCRLog::put("stopping old OCR instances");
        OCR::stop();
    }

    OCRLog::put("starting OCR instances");
    for(int i = 0; i < limitInstances; i++)
        _sOcrs.push_back(new OCR());

    connect(&_sTimer,SIGNAL(timeout()), _sOcrs[0], SLOT(update()));
    _sTimer.start(updateTime);

    sStarted = true;
}

void OCR::stop()
{
    if(!sStarted) {
        OCRLog::put("OCR is not started");
        return;
    }

    _sTimer.stop();

    OCRLog::put("Waiting ocr instances stop");
    foreach(OCR *ocr, _sOcrs)
        ocr->waitForFinished();

    if(!_sOcrs.empty()) {
        foreach (OCR *ocr, _sOcrs) {
            delete ocr;
        }
        _sOcrs.clear();
    }

    OCRLog::put("Bye");
    sStarted = false;
}

QString OCR::documentAdd(QString url)
{
    //create documentId
    QMap<QString, OCRDocument *>::iterator i;
    QString documentId = "";
    do {
        documentId = "asjdas8"+QString::number(++_sId);
        documentId = QString(QCryptographicHash::hash(documentId.toStdString().c_str(),QCryptographicHash::Md5).toHex());
        i = _sDocuments.find(documentId);
    } while(i != _sDocuments.end());

    //create document and download it
    OCRDocument *newDoc = new OCRDocument(documentId,url);
    _sDocuments.insert(documentId,newDoc);

    //return documentId to client
    return documentId;
}

bool OCR::documentRemove(QString documentId)
{
    QMap<QString, OCRDocument *>::iterator i;
    i = _sDocuments.find(documentId);
    if(i != _sDocuments.end()) {
        delete i.value();
        _sDocuments.erase(i);
        return true;
    }
    return false;
}

QString OCR::documentGetPage(QString documentId, int page)
{
    QMap<QString, OCRDocument *>::iterator i;
    i = _sDocuments.find(documentId);
    if(i != _sDocuments.end())
        return i.value()->getPage(page);
    return "";
}

QString OCR::documentGetInfo(QString documentId)
{
    QMap<QString, OCRDocument *>::iterator i;
    i = _sDocuments.find(documentId);
    if(i != _sDocuments.end())
        return i.value()->getInfo();
    return "";
}
