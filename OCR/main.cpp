#include <QCoreApplication>
#include "ocrserver.h"
#include "ocrdocument.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new OCRServer();
    return a.exec();
}
