

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#import "CameraServer.h"


/*
 ofxRTSPVideoEncoder.
 - based on GDCL's example http://www.gdcl.co.uk/2014/01/21/Video-Encoding-iOS7.html
 */

class ofxRTSPVideoEncoder {
public:
    
    ofxRTSPVideoEncoder();
    virtual ~ofxRTSPVideoEncoder();
    
    void setup(int width=320, int height=240, int fps=15);
    string getIpAddress();
    
    // display
    void setPosition(int x, int y);
    void setSize(int w, int h);
    
protected:
    
    string ipAddress;
    CameraServer* server;
    UIView* videoView;
    AVCaptureVideoPreviewLayer* preview;
};