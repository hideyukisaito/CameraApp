#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

#define SCREEN_WIDTH 873
#define SCREEN_HEIGHT 1396

//========================================================================
int main()
{

    ofAppGlutWindow window;
    ofSetupOpenGL(&window, SCREEN_WIDTH, SCREEN_HEIGHT, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
