#include "GuidePanel.h"


void GuidePanel::setFont(const string name, int size)
{
    font.loadFont(ofToDataPath(name), size, true, true);
    icon.loadImage(ofToDataPath("camera.png"));
    
    count.loadSound(ofToDataPath("sound/count.mp3"));
    shutter.loadSound(ofToDataPath("sound/shutter.mp3"));
    shutter.setPosition(0.15f);
}

void GuidePanel::setFont(ofTrueTypeFont f)
{
    font = f;
    icon.loadImage(ofToDataPath("camera.png"));
}

float GuidePanel::getWidth()
{
    return width;
}

float GuidePanel::getHeight()
{
    return height;
}

void GuidePanel::setColor(ofColor col)

{
    color = col;
}

void GuidePanel::setSize(float w, float h)
{
    width = w;
    height = h;
    
    focusXTween.setParameters(2, easingCirc, ofxTween::easeOut, focusX, width * 0.5 - 153, 0, 0);
}

void GuidePanel::show()
{
    if (!bShow)
    {
        bShow = true;
        yTween.setParameters(1, easingCirc, ofxTween::easeOut, y, ofGetHeight() - height, 500, 0);
        focusAlphaTween.setParameters(2, easingCirc, ofxTween::easeOut, focusAlpha, 255, 300, 0);
        
        ofAddListener(timer.TIMER_REACHED, this, &GuidePanel::onTimerReached);
        timer.setup(1000, true);
        count.play();
    }
}

void GuidePanel::hide()
{
    if (bShow)
    {
        bShow = false;
        yTween.setParameters(1, easingCirc, ofxTween::easeOut, y, ofGetHeight(), 500, 0);
        focusXTween.setParameters(2, easingCirc, ofxTween::easeOut, focusX, width * 0.5 - 153, 0, 0);
        focusAlphaTween.setParameters(2, easingCirc, ofxTween::easeOut, focusAlpha, 0, 300, 0);
    }
}

void GuidePanel::update(ofEventArgs &e)
{
    y = yTween.update();
    focusX = focusXTween.update();
    focusAlpha = focusAlphaTween.update();
}

void GuidePanel::draw()
{
    ypos = y + (height + font.stringHeight("3")) * 0.5;
    
    ofPushStyle();
        ofSetColor(color.r, color.g, color.b);
        ofRect(x, y, width, height);
    ofPopStyle();
    
    ofPushStyle();
        ofSetColor(255, 102, 51, focusAlpha);
        ofRect(focusX, ypos + 5, 30, 5);
    ofPopStyle();
    
    ofPushMatrix();
        ofPushStyle();
            ofSetColor(0);
            ofTranslate(width * 0.5 - 150, ypos);
            font.drawString("3", x, 0);
            font.drawString("2", x + 100, 0);
            font.drawString("1", x + 200, 0);
        ofPopStyle();
        ofPushStyle();
            icon.draw(x + 295, -icon.height);
        ofPopStyle();
    ofPopMatrix();
}

void GuidePanel::updateFocus()
{
    bFocus = true;
}

void GuidePanel::onTimerReached(ofEventArgs &e)
{
    if (3 == timer.count)
    {
        shutter.play();
        
        timer.stopTimer();
        ofRemoveListener(timer.TIMER_REACHED, this, &GuidePanel::onTimerReached);
        
        static ofEventArgs args;
        ofNotifyEvent(onCountDownCompleted, args, this);
        
        return;
    }
    else
    {
        count.play();
    }
    
    focusXTween.setParameters(2, easingCirc, ofxTween::easeOut, focusX, focusX + 100, 300, 0);
}
