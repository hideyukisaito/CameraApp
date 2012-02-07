#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    // general settings
    ofSetFrameRate(60);
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetCircleResolution(128);
    
    // load setting xml files
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
    
    // setup information dialog
    dialog.setup(1000);
    dialog.addState(STATE_1, "info_1.png");
    dialog.addState(STATE_2, "info_2.png");
    dialog.addState(STATE_3, "info_3.png");
    dialog.addState(STATE_4, "info_4.png");
    dialog.changeState(STATE_1);
    ofAddListener(dialog.onShowCompleted, this, &testApp::onDialogShowCompleted);
    
    // load ajax loading indicator
    loading.loadImage(ofToDataPath("loading.png"));
    loading.setImageType(OF_IMAGE_COLOR_ALPHA);
    
    // init flags
    isGuiAvalable = true;
    bShoot = false;
    bGrabFrame = true;
    
    // setup countdown panel
    ofColor color(255);
    ofTrueTypeFont font;
    panel.setFont(cameraAppSetting.getValue("FONT:NAME", "arial.ttf"), cameraAppSetting.getValue("FONT:SIZE", 20));
    panel.setColor(color);
    panel.setSize(ofGetWidth(), 120);
    
    // setup ofxQTKitVideoGrabber
    setupVideoGrabber();
    
    // setup ofxControlPanel
    setupControlPanel();
    isGuiAvalable = false;
    
    // init ofImage for captured image
    capture.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR_ALPHA);
    
    snapCount = getSnapCount();
    
    // setup OSC
    bOscEnabled = true;
    sender.setup(HOST, PORT_SEND);
    receiver.setup(PORT_RECEIVE);
    ofAddListener(receiver.onMessageReceived, this, &testApp::onMessageReceived);
}

