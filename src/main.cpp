#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
# ifdef FULL_HD_ENABLE
    ofSetupOpenGL(&window, 1920, 1080, OF_FULLSCREEN);
# else
    ofSetupOpenGL(&window, 1440, 800, OF_WINDOW);
# endif
	ofRunApp(new testApp()); // start the app
}
