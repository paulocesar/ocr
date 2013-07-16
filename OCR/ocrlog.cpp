#include "ocrlog.h"

OCRLog::OCRLog()
{
}

void OCRLog::put(const char * message)
{
    std::cout << "OCR: " << message << std::endl;
}

void OCRLog::put(QString message)
{
    std::cout  << "OCR: " << message.toStdString().c_str() << std::endl;
}
