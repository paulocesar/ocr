#ifndef OCRDOCUMENT_H
#define OCRDOCUMENT_H

#include <QList>
#include <QString>

class OCRDocument
{
public:
    enum Status {
        STATUS_FREE = 1,
        STATUS_CONVERTING = 2,
        STATUS_PROCESSING = 4,
        STATUS_COMPLETE = 8
    };

    OCRDocument(QString filename);

    bool isFree();

    QString getFilePath();
    QString getDir();
    QString getPage(int number);
    QString getInfo();
    int getLastPageReady();
    int getCompletePercent();

    QString getNextUnprocessedPagePath();
    void setNextProcessedPage(QString content);

    bool is(OCRDocument::Status status);
    OCRDocument::Status getStatus();
    void updateStatus();

protected:
    int getNextUnprocessedPage();

private:
    QString _filename;
    QString _dir;

    int _numberOfPages;
    QList<QString> _pagesProcessed;

    OCRDocument::Status _status;
};

#endif // OCRDOCUMENT_H
