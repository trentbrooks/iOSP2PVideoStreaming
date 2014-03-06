
#pragma  once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "TCPPixelsUtils.h"

/* reference:
 - encoder / server
 - http://forum.openframeworks.cc/index.php/topic,603.0.html
 - http://forum.openframeworks.cc/index.php/topic,3505.0.html
 - //theos pixel server?
 - todo: crash on client disconnect
 - those bugs in ofNetwork
 */


class TCPPixelsEncoder : public ofThread {
public:
    
    TCPPixelsEncoder();
    virtual ~TCPPixelsEncoder();

    void setup(int port, int pixelsWidth, int pixelsHeight);
    bool isConnected();
    
    int getProcessingTime();
    int getNumClients();
    
    // set pixels from video grabber
    void writePixels(ofPixelsRef px);
    void writePixels(unsigned char *data);
    
    // pixels are compressed into an ofBuffer- use this for debug view
    ofBuffer getBuffer();    
    
    int getFramesSent();
        
protected:
    
    bool connected;
    bool blocking;
    
    void sendData(); // main update function for encoder
    
    // thread
    void threadedFunction();
    int sleepTime, processTime;
    ofMutex timerMutex; // mutex just so we can monitor how long processTime takes
    
    // pixels + buffer
    ofPixels pixels;
    bool isBufferCompressed;
    ofBuffer buffer;
    int framesSent;
    
    // tcp settings
    ofxTCPServer* server;
    int numClients;
    int port;
    
    // ofSaveImage() encode options
    ofImageFormat format;
    ofImageQualityType quality;
};

