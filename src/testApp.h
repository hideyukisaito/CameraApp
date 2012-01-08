#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxQTKitVideoGrabber.h"
#include "ofxControlPanel.h"
#include "ofxOsc.h"
#include "Dialog.h"
#include "GuidePanel.h"
#include "ofxTween.h"
#include "ofxTimer.h"
#include "Timer.h"

#define CAMERA_APP_SETTING_FILE "CameraApp.xml"
#define SLIDESHOW_APP_SETTING_FILE "SlideShowApp.xml"

#define SCREEN_WIDTH 873
#define SCREEN_HEIGHT 1396
#define SLIDESHOW_WIDTH  1200
#define SLIDESHOW_HEIGHT 1920

#define STATE_1 "standby"
#define STATE_2 "confirmation"
#define STATE_3 "save"
#define STATE_4 "completed"

#define HOST "localhost"
#define PORT_RECEIVE 15001
#define PORT_SEND 15002

class testApp : public ofBaseApp
{
public:
    void setup();
    void setupVideoGrabber();
    void setupControlPanel();
	void update();
	void draw();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    void exit();
    
    void enableGrabFrame();
    void readyForShoot();
    void shoot();
    void saveImage();
    
    bool isGuiAvalable;
    bool bGrabFrame;
    bool bShoot;
    
    ofxControlPanel gui;
    
    ofxQTKitVideoGrabber vidGrabber;
    float vidWidth;
    float vidHeight;
    float vidX;
    float vidY;
    
    int snapCount;
    
    GuidePanel panel;
    
private:
    ofxXmlSettings cameraAppSetting;
    ofxXmlSettings slideShowAppSetting;
    
    ofxTimer timer;
    
    Dialog dialog;
    
    int loadingRot;
    ofImage loading;
    
    ofImage capture;
    
    int getSnapCount();
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    
    ofxTween captureAlphaTween;
    ofxTween overlayAlphaTween;
    ofxTween loadingAlphaTween;
    ofxEasingCirc easingCirc;
    
    void drawCapturedImage();
    void sendUpdateToSlideShowApp();
    
    void onDialogShowCompleted(string &statename);
    void onCountDownCompleted(ofEventArgs &e);
    void onMessageReceived(ofxOscMessage &msg);
    void onImageSaving(ofEventArgs &e);
};