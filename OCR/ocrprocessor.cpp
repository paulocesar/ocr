#include "ocrprocessor.h"

#include "ocrdocument.h"
#include "ocrlog.h"

#include <QFile>

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
    if(exitCode)
    {
        OCRLog::warning("processor returned a error: " + QString::number(exitCode));
        OCRLog::warning(QString(readAllStandardOutput()));
        OCRLog::warning(QString(readAllStandardError()));
        return false;
    }

    int page = _document->getNextUnprocessedPage();


    if(_document->is(OCRDocument::STATUS_FREE))
    {
        OCRLog::put("processor analizing...");
        _status = OCRProcessor::STATUS_PROCESSING;
        _document->setStatus(OCRDocument::STATUS_CONVERTING);

        int pages = 3;

        if(_document->getFormat() == "pdf")
        {
            /**
              * ERROR TODO: fix identify bug
              * SOLUTION: find 'Type/Page>>' inside pdf
              */
        }

        _document->setNumberOfPages(pages);
        emit finished(0);
    }

    else if(_document->is(OCRDocument::STATUS_CONVERTING))
    {
        OCRLog::put("processor converting...");
        QString cmd = cmdConvert(_document->getPathAndFilename()+"["+QString::number(page)+"]",_document->getPathAndPage(page));
        _document->setStatus(OCRDocument::STATUS_PROCESSING);
        start(cmd);
    }

    else if(_document->is(OCRDocument::STATUS_PROCESSING))
    {
        OCRLog::put("processor using ocr....");
        QString cmd = cmdTesseract(_document->getPathAndPage(page),_document->getPathAndTextTesseract(page));
        _document->setStatus(OCRDocument::STATUS_READING);
        start(cmd);
    }

    else if(_document->is(OCRDocument::STATUS_READING))
    {
        OCRLog::put("processor reading...");
        QFile file(_document->getPathAndText(page));
        file.open(QIODevice::ReadOnly);
        _document->setNextProcessedPage(QString(file.readAll()));

        if(_document->getNextUnprocessedPage() != -1)
            _document->setStatus(OCRDocument::STATUS_CONVERTING);
        else
            _document->setStatus(OCRDocument::STATUS_COMPLETE);

        emit finished(0);
    }

    else if(_document->is(OCRDocument::STATUS_COMPLETE))
    {
        OCRLog::put("processor finished...");
        _document = 0;
        _status = OCRProcessor::STATUS_IDLE;
    }

    return true;
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
    return "convert -density 400 " + orig + " " + dest;
}

QString OCRProcessor::cmdTesseract(QString orig,QString dest)
{
    return "tesseract " + orig + " " + dest;
}

QString OCRProcessor::cmdPDFNumberPages(QString orig)
{
    return "pdftk "+orig+" dump_data";
}
