#include "GuidePanel.h"


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
    ofSetColor(color.r, color.g, color.b);
    ofRect(x, y, width, height);
}
