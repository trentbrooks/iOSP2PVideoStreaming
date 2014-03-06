#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    //ofSetFrameRate(60);
    ofSetVerticalSync(true);
    //ofEnableAlphaBlending();
    
    ofSetOrientation(OF_ORIENTATION_90_RIGHT); // if i don't set landscape, width + height are inverted in AVFoundationGrabber getPixels()
    ofBackground(0);
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    // setup a standard video grabber.
    // sending/receiving buffer only works with AVFoundationVideoGrabber default sizes: 192x144, 480x360, 640x480, 1280x720
    int width = 192;//192;
    int height = 144;//144;
    video = new ofVideoGrabber();
    video->setDesiredFrameRate(30);
    video->initGrabber(width, height);//, false);
    
    
    // setup a TCP encoder server on seperate thread
    server.setup(4321,width,height); // width/height is inverted here if i don't set landscape orientation
    
    // testing the output image...
    texture = new ofTexture();
    texture->allocate(width, height, GL_RGB);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    video->update();
    
    if (video->isFrameNew() ){
        
        // write the pixels for sending
        server.writePixels(video->getPixels());// video->getPixelsRef());
        //server.writePixels(video->getPixelsRef());
        
        
        // test view the compressed buffer
        //ofBuffer buffer = server.getBuffer();
        //ofLoadImage(*texture, buffer); // slow
        
        
        // more tests... ofSaveImage behaving differently to non threaded??? only happening on single core iphone 4
        /*ofPixels pixels;
        pixels.setFromPixels(video->getPixels(), video->getWidth(), video->getHeight(), OF_IMAGE_COLOR);
        ofBuffer compressedBuffer;
        float s = ofGetElapsedTimeMillis();
        ofSaveImage(pixels, compressedBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM); //
        ofLog() << (ofGetElapsedTimeMillis() - s);*/

    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofSetColor(255);
    video->draw(0, 0, 320, 240);//, 160, 120);
    //texture->draw(161, 0, 160, 120);//video->getHeight());
    
    stringstream output;
    output << "TCPPixelsEncoder demo " << ofGetFrameRate() << endl;
    
    // adding all these slows things down due to thread locks, comment out when not debugging
    output << "connected: " << server.isConnected() << endl;
    output << "clients: " << server.getNumClients() << endl;
    output << "encoder processing time: " << server.getProcessingTime() << endl;
    output << "buffer size: " << server.getBuffer().size() << endl;
    output << "frames sent: " << server.getFramesSent() << endl;
    ofDrawBitmapStringHighlight(output.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
