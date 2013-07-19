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

void OCRLog::warning(const char * message)
{
    std::cout << "OCR WARNING!: " << message << std::endl;
}

void OCRLog::warning(QString message)
{
    std::cout  << "OCR WARNING!: " << message.toStdString().c_str() << std::endl;
}
