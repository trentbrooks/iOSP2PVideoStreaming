#include "ofxRTSPVideoDecoder.h"


ofxRTSPVideoDecoder::ofxRTSPVideoDecoder(){

    processTime = 0;
    sleepTime = 16;
    isFrameNew = false;
}

ofxRTSPVideoDecoder::~ofxRTSPVideoDecoder(){
    waitForThread();
}

void ofxRTSPVideoDecoder::setup(string rtpStream, int outputWidth, int outputHeight) {

    NSString* nsRtpStream = ofxStringToNSString(rtpStream);
    delegate = [[RTSPDecoderDelegate alloc] init:	this withURL:nsRtpStream withWidth:outputWidth withHeight:outputHeight];
    
    // add a container view
    videoView = [[UIImageView alloc] init];
    //[videoView setTransform:CGAffineTransformMakeRotation(M_PI/2)];
    videoView.frame = CGRectMake(0, 0, outputWidth, outputHeight);//ofGetWidth() * .5, ofGetHeight() * .5); // retina sizes are ignored
    //[videoView.layer addSublayer:preview];
    
    [ofxiPhoneGetGLParentView() addSubview:videoView];
    
    ofxiOSSetGLViewTransparent(true);
    ofxiPhoneSendGLViewToFront();
    
    startThread(); // true, false = blocking, verbose by default
}

// set the x,y position
void ofxRTSPVideoDecoder::setPosition(int x, int y) {

    videoView.frame = CGRectMake(x, y, videoView.frame.size.width, videoView.frame.size.height);
    
}

// set the width, height
void ofxRTSPVideoDecoder::setSize(int w, int h) {
    
    videoView.frame = CGRectMake(videoView.frame.origin.x, videoView.frame.origin.y, w, h);
    
}

//--------------------------------------------------------------
void ofxRTSPVideoDecoder::threadedFunction(){
    
    while( isThreadRunning() ){
        
        int startTime = ofGetElapsedTimeMillis();

        if(isFrameNew) {
            [[delegate video] stepFrame];
            isFrameNew = false;
        }
        
        // a seperate mutex for the timer/fps
        timerMutex.lock();
        processTime = ofGetElapsedTimeMillis() - startTime;
        int diff = sleepTime - processTime; //processTime- 16 = 60fps, 33 = 30fps
        if(diff < 1) diff = 1;
        timerMutex.unlock();
        
        
        sleep(diff);
    }
}

void ofxRTSPVideoDecoder::onFrameExtracted(UIImage* image) {
    
    videoView.image = image;
    
    mutex.lock();
    isFrameNew = true;
    mutex.unlock();
}

float ofxRTSPVideoDecoder::getProcessingTime() {
    Poco::ScopedLock<ofMutex> lock(timerMutex);
    return processTime;
}

//--------------------------------------------------------------
@implementation RTSPDecoderDelegate

@synthesize video;


//--------------------------------------------------------------
- (id) init :(ofxRTSPVideoDecoder *)rCpp withURL: (NSString*)url withWidth: (int) width withHeight: (int) height{
	if(self = [super init])	{
		NSLog(@"ofxRTSPVideoDecoder initiated");
        
        // ref to OF instance
        rtspFramesDecoderCpp = rCpp;
        
        //@"rtsp://211.79.36.213/ngwild_gphone.sdp"
        self.video = [[FrameExtractor alloc] initWithVideo:url];
        video.outputWidth = width;
        video.outputHeight = height;
        
        // setting up to receive a UIImage*, can also setup for CVPixelBufferRef or CMSampleBufferRef
        self.video.cgimageDelegate = self;
        [video setupCgimageSession];
        
        // print some info about the video
        NSLog(@"video duration: %f",video.duration);
        NSLog(@"video size: %d x %d", video.sourceWidth, video.sourceHeight);
        
	}
	return self;
}

-(void)didOutputCGImageBuffer:(NSTimer *)timer {
    //[video stepFrame];
  	//imageView.image = video.currentImage;
    //NSLog(@"hi");
    
    //[video stepFrame];
    //rtspFramesDecoderCpp->videoView.image = video.currentImage;
    rtspFramesDecoderCpp->onFrameExtracted(video.currentImage);
}

@end