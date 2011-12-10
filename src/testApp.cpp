#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    CAMERA_APP_SETTING_FILE = ofToDataPath("CameraApp.xml");
    SLIDESHOW_APP_SETTING_FILE = ofToDataPath("SlideShowApp.xml");
    
    cout << "CAMERA_APP_SETTING_FILE    : " << CAMERA_APP_SETTING_FILE << endl;
    cout << "SLIDESHOW_APP_SETTING_FILE : " << SLIDESHOW_APP_SETTING_FILE << endl;
    
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(128);
    
    cameraAppSetting.loadFile(CAMERA_APP_SETTING_FILE);
    slideShowAppSetting.loadFile(SLIDESHOW_APP_SETTING_FILE);
    if (slideShowAppSetting.pushTag("IMAGE"))
    {
        if (slideShowAppSetting.pushTag("PERSONS"))
        {
            cameraAppSetting.setValue("SNAP_COUNT", slideShowAppSetting.getNumTags("FILE_NAME") - 1);
            cameraAppSetting.saveFile();
            slideShowAppSetting.popTag();
        }
        slideShowAppSetting.popTag();
    }
    
    guide1.loadImage(ofToDataPath("info_1.png"));
    guide2.loadImage(ofToDataPath("info_2.png"));
    guide = guide1;
    guide1Y = ofGetHeight() - guide1.height * 2;
    
    rectAlpha = 0;
    
    isGuiAvalable = false;
    isPanelAvailable = false;
    bShoot = false;
    bGrabFrame = true;
    
    ofTrueTypeFont arial;
    arial.loadFont(ofToDataPath(cameraAppSetting.getValue("FONT:NAME", "arial.ttf")), cameraAppSetting.getValue("FONT:SIZE", 20), true, true);
    arial.setLineHeight(cameraAppSetting.getValue("FONT:LINE_HEIGHT", 24.0f));
    arial.setLetterSpacing(cameraAppSetting.getValue("FONT:LETTER_SPACING", 1.037));
    
    ofImage icon;
    icon.loadImage(ofToDataPath("camera.png"));
    
    panelY = ofGetHeight();
    ofColor color(255);
    panel.setFont(arial);
    panel.setIcon(icon);
    panel.setColor(color);
    panel.setSize(ofGetWidth(), 120);
    
    //vidGrabber.setDeviceID(4);
    vidGrabber.initGrabber(cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_WIDTH", 1280), cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_HEIGHT", 720));
    //vidGrabber.initGrabber(640, 480);
    vidWidth = vidGrabber.getWidth() * (SCREEN_WIDTH / (float)vidGrabber.getHeight());
    vidHeight = vidGrabber.getHeight() * (SCREEN_WIDTH / (float)vidGrabber.getHeight());
    vidX = -vidWidth;
    vidY = 0;
    
    gui.setup("control panel test", 0, 0, 340, 400);
    gui.addPanel("panel 1", 1);
    gui.addSlider("rotation", "rot", 100, 0, 360, false);
    gui.addSlider("video x", "vidX", 0, -2000, 2000, false);
    gui.addSlider("video y", "vidY", 0, -2000, 2000, false);
    gui.loadSettings(ofToDataPath("controlPanel.xml"));
    
    img.allocate(1920, 1080, OF_IMAGE_COLOR_ALPHA);
    
    if (slideShowAppSetting.pushTag("IMAGE"))
    {
        if (slideShowAppSetting.pushTag("PERSONS"))
        {
            snapCount = slideShowAppSetting.getNumTags("FILE_NAME");
            slideShowAppSetting.popTag();
        }
        slideShowAppSetting.popTag();
    }
    cout << "snapCount: " << snapCount << endl;
    
    sender.setup(HOST, PORT_SEND);
    receiver.setup(PORT_RECEIVE);
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(0);
    
    while (receiver.hasWaitingMessages())
    {
        ofxOscMessage msg;
        receiver.getNextMessage(&msg);
        string addr = msg.getAddress();
        int state;
        
        if ("/button/1" == addr)
        {
            state = ofToInt(msg.getArgAsString(0));
            std::printf("state: %d\n", state);
            if (bGrabFrame)
            {
                if (1 == state) readyForShoot();
            }
            else
            {
                if (1 == state) saveImage();
            }
        }
        else if ("/button/2" == addr)
        {
            state = ofToInt(msg.getArgAsString(0));
            if (!bGrabFrame)
            {
                if (1 == state) enableGrabFrame();
            }
        }
    }
    
    if (bGrabFrame) vidGrabber.grabFrame();
    
    if (isPanelAvailable)
    {
        panelY += ((ofGetHeight() - panel.getHeight()) - panelY) * 0.2;
        guide1Y += ((ofGetHeight() + 10) - guide1Y) * 0.2;
    }
    else
    {
        panelY += (ofGetHeight() + 10 - panelY) * 0.2;
        guide1Y += ((ofGetHeight() - guide1.height * 2) - guide1Y) * 0.2;
        //if (bGrabFrame && ofGetHeight() > guide1Y) guide = guide1;
    }
    
    guide = bGrabFrame ? guide1 : guide2;
    
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
    
    guide.draw((ofGetWidth() - guide1.width) >> 1, guide1Y);
    
    testApp::shoot();
    
    panel.draw(0, panelY);
    
    if (isGuiAvalable)
    {
        gui.draw();
    }
}

void testApp::enableGrabFrame()
{
    bShoot = false;
    bGrabFrame = true;
}

void testApp::readyForShoot()
{
    timer.reset();
    bShoot = true;
    isPanelAvailable = true;
}

void testApp::shoot()
{
    if (bShoot)
    {
        timer.update();
        panel.updateFocus();
        if (3 < timer.getSecond() && 3.2 > timer.getSecond())
        {
            bShoot = false;
            bGrabFrame = false;
            isPanelAvailable = false;
            rectAlpha = 255;
        }
    }
    ofPushStyle();
        ofSetColor(255, 255, 255, rectAlpha);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
}

void testApp::saveImage()
{
    string path = cameraAppSetting.getValue("EXPORT_PATH", "");
    ++snapCount;
    cout << "saveImage - snapCount: " << snapCount << endl;
    
    img.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR);
    img.resize(1920, 1200);
    img.rotate90(135);
    img.saveImage(ofToDataPath(path + ofToString(snapCount) + ".jpg"));
    
    cameraAppSetting.setValue("SNAP_COUNT", snapCount);
    cameraAppSetting.saveFile();
    
    if (slideShowAppSetting.pushTag("IMAGE"))
    {
        if (slideShowAppSetting.pushTag("PERSONS"))
        {
            slideShowAppSetting.addValue("FILE_NAME", ofToString(snapCount) + ".jpg");
            slideShowAppSetting.saveFile();
            slideShowAppSetting.popTag();
        }
        slideShowAppSetting.popTag();
    }
    
    
    ofxOscMessage msg;
    msg.setAddress("/image/filename");
    msg.addStringArg(ofToString(snapCount) + ".jpg");
    sender.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (32 == key)
    {
        readyForShoot();
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
        enableGrabFrame();
    }
    else if ('s' == key)
    {
        cameraAppSetting.saveFile();
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

//--------------------------------------------------------------
void testApp::exit()
{
    img.clear();
    guide.clear();
    guide1.clear();
    guide2.clear();
    vidGrabber.close();
    cameraAppSetting.saveFile();
}