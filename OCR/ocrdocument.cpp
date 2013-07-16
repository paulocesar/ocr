#include "ocrdocument.h"

#include "ocrlog.h"

OCRDocument::OCRDocument(QString filename)
{
    //TODO: create directory
    _dir = "";

    _filename = filename;
    _status = OCRDocument::STATUS_FREE;
    _numberOfPages = -1;
}

QString OCRDocument::getFilePath()
{
    return _dir + "/" + _filename;
}

QString OCRDocument::getDir()
{
    return _dir;
}

QString OCRDocument::getPage(int number)
{
    if(number < 0 || number >= _pagesProcessed.length())
        return "";
    return _pagesProcessed[number];
}

int OCRDocument::getLastPageReady()
{
    return _pagesProcessed.length() - 1;
}

QString OCRDocument::getNextUnprocessedPagePath()
{
    int number = this->getNextUnprocessedPage();
    if(number < 0)
        return "";
    return this->getDir() + "/" + QString::number(number);
}


QString OCRDocument::getInfo() {
    return "hello world";
}

int OCRDocument::getNextUnprocessedPage()
{
    if(_numberOfPages > _pagesProcessed.length())
        return _pagesProcessed.length();
    return -1;
}

int OCRDocument::getCompletePercent()
{
    return (int) (_pagesProcessed.length()/_numberOfPages)*100;
}

bool OCRDocument::is(OCRDocument::Status status) {
    return (_status == status) ? true : false;
}

OCRDocument::Status OCRDocument::getStatus()
{
    return _status;
}

void OCRDocument::updateStatus()
{
    //TODO: update conditions
    _status = OCRDocument::STATUS_PROCESSING;
}


void OCRDocument::setNextProcessedPage(QString content)
{
    if(_numberOfPages <= _pagesProcessed.length()) {
        QString log = "Cannot add more pages in this document: ";
        log += _filename;
        OCRLog::put(log);
        return;
    }

    _pagesProcessed.push_back(content);
}

bool OCRDocument::isFree() {
    return (getStatus()  == OCRDocument::STATUS_FREE) ? true : false;
}
