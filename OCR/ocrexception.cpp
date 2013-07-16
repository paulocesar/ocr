#include "ocrexception.h"

OCRException::OCRException(std::string error)
{
    _error = "!!!OCR exception: ";
    _error.append(error);
    std::cout << _error << std::endl;
}

const char * OCRException::what() const throw()
{
    return  + _error.c_str();
}
