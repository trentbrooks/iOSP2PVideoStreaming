#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	

    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofBackground(0,0);
    
    rtspDecoder = new ofxRTSPVideoDecoder();
    // rtsp://211.79.36.213/ngwild_gphone.sdp
    rtspDecoder->setup("rtsp://10.0.1.134", 320, 240);
}

//--------------------------------------------------------------
void ofApp::update(){

    // need to add methods for reconnection, etc
}

//--------------------------------------------------------------
void ofApp::draw(){
	
    stringstream output;
    output << "RTSP Decoder Example: " << ofGetFrameRate() << endl;
    output << "Decode process time (thread): " << rtspDecoder->getProcessingTime() << endl;
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
