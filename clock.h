#ifndef CLOCK_H
#define CLOCK_H

#include "subject.h"

class Clock : public Subject
{
public:
    Clock();

    virtual int GetHour();
    virtual int GetMinute();
    virtual int GetSecond();

    void Tick();

};

#endif // CLOCK_H
