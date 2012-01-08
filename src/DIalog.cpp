//
//  DIalog.cpp
//  CameraApp
//
//  Created by Hideyuki Saito on 12/01/08.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Dialog.h"

//--------------------------------------------------------------
void Dialog::setup(int duration)
{
    this->duration = duration;
}

//--------------------------------------------------------------
void Dialog::draw()
{
    ypos = yposTween.update();
    current.draw((ofGetWidth() - current.width) >> 1, ypos);
}

//--------------------------------------------------------------
void Dialog::show()
{
    
}

//--------------------------------------------------------------
void Dialog::hide()
{
    ofAddListener(yposTween.end_E, this, &Dialog::currentDialogHideComplete);
    yposTween.setParameters(1, easingCirc, ofxTween::easeOut, ypos, ofGetHeight(), duration / 3, 0);
}

//--------------------------------------------------------------
bool Dialog::addState(const string statename, const string filename)
{
    cout << "ofToDataPath(filename) = " << ofToDataPath(filename) << endl;
    ofImage img;
    if (img.loadImage(ofToDataPath(filename)))
    {
        states.insert(map<string, ofImage>::value_type(statename, img));
        current = states[statename];
        currentStateName = statename;
        
        return true;
    }
    return false;
}

//--------------------------------------------------------------
bool Dialog::hasState(const string statename)
{
    map<string, ofImage>::iterator iter = states.find(statename);
    if (iter != states.end())
    {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void Dialog::changeState(const string statename)
{
    if (hasState(statename))
    {
        bChange = true;
        
        nextStateName = statename;
        ofNotifyEvent(onStateChanged, currentStateName, this);
        
        ofAddListener(yposTween.end_E, this, &Dialog::currentDialogHideComplete);
        yposTween.setParameters(1, easingCirc, ofxTween::easeOut, ypos, ofGetHeight(), duration / 3, 0);
    }
}

//--------------------------------------------------------------
void Dialog::changeState(const string statename, float delay)
{
    if (hasState(statename))
    {
        bChange = true;
        
        nextStateName = statename;
        ofNotifyEvent(onStateChanged, currentStateName, this);
        
        ofAddListener(yposTween.end_E, this, &Dialog::currentDialogHideComplete);
        yposTween.setParameters(1, easingCirc, ofxTween::easeOut, ypos, ofGetHeight(), duration / 3, delay);
    }
}

//--------------------------------------------------------------
void Dialog::currentDialogShowComplete(int &id)
{
    ofNotifyEvent(onShowCompleted, currentStateName, this);
}

//--------------------------------------------------------------
void Dialog::currentDialogHideComplete(int &id)
{
    ofRemoveListener(yposTween.end_E, this, &Dialog::currentDialogHideComplete);
    
    if (bChange)
    {
        bChange = false;
        
        currentStateName = nextStateName;
        current = states[nextStateName];
        
        ofAddListener(yposTween.end_E, this, &Dialog::currentDialogShowComplete);
        yposTween.setParameters(1, easingElastic, ofxTween::easeOut, ofGetHeight(), ofGetHeight() - current.getHeight() * 2, duration, 300);
    }
    
    ofNotifyEvent(onHideCompleted, currentStateName, this);
}

//--------------------------------------------------------------
string Dialog::getCurrentStateName()
{
    return currentStateName;
}