#include "GuidePanel.h"


void GuidePanel::setFont(const string name, int size)
{
    font.loadFont(ofToDataPath(name), size, true, true);
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
}

void GuidePanel::show()
{
    if (!bShow)
    {
        bShow = true;
        yTween.setParameters(1, easingCirc, ofxTween::easeOut, y, ofGetHeight() - height, 500, 0);
    }
}

void GuidePanel::hide()
{
    if (bShow)
    {
        bShow = false;
        yTween.setParameters(1, easingCirc, ofxTween::easeOut, y, ofGetHeight(), 500, 0);
    }
}

void GuidePanel::update(ofEventArgs &e)
{
    y = yTween.update();
    cout << "y: " << y << endl;
}

void GuidePanel::draw()
{
    ypos = y + (height + font.stringHeight("3")) * 0.5;
    
    ofPushStyle();
        ofSetColor(color.r, color.g, color.b);
        ofRect(x, y, width, height);
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
            icon.draw(x + 300, -icon.height);
        ofPopStyle();
    ofPopMatrix();
}

void GuidePanel::updateFocus()
{
    bFocus = true;
}

void GuidePanel::onTimerReached(ofEventArgs &e)
{
    
}
