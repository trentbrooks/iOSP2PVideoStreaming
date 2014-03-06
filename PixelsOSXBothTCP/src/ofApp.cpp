#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofBackground(0);
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
    
    
    // setup a standard video grabber
    int width = 192;//320;
    int height = 144;//240;
    int port = 4321;
    
#ifdef IS_SERVER
    // setup a TCP encoder server on seperate thread
    server.setup(port,width,height);
    video = new ofVideoGrabber();//AVFoundationVideoGrabber();
    video->initGrabber(width, height);
    ofLog() << video->getPixelFormat();
#else
    client.setup("10.0.1.134", port, width, height);
    //client.setup("127.0.0.1", port, width, height);
#endif
    
    texture = new ofTexture();
    texture->allocate(width, height, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef IS_SERVER
    video->update();
    if (video->isFrameNew() ){
        
        // write the pixels for sending
        server.writePixels(video->getPixels());//Ref());
        
        // test view the compressed buffer
        // only works if client is connected..
        if(server.getNumClients() > 0) {
            ofBuffer buffer = server.getBuffer();
            ofLoadImage(*texture, buffer);
        }
        
    }
#else
    client.readToTexture(texture);
#endif
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
#ifdef IS_SERVER
    video->draw(0,0, 640, 480);
    if(server.getNumClients() > 0) {
        texture->draw(641, 0, 640, 480); // draw the compressed video as well
    }
#else
    texture->draw(0, 0, 640, 480);
#endif
    
    stringstream output;
    #ifdef IS_SERVER
    output << "TCPPixelsEncoder OSX demo " << ofGetFrameRate() << endl;
    
    // adding all these slows things down due to thread locks, comment out when not debugging
    output << "connected: " << server.isConnected() << endl;
    output << "clients: " << server.getNumClients() << endl;
    output << "encoder processing time: " << server.getProcessingTime() << endl;
    output << "buffer size: " << server.getBuffer().size() << endl;
    output << "frames sent: " << server.getFramesSent() << endl;
#else
    output << "TCPPixelsDecoder OSX demo " << ofGetFrameRate() << endl;
    
    // adding all these slows things down due to thread locks, comment out when not debugging
    output << "connected: " << client.isConnected() << endl;
    output << "decoder processing time: " << client.getProcessingTime() << endl;
    output << "buffer size: " << client.getReceivedSize() << endl;
    output << "frames received " << client.getFramesReceived() << endl;
#endif
    
    ofDrawBitmapStringHighlight(output.str(), 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
