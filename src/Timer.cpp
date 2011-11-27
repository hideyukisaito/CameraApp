#include "Timer.h"

void Timer::update()
{
    millis += millisPerFrame;
}

void Timer::reset()
{
    millis = 0;
}

float Timer::getSecond()
{
    return millis;
}