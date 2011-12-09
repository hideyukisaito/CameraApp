#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxQTKitVideoGrabber.h"
#include "ofxControlPanel.h"
#include "ofxOsc.h"
#include "GuidePanel.h"
#include "Timer.h"

#define SCREEN_WIDTH 873
#define SCREEN_HEIGHT 1396
#define SLIDESHOW_WIDTH  1200
#define SLIDESHOW_HEIGHT 1920

#define CAMERA_APP_SETTING_FILE "/Users/otiashee/Develop/openFrameworks/of_preRelease_v007_osx/apps/myApps/SlideShowApp/bin/data/CameraApp.xml"
#define SLIDESHOW_APP_SETTING_FILE "/Users/otiashee/Develop/openFrameworks/of_preRelease_v007_osx/apps/myApps/SlideShowApp/bin/data/SlideShowApp.xml"

#define HOST "localhost"
#define PORT_RECEIVE 15001
#define PORT_SEND 15002

class testApp : public ofBaseApp
{

public:
    void setup();
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
    
    ofxXmlSettings cameraAppSetting;
    ofxXmlSettings slideShowAppSetting;
    
    bool isPanelAvailable;
    bool isGuiAvalable;
    bool bGrabFrame;
    bool bShoot;
    
    float panelY;
    
    int rectAlpha;
    
    ofxControlPanel gui;
    
    ofxQTKitVideoGrabber vidGrabber;
    float vidWidth;
    float vidHeight;
    float vidX;
    float vidY;
    
    ofImage img;
    int snapCount;
    
    ofImage guide, guide1, guide2;
    float guide1Y;
    int guide1Alpha;
    
    GuidePanel panel;
    Timer timer;
    
    ofxOscSender sender;
    ofxOscReceiver receiver;
};