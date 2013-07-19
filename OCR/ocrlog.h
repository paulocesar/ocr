#ifndef OCRLOG_H
#define OCRLOG_H

#include <QString>
#include <iostream>

class OCRLog
{
public:

    static void put(const char * message);
    static void put(QString message);

    static void warning(const char * message);
    static void warning(QString message);

private:
    OCRLog();
};

#endif // OCRLOG_H
