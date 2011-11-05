#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxStat.h"
#include "GuidePanel.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1440

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
    
    bool isPanelAvailable;
    bool isGuiAvalable;
    
    float panelY;
    
    ofxControlPanel gui;
    
    ofVideoGrabber vidGrabber;
    ofImage img;
    
    GuidePanel panel;
    
    DataFolderWatcher data;
    
    ofxStat root;
    RootFolderListener listener;
};