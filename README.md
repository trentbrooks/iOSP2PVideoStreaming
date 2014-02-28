iOSP2PVideoStreaming
====================

Various tests/examples for P2P video streaming between iOS devices with an OF frontend.

Working so far...
 - early testing of RTSP video streaming from 1 ios device to another.
 - very loose OF wrapper's (ofxRTSPVideoDecoder and ofxRTSPVideoEncoder) for native ios rtsp video streaming examples (links below).

To do...
 - make a gstreamer example. still can't get it working with rtsp, only http. gstreamer pipelines are annoying.

Links:
RTSP decoder: https://github.com/mooncatventures-group/RtspFrames-test)
ffmpegDecoder framework: https://github.com/mooncatventures-group/ffmpegDecoder
RTSP encoder: http://www.gdcl.co.uk/2014/01/21/Video-Encoding-iOS7.html
GStreamer ios examples: https://coaxion.net/blog/2013/10/gstreamer-1-0-examples-for-ios-android-and-in-general/, http://cgit.freedesktop.org/~slomo/gst-sdk-tutorials/
GStreamer ios official: http://docs.gstreamer.com/display/GstSDK/iOS+tutorials (don't download SDK from website- it's old)