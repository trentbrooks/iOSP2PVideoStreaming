
#pragma  once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "TCPPixelsUtils.h"

/* reference:
 - decoder / client
 - http://forum.openframeworks.cc/index.php/topic,603.0.html
 - http://forum.openframeworks.cc/index.php/topic,3505.0.html
 - //theos pixel server?
 */

class TCPPixelsDecoder : public ofThread {
public:
    
    TCPPixelsDecoder();
    virtual ~TCPPixelsDecoder();
    
    void setup(string host, int port, int pixelsWidth, int pixelsHeight);
    bool isConnected();

    int getProcessingTime();
    
    // pixels
    void readToTexture(ofTexture* texture);
    
    // pixels are compressed into an ofBuffer- use this for debug view
    ofBuffer getBuffer();
    int getReceivedSize();
    
    
    int getFramesReceived();
    
protected:
    
    bool connected;
    bool blocking;
    
    
    void receiveData(); // main update function for decoder
    
    // thread
    void threadedFunction();
    int sleepTime, processTime;
    ofMutex timerMutex; // mutex just so we can monitor how long processTime takes
    
    // pixels
    int receivedBytes;
    bool isFrameNew;
    int framesReceived;
    ofBuffer buffer;
    ofPixels pixels;
    char tempBuffer[512];
    
    
    // tcp settings
    ofxTCPClient* client;
    int port;
    string host;
    
};

