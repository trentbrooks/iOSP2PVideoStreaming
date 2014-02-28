#include "ofxRTSPVideoEncoder.h"


ofxRTSPVideoEncoder::ofxRTSPVideoEncoder(){
    
    ipAddress = "";
}

ofxRTSPVideoEncoder::~ofxRTSPVideoEncoder(){
    
    ofLogVerbose() << "Closing ofxRTSPVideoEncoder.";
}

//int width, int height, int fps
void ofxRTSPVideoEncoder::setup(int width, int height, int fps) {

    [[CameraServer server] startup];

    
    // get the preview layer and add it as a subview of the OF/opengl window
    preview = [[CameraServer server] getPreviewLayer];
    [preview removeFromSuperlayer];
    preview.frame = CGRectMake(0, 0, width, height); // retina sizes are ignored
    //[[preview connection] setVideoOrientation:UIInterfaceOrientationPortrait];
    
    videoView = [[UIView alloc] init];
    //videoView.frame = CGRectMake(0, 0, width*.5, height*.5); // retina sizes are ignored
    [videoView.layer addSublayer:preview];
    [ofxiPhoneGetGLParentView() addSubview:videoView];
    
    //preview.frame = videoView.bounds;
    // make the OF transparent and bring to front
    // ofBackground must be set to (0,0)
    ofxiOSSetGLViewTransparent(true);
    ofxiPhoneSendGLViewToFront();
    
    // get ip address
    ipAddress = ofxNSStringToString( [[CameraServer server] getURL] );
}

// set the x,y position
void ofxRTSPVideoEncoder::setPosition(int x, int y) {
    
    preview.frame = CGRectMake(x, y, preview.frame.size.width, preview.frame.size.height);
    
}

// set the width, height
void ofxRTSPVideoEncoder::setSize(int w, int h) {
    
    preview.frame = CGRectMake(preview.frame.origin.x, preview.frame.origin.y, w, h);
    
}


string ofxRTSPVideoEncoder::getIpAddress() {
    
    return ipAddress;
}