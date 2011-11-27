#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxStat.h"
#include "GuidePanel.h"
#include "Timer.h"

#define SCREEN_WIDTH 873
#define SCREEN_HEIGHT 1396
#define SLIDESHOW_WIDTH  1200
#define SLIDESHOW_HEIGHT 1920

class DataFolderWatcher : public ofxStat
{
public:
    void onUpdated()
    {
        printf("DataFolderWatcher: updated!");
    }
};

class RootFolderListener
{
public:
    void onUpdated(ofEventArgs&)
    {
        printf("RootFolderListener: updated!");
    }
};

class testApp : public ofBaseApp
{

public:
    ~testApp()
    {
        XML.saveFile("settings.xml");
    }
    
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
    
    ofxXmlSettings XML;
    
    void shoot();
    void saveImage();
    
    bool isPanelAvailable;
    bool isGuiAvalable;
    bool bGrabFrame;
    bool bShoot;
    
    float panelY;
    
    int rectAlpha;
    
    ofxControlPanel gui;
    
    ofVideoGrabber vidGrabber;
    float vidWidth;
    float vidHeight;
    float vidX;
    float vidY;
    
    ofImage img;
    int snapCount;
    
    ofImage guide01;
    float guide01Y;
    int guide01Alpha;
    
    GuidePanel panel;
    Timer timer;
    
    DataFolderWatcher data;
    
    ofxStat root;
    RootFolderListener listener;
};