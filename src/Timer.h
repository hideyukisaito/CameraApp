#ifndef testApp_Timer_h
#define testApp_Timer_h
#endif

#include "ofMain.h"

class Timer
{
public:
    void update();
    void reset();
    float getSecond();
    
    Timer()
    {
        second = millis = 0;
        millisPerFrame = 1.0 / ofGetFrameRate();
        bDoneCallback = false;
    }
    
private:
    int second;
    float millis;
    float millisPerFrame;
    
    bool bDoneCallback;
};
