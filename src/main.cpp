#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1440

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
