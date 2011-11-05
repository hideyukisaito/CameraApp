#pragma once

#ifndef testApp_GuidePanel_h
#define testApp_GuidePanel_h
#endif

#include "ofMain.h"

class GuidePanel
{
public:
    
    float getWidth();
    float getHeight();
    void setSize(float w, float y);
    void setColor(ofColor col);
    void draw(float x, float y);
    
    
private:
    
    float width;
    float height;
    ofColor color;
};
