#include "GuidePanel.h"


void GuidePanel::setFont(ofTrueTypeFont font)
{
    this->font = font;
}

void GuidePanel::setIcon(ofImage icon)
{
    this->icon = icon;
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

void GuidePanel::draw(float x, float y)
{
    ypos = y + (this->height + font.stringHeight("3")) * 0.5;
    
    ofPushStyle();
        ofSetColor(color.r, color.g, color.b);
        ofRect(x, y, width, height);
    ofPopStyle();
    
    ofPushMatrix();
        ofPushStyle();
            ofSetColor(0);
            ofTranslate(this->width * 0.5 - 150, ypos);
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
