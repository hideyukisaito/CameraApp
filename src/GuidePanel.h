#pragma once

#ifndef testApp_GuidePanel_h
#define testApp_GuidePanel_h
#endif

#include "ofMain.h"
#include "ofTrueTypeFont.h"
#include "ofxTween.h"
#include "ofxTimer.h"
#include "ofSoundPlayer.h"

class GuidePanel
{
public:
    GuidePanel()
    {
        x = 0;
        y = ofGetHeight();
        bShow = true;
        bFocus = false;
        focusAlpha = 0;
        
        hide();
        
        ofAddListener(ofEvents.update, this, &GuidePanel::update);
        ofAddListener(timer.TIMER_REACHED, this, &GuidePanel::onTimerReached);
    };
    
    ~GuidePanel()
    {
        ofRemoveListener(ofEvents.update, this, &GuidePanel::update);
        count.unloadSound();
        shutter.unloadSound();
    };
    
    void setFont(const string name, int size);
    void setFont(ofTrueTypeFont f);
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
    
    ofEvent<ofEventArgs> onCountDownCompleted;
    
    
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
    float focusAlpha;
    
    ofxTimer timer;
    void onTimerReached(ofEventArgs &e);
    
    ofxTween yTween;
    ofxTween focusXTween;
    ofxTween focusAlphaTween;
    ofxEasingCirc easingCirc;
    
    ofSoundPlayer count;
    ofSoundPlayer shutter;
};
