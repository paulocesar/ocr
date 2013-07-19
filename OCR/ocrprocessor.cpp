#include "ocrprocessor.h"

#include "ocrdocument.h"
#include "ocrlog.h"

#include <QDebug>

OCRProcessor::OCRProcessor(QObject *parent) :
    QProcess(parent), _status(OCRProcessor::STATUS_IDLE)
{
    connect(this,SIGNAL(finished(int)),this,SLOT(updateState(int)));
    _interrupt = false;
}

bool OCRProcessor::is(OCRProcessor::Status status)
{
    return (status == _status) ? true : false;
}

bool OCRProcessor::process(OCRDocument *document)
{
    if(!document)
        return false;
    if(!is(OCRProcessor::STATUS_IDLE))
        return false;

    _document = document;

    return updateState();
}

bool OCRProcessor::isFree()
{
    return (is(OCRProcessor::STATUS_IDLE)) ? true : false;
}

bool OCRProcessor::updateState(int exitCode)
{
    if(exitCode) {
        OCRLog::warning("processor returned a error: " + QString::number(exitCode));
    }

    if(_document->is(OCRDocument::STATUS_FREE))
    {
        _document->setStatus(OCRDocument::STATUS_CONVERTING);
        _status = OCRProcessor::STATUS_CONVERTING;
        OCRLog::put("converting to image: " + _document->getId());
        convertToImage();
        return true;
    }

    if(_document->is(OCRDocument::STATUS_CONVERTING))
    {
        _document->setStatus(OCRDocument::STATUS_PROCESSING);
        _status = OCRProcessor::STATUS_PROCESSING;
        OCRLog::put("converting to text: " + _document->getId());
        convertToText();
        return true;
    }

    if(_document->is(OCRDocument::STATUS_PROCESSING))
    {
        _document->setStatus(OCRDocument::STATUS_READING);
        _status = OCRProcessor::STATUS_READING;
        OCRLog::put("reading from file: " + _document->getId());
        readFromFiles();
        return true;
    }

    if(_document->is(OCRDocument::STATUS_READING))
    {
        OCRLog::put("document is complete: " + _document->getId());
        _document->setStatus(OCRDocument::STATUS_COMPLETE);
        _status = OCRProcessor::STATUS_IDLE;
        _document = 0;
        return true;
    }

    return false;
}

void OCRProcessor::interrupt()
{
    if(!is(OCRProcessor::STATUS_IDLE))
        kill();
    _status = OCRProcessor::STATUS_IDLE;
    emit processInterrupted(this);
}

QString OCRProcessor::cmdConvert(QString orig,QString dest)
{
    return "convert -density 600 " + orig + " " + dest;
}

QString OCRProcessor::cmdTesseract(QString orig,QString dest)
{
    return "tesseract " + orig + " " + dest;
}

QString OCRProcessor::cmdIdentify(QString orig)
{
    return "identify -format %n " + orig;
}

void OCRProcessor::convertToImage()
{
    //images have only one page
    int pages = 1;

    /**
      * ERROR TODO: fix identify bug
      */

//    //get pdf number of pages
//    if(_document->getFormat() == "pdf")
//    {
//        QProcess p;
//        OCRLog::put(cmdIdentify(_document->getPathAndFilename()));
//        p.start(cmdIdentify(_document->getPathAndFilename()));
//        p.waitForFinished();
//        pages = p.readAllStandardOutput().toInt();

//        qDebug() << p.readAllStandardOutput();
//        qDebug() << p.readAllStandardError();
//    }

    OCRLog::put("pages: " + QString::number(pages) + " " + _document->getId());

    if(pages < 1) {
        interrupt();
        return ;
    }


    //inform document about his number of pages
    _document->setNumberOfPages(pages);

    //create the command to convert document to tesseract images
    QString cmd = cmdConvert(_document->getPathAndFilename(),_document->getPathAndPage());

    for(int i = 1; i < pages; i++)
    {
        cmd += "; " + cmdConvert(_document->getPathAndFilename(),_document->getPathAndPage(i));
    }

    //execute as background
    start(cmd);
}


void OCRProcessor::convertToText()
{
    QString cmd = cmdTesseract(_document->getPathAndPage(),_document->getPathAndText());
    for(int i = 1; i < _document->getNumberOfPages(); i++) {
        cmd += "; " + cmdTesseract(_document->getPathAndPage(i),_document->getPathAndText());
    }

    start(cmd);
}

void OCRProcessor::readFromFiles()
{
    /**
      * TODO: copy content from file
      */
}
