#ifndef OCREXCEPTION_H
#define OCREXCEPTION_H

#include <iostream>

class OCRException : public std::exception
{
public:
    OCRException(std::string error);
    const char * what() const throw();
    virtual ~OCRException() throw() {}
private:
    std::string _error;
};

#endif // OCREXCEPTION_H
