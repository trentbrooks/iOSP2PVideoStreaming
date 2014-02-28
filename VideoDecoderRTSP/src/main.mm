#include "ofMain.h"
#include "ofApp.h"

int main(){
    ofAppiOSWindow * iOSWindow = new ofAppiOSWindow();
	
	//iOSWindow->enableDepthBuffer();
	//iOSWindow->enableAntiAliasing(4);
	
	if(iOSWindow->isRetinaSupportedOnDevice()) iOSWindow->enableRetina();
    
    iOSWindow->enableHardwareOrientation();
	
	ofSetupOpenGL(iOSWindow, 1024, 768, OF_FULLSCREEN);
	ofRunApp(new ofApp);
}
