#include <QCoreApplication>
#include "ocrserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new OCRServer();
    return a.exec();
}
