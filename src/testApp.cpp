#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(128);
    
    cameraAppSetting.loadFile(ofToDataPath(CAMERA_APP_SETTING_FILE));
    slideShowAppSetting.loadFile(ofToDataPath(SLIDESHOW_APP_SETTING_FILE));
    
    // init snap count
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
    
    dialog1.loadImage(ofToDataPath("info_1.png"));
    dialog2.loadImage(ofToDataPath("info_2.png"));
    dialog = dialog1;
    dialog1Y = ofGetHeight() - dialog1.height * 2;
    
    rectAlpha = 0;
    
    isGuiAvalable = true;
    isPanelAvailable = false;
    bShoot = false;
    bGrabFrame = true;
    
    ofTrueTypeFont arial;
    arial.loadFont(ofToDataPath(cameraAppSetting.getValue("FONT:NAME", "arial.ttf")), cameraAppSetting.getValue("FONT:SIZE", 20), true, true);
    arial.setLineHeight(cameraAppSetting.getValue("FONT:LINE_HEIGHT", 24.0f));
    arial.setLetterSpacing(cameraAppSetting.getValue("FONT:LETTER_SPACING", 1.037));
    
    panelY = ofGetHeight();
    ofColor color(255);
    panel.setFont(cameraAppSetting.getValue("FONT:NAME", "arial.ttf"), cameraAppSetting.getValue("FONT:SIZE", 20));
    panel.setColor(color);
    panel.setSize(ofGetWidth(), 120);
    
    setupVideoGrabber();
    setupControlPanel();
    
    img.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR_ALPHA);
    
    snapCount = getSnapCount();
    cout << "snapCount: " << snapCount << endl;
    
    sender.setup(HOST, PORT_SEND);
    receiver.setup(PORT_RECEIVE);
    ofAddListener(receiver.onMessageReceived, this, &testApp::onMessageReceived);
}

//--------------------------------------------------------------
void testApp::setupVideoGrabber()
{
    //vidGrabber.setDeviceID(4);
    vidGrabber.initGrabber(cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_WIDTH", 1280), cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_HEIGHT", 720));
    
    cout << "vidGrabber.height = " << ofToString(vidGrabber.getHeight()) << endl;
    cout << "vidGrabber.width = " << ofToString(vidGrabber.getWidth()) << endl;
    cout << "(float)SCREEN_HEIGHT / (float)vidGrabber.getWidth() = " << ofToString((float)SCREEN_HEIGHT / (float)vidGrabber.getWidth()) << endl;
    cout << "(float)SCREEN_WIDTH / (float)vidGrabber.getHeight() = " << ofToString((float)SCREEN_WIDTH / (float)vidGrabber.getHeight()) << endl;
    vidWidth = vidGrabber.getWidth() * ((float)SCREEN_HEIGHT / (float)vidGrabber.getWidth());
    vidHeight = vidGrabber.getHeight() * (float)(SCREEN_WIDTH / (float)vidGrabber.getHeight());
    vidX = -vidWidth;
    vidY = 0;
}

