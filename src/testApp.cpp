#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(128);
    
    XML.loadFile("settings.xml");
    
    guide01.loadImage("info_1.png");
    guide01Y = ofGetHeight() - guide01.height * 2;
    
    rectAlpha = 0;
    
    isGuiAvalable = false;
    isPanelAvailable = false;
    bShoot = false;
    bGrabFrame = true;
    
    ofTrueTypeFont arial;
    arial.loadFont(XML.getValue("FONT:NAME", "arial.ttf"), XML.getValue("FONT:SIZE", 20), true, true);
    arial.setLineHeight(XML.getValue("FONT:LINE_HEIGHT", 24.0f));
    arial.setLetterSpacing(XML.getValue("FONT:LETTER_SPACING", 1.037));
    
    ofImage icon;
    icon.loadImage("camera.png");
    
    panelY = ofGetHeight();
    ofColor color(255);
    panel.setFont(arial);
    panel.setIcon(icon);
    panel.setColor(color);
    panel.setSize(ofGetWidth(), 120);
    
    vidGrabber.setVerbose(false);
    vidGrabber.setUseTexture(false);
    vidGrabber.initGrabber(XML.getValue("VIDEO_GRABBER:FRAME_WIDTH", 1280), XML.getValue("VIDEO_GRABBER:FRAME_HEIGHT", 720));
    vidWidth = vidGrabber.width * (SCREEN_WIDTH / (float)vidGrabber.height);
    vidHeight = vidGrabber.height * (SCREEN_WIDTH / (float)vidGrabber.height);
    vidX = -vidWidth;
    vidY = 0;
    
    gui.setup("control panel test", 0, 0, 340, 400);
    gui.addPanel("panel 1", 1);
    gui.addSlider("rotation", "rot", 100, 0, 360, false);
    gui.addSlider("video x", "vidX", 0, -2000, 2000, false);
    gui.addSlider("video y", "vidY", 0, -2000, 2000, false);
    gui.loadSettings("controlPanel.xml");
    
    img.allocate(1920, 1080, OF_IMAGE_COLOR);
    snapCount = XML.getValue("SNAP_COUNT", 0);
    
    data.watch("/Users/otiashee/Develop/openFrameworks/of_preRelease_v007_osx/apps/myApps/testApp/bin/data/export");
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(0);
    
    bool isNewFrame = false;
    
    if (bGrabFrame) vidGrabber.grabFrame();
    
    //img.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR);
    //img.crop((vidGrabber.getWidth() - img.getWidth()) >> 1, (vidGrabber.getHeight() - img.getHeight()) >> 1, 900, ofGetHeight());
    //img.resize(img.getWidth() * 2, img.getHeight() * 2);
    
    if (isPanelAvailable)
    {
        panelY += ((ofGetHeight() - panel.getHeight()) - panelY) * 0.2;
        guide01Y += ((ofGetHeight() + 10) - guide01Y) * 0.2;
        
    }
    else
    {
        panelY += (ofGetHeight() + 10 - panelY) * 0.2;
        guide01Y += ((ofGetHeight() - guide01.height * 2) - guide01Y) * 0.2;
    }
    
    if (0 < rectAlpha)
    {
        rectAlpha = 0 >= rectAlpha ? 0 : rectAlpha -= 10;
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofPushMatrix();
    ofRotate(270);
    vidGrabber.draw(vidX, vidY, vidWidth, vidHeight);
    ofPopMatrix();
    
    ofPushStyle();
    guide01.draw((ofGetWidth() - guide01.width) >> 1, guide01Y);
    
    testApp::shoot();
    
    panel.draw(0, panelY);
    
    if (isGuiAvalable)
    {
        gui.draw();
    }
}

void testApp::shoot()
{
    if (bShoot)
    {
        timer.update();
        panel.updateFocus();
        if (3 < timer.getSecond() && 3.2 > timer.getSecond())
        {
            testApp::saveImage();
        }
    }
    ofPushStyle();
        ofSetColor(255, 255, 255, rectAlpha);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
}

void testApp::saveImage()
{
    bShoot = false;
    bGrabFrame = false;
    isPanelAvailable = false;
    rectAlpha = 255;
    
    img.setFromPixels(vidGrabber.getPixels(), vidGrabber.width, vidGrabber.height, OF_IMAGE_COLOR);
    img.resize(1920, 1080);
    img.rotate90(135);
    img.saveImage(XML.getValue("SAVE_PATH", "export") + "/" + XML.getValue("IMAGE_PREFIX", "") + ofToString(snapCount++) + ".jpg");
    
    XML.setValue("SNAP_COUNT", snapCount);
    XML.saveFile("settings.xml");
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (32 == key)
    {
        timer.reset();
        bShoot = true;
        isPanelAvailable = true;
        
        isPanelAvailable = true;
        //isGuiAvalable = !isGuiAvalable;
    }
    else if (OF_KEY_UP == key)
    {
        isPanelAvailable = true;
    }
    else if (OF_KEY_DOWN == key)
    {
        isPanelAvailable = false;
    }
    else if (OF_KEY_RETURN == key)
    {
        bShoot = false;
        bGrabFrame = true;
    }
    else if ('s' == key)
    {
        XML.saveFile("settings.xml");
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}