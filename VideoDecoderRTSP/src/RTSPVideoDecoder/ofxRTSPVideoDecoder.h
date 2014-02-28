#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#import <UIKit/UIKit.h>
#import <FFmpegDecoder/FrameExtractor.h>


/*
 ofxRTSPVideoDecoder.
 - example based on mooncatventures-group RtspFrames-test example (https://github.com/mooncatventures-group/RtspFrames-test)
 - uses ffmpeg (FFmpegDecoder framework also from mooncatventures-group)
 - only works on device (not simulator)
 */

class ofxRTSPVideoDecoder;

//--------------------------------------------------------------
@interface RTSPDecoderDelegate : NSObject <CGImageBufferDelegate> {
    
    ofxRTSPVideoDecoder* rtspFramesDecoderCpp;
    FrameExtractor *video;
}

@property (nonatomic, retain) FrameExtractor *video;
- (id) init:(ofxRTSPVideoDecoder *)rCpp withURL: (NSString*)url withWidth: (int) width withHeight: (int) height;
@end


//--------------------------------------------------------------
class ofxRTSPVideoDecoder : public ofThread {
public:
    
    ofxRTSPVideoDecoder();
    virtual ~ofxRTSPVideoDecoder();
    
    
    void setup(string rtpStream, int outputWidth=320, int outputHeight=240);
    void threadedFunction();
    float getProcessingTime();
    
    
    // ios delegate
    RTSPDecoderDelegate* delegate;
    UIImageView* videoView;
    
    // callback
    void onFrameExtracted(UIImage* image);
    bool isFrameNew;
    
    // display
    void setPosition(int x, int y);
    void setSize(int w, int h);
    
protected:
    
    int sleepTime, processTime;
    ofMutex timerMutex; // mutex just so we can monitor how long processTime takes
    
};