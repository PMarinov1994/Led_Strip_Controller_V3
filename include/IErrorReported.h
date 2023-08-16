#ifndef __IERROR_REPORTER_H
#define __IERROR_REPORTER_H

#include "Arduino.h"

class IErrorReporter
{
public:
    IErrorReporter() {};
    virtual ~IErrorReporter() {};

public:
    virtual void ReportError(String err) = 0;
};

#endif