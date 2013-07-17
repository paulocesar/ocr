#ifndef OCRDOCUMENT_H
#define OCRDOCUMENT_H

#include <QMap>
#include <QObject>
#include <QList>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class OCRDocument : public QObject
{
    Q_OBJECT

public:
    enum Status {
        STATUS_DOWNLOADING = 1,
        STATUS_FREE = 2,
        STATUS_CONVERTING = 4,
        STATUS_PROCESSING = 8,
        STATUS_COMPLETE = 16
    };

    OCRDocument(QString documentId, QString url, QObject *parent = 0);

    //document access interface
    static void startup(int limitOcrs = 3);
    static void update();
    static void stop();

    static QString documentAdd(QString filename);
    static bool documentRemove(QString documentId);
    static QString documentPage(QString documentId, int page);
    static QString documentInfo(QString documentId);

    //processing page
    void setNumberOfPages(int pages);
    void setNextProcessedPage(QString content);

    //general information
    QString getId();
    QString getUrl();
    QString getFilename();
    QString getFormat();
    QString getPage(int number);
    QString getInfo();
    int getCompletePercent();
    int getLastPageReady();

    //status
    bool is(OCRDocument::Status status);
    bool isFree();
    OCRDocument::Status getStatus();
    void setStatus(OCRDocument::Status status);

private slots:
    void downloaded(QNetworkReply* data);
    void downloadProgress (qint64 received, qint64 total);

private:
    QNetworkAccessManager _manager;
    QString _documentId;
    QString _format;
    QString _url;
    int _numberOfPages;
    QList<QString> _pagesProcessed;
    OCRDocument::Status _status;

    //group of documents
    static QMap<QString,OCRDocument *> _sDocuments;

};

#endif // OCRDOCUMENT_H