//--------------------------------------------------------------
void testApp::setupVideoGrabber()
{
    //vidGrabber.setDeviceID(4);
    vidGrabber.initGrabber(cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_WIDTH", 1280), cameraAppSetting.getValue("VIDEO_GRABBER:FRAME_HEIGHT", 720));
    
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
void testApp::drawCapturedImage()
{
    ofPushStyle();
        ofSetColor(255, 255, 255, captureAlphaTween.update());
        ofPushMatrix();
            ofRotate(-90);
            ofTranslate(-SCREEN_HEIGHT, 0);
            capture.draw(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
        ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void testApp::update()
{
    ofBackground(0);
    
    if (bGrabFrame) vidGrabber.grabFrame();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofPushMatrix();
        ofRotate(270);
        vidGrabber.draw(vidX, vidY, gui.getValueF("vidWidth") * gui.getValueF("vidWidthOffset"), gui.getValueF("vidHeight") * gui.getValueF("vidHeightOffset"));
    ofPopMatrix();
    
    drawCapturedImage();
    
    int overlayAlpha = (int)overlayAlphaTween.update();
    
    ofPushStyle();
        ofSetColor(255, 255, 255, overlayAlpha);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofPopStyle();
    
    dialog.draw();
    if (STATE_3 == dialog.getCurrentStateName())
    {
        ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotate(loadingRot);
            ofPushStyle();
            ofSetColor(255, 255, 255, overlayAlpha * 2);
            loading.draw(-loading.getWidth() / 2, -loading.getHeight() / 2);
            ofPopStyle();
        ofPopMatrix();
    }
    
    panel.draw();
    
    if (isGuiAvalable)
    {
        gui.draw();
    }
}

//--------------------------------------------------------------
void testApp::enableGrabFrame()
{
    captureAlphaTween.setParameters(4, easingCirc, ofxTween::easeOut, 255, 0, 1000, 0);
    dialog.changeState(STATE_1, 500);
    
    bShoot = false;
    bGrabFrame = true;
}

//--------------------------------------------------------------
void testApp::readyForShoot()
{
    bOscEnabled = false;
    bShoot = true;
    
    ofAddListener(panel.onCountDownCompleted, this, &testApp::onCountDownCompleted);
    dialog.hide();
    panel.show();
}

//--------------------------------------------------------------
void testApp::shoot()
{
    if (bShoot)
    {
        capture.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight(), OF_IMAGE_COLOR);
        captureAlphaTween.setParameters(4, easingCirc, ofxTween::easeOut, 0, 255, 1000, 0);
        
        dialog.changeState(STATE_2);
        
        bShoot = false;
        bGrabFrame = false;
    }
}

//--------------------------------------------------------------
void testApp::saveImage()
{
    string path = cameraAppSetting.getValue("EXPORT_PATH", "");
    ++snapCount;
    
    ofImage img;
    img.clone(capture);
    img.resize(1920, 1200);
    img.rotate90(135);
    img.saveImage(ofToDataPath(path + ofToString(snapCount) + ".jpg"));
    
    cameraAppSetting.setValue("SNAP_COUNT", snapCount);
    cameraAppSetting.saveFile();
    
    if (slideShowAppSetting.pushTag("IMAGE"))
    {
        if (slideShowAppSetting.pushTag("PERSONS"))
        {
            slideShowAppSetting.addValueAtFirst("FILE_NAME", ofToString(snapCount) + ".jpg");
            slideShowAppSetting.saveFile();
            slideShowAppSetting.popTag();
        }
        slideShowAppSetting.popTag();
    }
    
    sendUpdateToSlideShowApp();
    
    dialog.changeState(STATE_4);
    overlayAlphaTween.setParameters(1, easingCirc, ofxTween::easeOut, 127, 0, 1000, 1000);
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
void testApp::onDialogShowCompleted(string &statename)
{
    cout << "onStateChanged statename = " << statename << endl;
    
    if (STATE_1 == statename || STATE_2 == statename)
    {
        bOscEnabled = true;
    }
    else if (STATE_3 == statename)
    {
        bOscEnabled = false;
        saveImage();
    }
    else if (STATE_4 == statename)
    {
        ofRemoveListener(timer.TIMER_REACHED, this, &testApp::onImageSaving);
        timer.stopTimer();
        
        enableGrabFrame();
        dialog.changeState(STATE_1, 2000);
    }
}

//--------------------------------------------------------------
void testApp::onCountDownCompleted(ofEventArgs &e)
{
    overlayAlphaTween.setParameters(1, easingCirc, ofxTween::easeOut, 255, 0, 500, 0);
    shoot();
    
    ofRemoveListener(panel.onCountDownCompleted, this, &testApp::onCountDownCompleted);
    panel.hide();
}

//--------------------------------------------------------------
void testApp::onMessageReceived(ofxOscMessage &msg)
{
    if (bOscEnabled)
    {
        string addr = msg.getAddress();
        int state;
        
        if ("/button/1" == addr)
        {
            string s = msg.getArgAsString(0);
            
            if (bGrabFrame)
            {
                if (1 == ofToInt(msg.getArgAsString(0))) readyForShoot();
            }
            else
            {
                if (1 == ofToInt(msg.getArgAsString(0)))
                {
                    overlayAlphaTween.setParameters(1, easingCirc, ofxTween::easeOut, 0, 127, 1000, 0);
                    
                    ofAddListener(timer.TIMER_REACHED, this, &testApp::onImageSaving);
                    timer.setup(50, true);
                    loadingRot = 0;
                    
                    dialog.changeState(STATE_3);
                }
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
}

//--------------------------------------------------------------
void testApp::onImageSaving(ofEventArgs &e)
{
    loadingRot = 360 == loadingRot ? 45 : loadingRot + 45;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    gui.keyPressed(key);
    if (32 == key)
    {
       isGuiAvalable = !isGuiAvalable;
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
void testApp::exit()
{
    capture.clear();
    vidGrabber.close();
    loading.clear();
    
    cameraAppSetting.saveFile();
    slideShowAppSetting.saveFile();
}