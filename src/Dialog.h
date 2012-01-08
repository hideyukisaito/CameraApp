//
//  Dialog.h
//  CameraApp
//
//  Created by Hideyuki Saito on 12/01/08.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//
#pragma once

#include "ofMain.h"
#include "ofxTween.h"

#ifndef CameraApp_Dialog_h
#define CameraApp_Dialog_h

class Dialog
{
public:
    void setup(int duration);
    void draw();
    
    void show();
    void hide();
    bool addState(const string statename, const string filename);
    bool hasState(const string statename);
    void changeState(const string statename);
    void changeState(const string statename, float delay);
    string getCurrentStateName();
    
    ofEvent<string> onShowCompleted;
    ofEvent<string> onHideCompleted;
    ofEvent<string> onStateChanged;
    
private:
    map<string, ofImage> states;
    
    ofImage current;
    ofImage next;
    string currentStateName;
    string nextStateName;
    
    bool bChange;
    int duration;
    float ypos;
    ofxTween yposTween;
    ofxEasingCirc easingCirc;
    ofxEasingElastic easingElastic;
    
    void currentDialogShowComplete(int &id);
    void currentDialogHideComplete(int &id);
};

#endif
