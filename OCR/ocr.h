#ifndef OCR_H
#define OCR_H

#include <QProcess>
#include <QMap>
#include <QList>
#include <QTimer>

class OCRDocument;

class OCR : public QProcess
{
    Q_OBJECT
public:

    enum Status {
        STATUS_IDLE = 1,
        STATUS_PROCESSING = 2
    };

    static void startup(int limitInstances = 3, int updateTime = 2000);
    static void stop();

    static QString documentAdd(QString filename);
    static bool documentRemove(QString documentId);
    static QString documentGetPage(QString documentId, int page);
    static QString documentGetInfo(QString documentId);

protected:
    static bool sStarted;
    static QString getFreeDocument();

private slots:
    static void update();

protected:
    explicit OCR(QObject *parent = 0);
    QString getDocumentId();
    void process(QString documentId = "");
    OCR::Status getStatus();
    bool isDocumentComplete();

private slots:
    void checkProcess(int exitCode);




private:
    static int _sId;
    static QMap<QString, OCRDocument *> _sDocuments;
    static QList<OCR *> _sOcrs;
    static QTimer _sTimer;

    QString _documentId;
    OCR::Status _status;
};

#endif // OCR_H
