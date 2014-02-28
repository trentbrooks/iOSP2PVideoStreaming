//
//  CameraServer.m
//  Encoder Demo
//
//  Created by Geraint Davies on 19/02/2013.
//  Copyright (c) 2013 GDCL http://www.gdcl.co.uk/license.htm
//

#import "CameraServer.h"
#import "AVEncoder.h"
#import "RTSPServer.h"

static CameraServer* theServer;

@interface CameraServer  () <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureSession* _session;
    AVCaptureVideoPreviewLayer* _preview;
    AVCaptureVideoDataOutput* _output;
    dispatch_queue_t _captureQueue;
    
    AVEncoder* _encoder;
    
    RTSPServer* _rtsp;
}
@end


@implementation CameraServer

+ (void) initialize
{
    // test recommended to avoid duplicate init via subclass
    if (self == [CameraServer class])
    {
        theServer = [[CameraServer alloc] init];
    }
}

+ (CameraServer*) server
{
    return theServer;
}


/*
 NSString *const AVCaptureSessionPresetPhoto;
 NSString *const AVCaptureSessionPresetHigh;
 NSString *const AVCaptureSessionPresetMedium;
 NSString *const AVCaptureSessionPresetLow;
 NSString *const AVCaptureSessionPreset320x240;
 NSString *const AVCaptureSessionPreset352x288;
 NSString *const AVCaptureSessionPreset640x480;
 NSString *const AVCaptureSessionPreset960x540;
 NSString *const AVCaptureSessionPreset1280x720;
 NSString *const AVCaptureSessionPresetiFrame960x540;
 NSString *const AVCaptureSessionPresetiFrame1280x720;
 //192x144, 720x480, 640x360, 480x360
 
 NSString * preset = AVCaptureSessionPresetMedium;
 width	= 480;
 height	= 360;
 
 if(ofxiOSGetDeviceRevision() == ofxiOS_DEVICE_IPHONE_3G) {
 width = 400;
 height = 304;
 }
 else {
 if( w == 640 && h == 480 ){
 preset = AVCaptureSessionPreset640x480;
 width	= w;
 height	= h;
 }
 else if( w == 1280 && h == 720 ){
 preset = AVCaptureSessionPreset1280x720;
 width	= w;
 height	= h;
 }
 else if( w == 192 && h == 144 ){
 preset = AVCaptureSessionPresetLow;
 width	= w;
 height	= h;
 }
 }
 [self.captureSession setSessionPreset:preset];
 
 */


- (void) startup
{
    if (_session == nil)
    {
        NSLog(@"Starting up server");
        
        // create capture device with video input
        _session = [[AVCaptureSession alloc] init];
        AVCaptureDevice* dev = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        AVCaptureDeviceInput* input = [AVCaptureDeviceInput deviceInputWithDevice:dev error:nil];
        [_session addInput:input];
        
        // TB: adjust the internal capture preset (default is 720p on iphone 4)
        [_session setSessionPreset:AVCaptureSessionPreset640x480];//]AVCaptureSessionPresetLow];
        
        // create an output for YUV output with self as delegate
        _captureQueue = dispatch_queue_create("uk.co.gdcl.avencoder.capture", DISPATCH_QUEUE_SERIAL);
        _output = [[AVCaptureVideoDataOutput alloc] init];
        [_output setSampleBufferDelegate:self queue:_captureQueue];
        NSDictionary* setcapSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange], kCVPixelBufferPixelFormatTypeKey,
                                        nil];
        _output.videoSettings = setcapSettings;
        _output.alwaysDiscardsLateVideoFrames = YES;
        [_session addOutput:_output];
        
        // TB: adjust FPS
        AVCaptureConnection *conn = [_output connectionWithMediaType:AVMediaTypeVideo];
        if ([conn isVideoMinFrameDurationSupported] && [conn isVideoMaxFrameDurationSupported]){
            [conn setVideoMinFrameDuration:CMTimeMake(1, 15)];
            [conn setVideoMaxFrameDuration:CMTimeMake(1, 15)];
        }
        
        // create an encoder
        _encoder = [AVEncoder encoderForHeight:480 andWidth:640];
        [_encoder encodeWithBlock:^int(NSArray* data, double pts) {
            if (_rtsp != nil)
            {
                _rtsp.bitrate = _encoder.bitspersecond;
                [_rtsp onVideoData:data time:pts];
            }
            return 0;
        } onParams:^int(NSData *data) {
            _rtsp = [RTSPServer setupListener:data];
            return 0;
        }];
        
        // start capture and a preview layer
        [_session startRunning];
        
        
        _preview = [AVCaptureVideoPreviewLayer layerWithSession:_session];
        _preview.videoGravity = AVLayerVideoGravityResizeAspectFill;
    }
}

- (void) captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    // pass frame to encoder
    [_encoder encodeFrame:sampleBuffer];
}

- (void) shutdown
{
    NSLog(@"shutting down server");
    if (_session)
    {
        [_session stopRunning];
        _session = nil;
    }
    if (_rtsp)
    {
        [_rtsp shutdownServer];
    }
    if (_encoder)
    {
        [ _encoder shutdown];
    }
}

- (NSString*) getURL
{
    NSString* ipaddr = [RTSPServer getIPAddress];
    NSString* url = [NSString stringWithFormat:@"rtsp://%@/", ipaddr];
    return url;
}

- (AVCaptureVideoPreviewLayer*) getPreviewLayer
{
    return _preview;
}

@end