//--------------------------------------------------------------
void testApp::setupControlPanel()
{
    gui.setup("Settings", 0, 0, 270, 400);
    gui.addPanel("", 1);
    gui.addTextInput("OSC send port", "oscSendPort", ofToString(PORT_SEND), 80);
    gui.addTextInput("OSC receive port", "oscReceivePort", ofToString(PORT_RECEIVE), 80);
    gui.addSlider("video width", "vidHeight", vidHeight, vidHeight, vidHeight * 2, false);
    gui.addSlider("video height", "vidWidth", vidWidth, vidWidth, vidWidth * 2, false);
    gui.addSlider("width offset", "vidHeightOffset", 1, 1, 2, false);
    gui.addSlider("height offset", "vidWidthOffset", 1, 1, 2, false);
    gui.addToggle("resize on save", "bResize", false);;
    gui.loadSettings(ofToDataPath("controlPanel.xml"));
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(0);
    
    if (bGrabFrame) vidGrabber.grabFrame();
    
    if (isPanelAvailable)
    {
        panelY += ((ofGetHeight() - panel.getHeight()) - panelY) * 0.2;
        dialog1Y += ((ofGetHeight() + 10) - dialog1Y) * 0.2;
    }
    else
    {
        panelY += (ofGetHeight() + 10 - panelY) * 0.2;
        dialog1Y += ((ofGetHeight() - dialog1.height * 2) - dialog1Y) * 0.2;
        //if (bGrabFrame && ofGetHeight() > dialog1Y) dialog = dialog1;
    }
    
    dialog = bGrabFrame ? dialog1 : dialog2;
    
    if (0 < rectAlpha)
    {
        rectAlpha = 0 >= rectAlpha ? 0 : rectAlpha -= 10;
    }
    
    //gui.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofPushMatrix();
        ofRotate(270);
        vidGrabber.draw(vidX, vidY, gui.getValueF("vidWidth") * gui.getValueF("vidWidthOffset"), gui.getValueF("vidHeight") * gui.getValueF("vidHeightOffset"));
    ofPopMatrix();
    
    dialog.draw((ofGetWidth() - dialog1.width) >> 1, dialog1Y);
    
    shoot();
    
    panel.draw();
    
    if (isGuiAvalable)
    {
        gui.draw();
    }
}

//--------------------------------------------------------------
void testApp::enableGrabFrame()
{
    bShoot = false;
    bGrabFrame = true;
}

//--------------------------------------------------------------
void testApp::readyForShoot()
{
    timer.reset();
    bShoot = true;
    isPanelAvailable = true;
    panel.show();
}

//--------------------------------------------------------------
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
            panel.hide();
            isPanelAvailable = false;
            rectAlpha = 255;
        }
    }
    ofPushStyle();
        ofSetColor(255, 255, 255, rectAlpha);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::saveImage()
{
    string path = cameraAppSetting.getValue("EXPORT_PATH", "");
    ++snapCount;
    cout << "saveImage - snapCount: " << snapCount << endl;
   
     
    img.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR);
    // if (gui.getValueB("bResize")) img.resize(1920, 1200);
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
    
    sendUpdateToSlideShowApp();
}

//--------------------------------------------------------------
int testApp::getSnapCount()
{
    int count;
    
    if (slideShowAppSetting.pushTag("IMAGE"))
    {
        if (slideShowAppSetting.pushTag("PERSONS"))
        {
            count = slideShowAppSetting.getNumTags("FILE_NAME");
            slideShowAppSetting.popTag();
        }
        slideShowAppSetting.popTag();
    }
    
    return count;
}

//--------------------------------------------------------------
void testApp::sendUpdateToSlideShowApp()
{
    ofxOscMessage msg;
    msg.setAddress("/image/filename");
    msg.addStringArg(ofToString(snapCount) + ".jpg");
    sender.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::onMessageReceived(ofxOscMessage &msg)
{
    string addr = msg.getAddress();
    int state;
    
    if ("/button/1" == addr)
    {
        string s = msg.getArgAsString(0);
        cout << "state = " << s << endl;
        if (bGrabFrame)
        {
            if (1 == ofToInt(msg.getArgAsString(0))) readyForShoot();
        }
        else
        {
            if (1 == ofToInt(msg.getArgAsString(0))) saveImage();
        }
    }
    else if ("/button/2" == addr)
    {
        if (!bGrabFrame)
        {
            if (1 == ofToInt(msg.getArgAsString(0))) enableGrabFrame();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    gui.keyPressed(key);
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
    else if (OF_KEY_RETURN == key)
    {
        receiver.setup(ofToInt(gui.getValueS("oscReceivePort")));
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
    dialog.clear();
    dialog1.clear();
    dialog2.clear();
    vidGrabber.close();
    cameraAppSetting.saveFile();
}