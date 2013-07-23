#ifndef OCRDOCUMENT_H
#define OCRDOCUMENT_H

#include <QCoreApplication>
#include <QMap>
#include <QObject>
#include <QList>
#include <QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "ocrprocessor.h"

#define IT_DOC QMap<QString,OCRDocument *>::iterator
#define IT_PRO QList<OCRProcessor *>::iterator

class OCRTimerResponse;

class OCRDocument : public QObject
{
    Q_OBJECT
public:

    enum Status {
        STATUS_DOWNLOADING = 1,
        STATUS_FREE = 2,
        STATUS_CONVERTING = 4,
        STATUS_PROCESSING = 8,
        STATUS_READING = 16,
        STATUS_COMPLETE = 32
    };

    //status
    bool is(OCRDocument::Status status);
    bool isFree();
    OCRDocument::Status getStatus();
    void setStatus(OCRDocument::Status status);

    //document access interface
    static void startup(int limitProcessors = 3, int updateTime = 2000);
    static void stop();

    static QString documentAdd(QString url);
    static bool documentRemove(QString documentId);
    static QString documentPage(QString documentId, int page);
    static QString documentInfo(QString documentId);

    //processing page
    void setNumberOfPages(int pages);
    int getNumberOfPages();
    void setNextProcessedPage(QString content);
    int getNextUnprocessedPage();

    //general information
    QString getId();
    QString getUrl();
    QString getFilename();
    QString getPathAndFilename();
    QString getPathAndPage(int page = 0);
    QString getPathAndText(int page = 0);
    QString getPathAndTextTesseract(int page = 0);
    QString getFormat();
    QString getPage(int number);
    QString getInfo();
    int getCompletePercent();
    int getLastPageReady();

    //constructor
    OCRDocument(QString documentId, QString url, QObject *parent = 0);

protected:
    static void update();
    static OCRDocument *getNextFreeDocument();

private slots:
    void downloaded(QNetworkReply* data);
    void downloadProgress (qint64 received, qint64 total);

private:
    static QString PATH_FILES;

    QString _documentId;
    QString _format;
    QString _url;
    int _numberOfPages;
    QList<QString> _pagesProcessed;
    OCRDocument::Status _status;
    QNetworkAccessManager _manager;

    //documents management
    static QMap<QString,OCRDocument *> _sDocuments;
    static QList<OCRProcessor *> _sProcessors;
    static bool _sStarted;
    static int _sId;

    //update timer
    static QTimer _sTimer;
    //class created to handle the connect method with a static method
    friend class OCRTimerResponse;

};

/**
 * @brief The OCRTimerResponse class connects the timer to a static method in the OCRDocument
 * OCRDocuments cannot be connected because they're easelly deleted
 */
class OCRTimerResponse : public QObject {
    Q_OBJECT

private:
    static OCRTimerResponse _sSingleton;
    OCRTimerResponse() : QObject() {
        QObject::connect(&OCRDocument::_sTimer,SIGNAL(timeout()),this,SLOT(update()));
    }
private slots:
    void update() {
        OCRDocument::update();
    }
};


#endif // OCRDOCUMENT_H
