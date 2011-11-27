#pragma once

#ifndef testApp_GuidePanel_h
#define testApp_GuidePanel_h
#endif

#include "ofMain.h"
#include "ofTrueTypeFont.h"
#include "ofxTween.h"

class GuidePanel
{
public:
    void setFont(ofTrueTypeFont font);
    void setIcon(ofImage icon);
    float getWidth();
    float getHeight();
    void setSize(float w, float y);
    void setColor(ofColor col);
    void draw(float x, float y);
    void updateFocus();
    
    GuidePanel()
    {
        bFocus = false;
        focusX = focusY = 0;
    }
    
    
private:
    float width;
    float height;
    ofColor color;
    
    ofTrueTypeFont font;
    float ypos;
    
    ofImage icon;
    
    bool bFocus;
    float focusX;
    float focusY;
};
