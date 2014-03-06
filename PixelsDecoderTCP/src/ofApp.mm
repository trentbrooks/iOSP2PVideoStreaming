#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    //ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetOrientation(OF_ORIENTATION_90_RIGHT); // if i don't set landscape, width + height are inverted in AVFoundationGrabber getPixels()
    ofBackground(0);
    ofLog() << ofGetUsingArbTex();
    
    // setup a standard video grabber.
    // sending/receiving buffer only works with AVFoundationVideoGrabber default sizes: 192x144, 480x360, 640x480, 1280x720
    int width = 192;
    int height = 144;
    int port = 4321;
    client.setup("10.0.1.131", port, width, height); // width/height is inverted here if i don't set landscape orientation
    
    texture = new ofTexture();
    texture->allocate(width, height, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){

    client.readToTexture(texture);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofSetColor(255);
    texture->draw(0,0, 320, 240);
    
    stringstream output;
    output << "TCPPixelsDecoder demo " << ofGetFrameRate() << endl;
    
    // adding all these slows things down due to thread locks, comment out when not debugging
    output << "connected: " << client.isConnected() << endl;
    output << "decoder processing time: " << client.getProcessingTime() << endl;
    output << "buffer size: " << client.getReceivedSize() << endl;
    output << "frames received " << client.getFramesReceived() << endl;
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
