#pragma once

#ifndef testApp_GuidePanel_h
#define testApp_GuidePanel_h
#endif

#include "ofMain.h"
#include "ofTrueTypeFont.h"
#include "ofxTween.h"
#include "ofxTimer.h"

class GuidePanel
{
public:
    GuidePanel()
    {
        x = 0;
        y = ofGetHeight();
        bShow = true;
        bFocus = false;
        focusX = focusY = 0;
        
        hide();
        
        ofAddListener(ofEvents.update, this, &GuidePanel::update);
    };
    
    ~GuidePanel()
    {
        ofRemoveListener(ofEvents.update, this, &GuidePanel::update);
    };
    
    void setFont(const string name, int size);
    float getWidth();
    float getHeight();
    void setSize(float w, float y);
    void setColor(ofColor col);
    void show();
    void hide();
    void update(ofEventArgs &e);
    //void draw(float x, float y);
    void draw();
    void updateFocus();
    
    
private:
    bool bShow;
    
    float x, y;
    float width;
    float height;
    ofColor color;
    
    ofTrueTypeFont font;
    float ypos;
    
    ofImage icon;
    
    bool bFocus;
    float focusX;
    float focusY;
    
    ofxTimer timer;
    void onTimerReached(ofEventArgs &e);
    
    ofxTween yTween;
    ofxEasingCirc easingCirc;
};
