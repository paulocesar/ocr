#include <QCoreApplication>
#include "ocrserver.h"
#include "ocr.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    new OCRServer();
    OCR::startup();
    OCR::startup();
    return a.exec();
}
