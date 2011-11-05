#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(30);
    ofBackground(0);
    ofEnableSmoothing();
    
    isGuiAvalable = false;
    isPanelAvailable = false;
    
    gui.setup("control panel test", 0, 0, 340, 400);
    gui.addPanel("panel 1", 1);
    gui.addSlider("rect size", "size", 100, 0, 800, true);
    gui.addSlider("rotation x", "rotX", 0, 0, 360, false);
    gui.addSlider("rotation y", "rotY", 0, 0, 360, false);
    gui.addSlider("rotation z", "rotZ", 0, 0, 360, false);
    gui.loadSettings("controlPanel.xml");
    
    panelY = ofGetHeight();
    ofColor color(255);
    panel.setColor(color);
    panel.setSize(ofGetWidth(), 120);
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(960, 720);
    
    img.allocate(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, OF_IMAGE_COLOR);
    
    data.watch("/Users/otiashee/Downloads");
    
    root.watch("/");
    ofAddListener(root.updated, &listener, &RootFolderListener::onUpdated);
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(0);
    
    bool isNewFrame = false;
    
    vidGrabber.grabFrame();
    
    img.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR);
    img.crop((img.width - ofGetWidth()) / 2, (img.height - ofGetHeight()) / 2, ofGetWidth(), ofGetHeight());
    
    if (isPanelAvailable)
    {
        panelY += ((ofGetHeight() - panel.getHeight()) - panelY) * 0.3;
    }
    else
    {
        panelY += (ofGetHeight() - panelY) * 0.5;
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    img.draw(0, 0);
    
    panel.draw(0, panelY);
    
    if (isGuiAvalable)
    {
        gui.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (32 == key)
    {
        isGuiAvalable = !isGuiAvalable;
    }
    else if (OF_KEY_UP == key)
    {
        isPanelAvailable = true;
    }
    else if (OF_KEY_DOWN == key)
    {
        isPanelAvailable = false;
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