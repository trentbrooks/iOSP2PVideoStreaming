#pragma once

#include "ofMain.h"
#include "TCPPixelsEncoder.h"
#include "TCPPixelsDecoder.h"

//#define IS_SERVER

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
#ifdef IS_SERVER
    ofVideoGrabber* video;
    TCPPixelsEncoder server;
#else
    TCPPixelsDecoder client;
#endif
    
    ofTexture* texture;
		
};
