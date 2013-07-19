#ifndef OCRPROCESSOR_H
#define OCRPROCESSOR_H

#include <QProcess>

class OCRDocument;

class OCRProcessor : public QProcess
{
    Q_OBJECT
public:

    enum Status {
        STATUS_IDLE = 1,
        STATUS_CONVERTING = 2,
        STATUS_PROCESSING = 4,
        STATUS_READING = 8
    };

    //status
    bool is(OCRProcessor::Status status);
    bool isFree();

    //processor manipulation
    bool process(OCRDocument *document);
    void interrupt();

    explicit OCRProcessor(QObject *parent = 0);

signals:
    void processInterrupted(OCRProcessor *process);

private slots:
    //finite state machine
    bool updateState(int exitCode = 0);

private:
    void convertToImage();
    void convertToText();
    void readFromFiles();

    QString cmdConvert(QString orig,QString dest);
    QString cmdTesseract(QString orig,QString dest);
    QString cmdIdentify(QString orig);

private:
    OCRProcessor::Status _status;
    OCRDocument *_document;
    bool _interrupt;
};

#endif // OCRPROCESSOR_H
