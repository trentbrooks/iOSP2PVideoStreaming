
#include "TCPPixelsEncoder.h"


TCPPixelsEncoder::TCPPixelsEncoder(){

    connected = false;
    blocking = false;
    sleepTime = 16;
    processTime = 0;
    numClients = 0;
    format = OF_IMAGE_FORMAT_JPEG;
    quality = OF_IMAGE_QUALITY_HIGH;
    isBufferCompressed = false;
    framesSent = 0;
    server = NULL;
    
}

TCPPixelsEncoder::~TCPPixelsEncoder(){

    ofLog() << "closing TCPPixelsEncoder";

    waitForThread();
    if(server != NULL) {
        if(server->isConnected()) server->close();
    }
}


void TCPPixelsEncoder::setup(int port, int pixelsWidth, int pixelsHeight) {
    
    this->port = port;
    
    // have to preallocate the ofPixels for some reason
    pixels.allocate(pixelsWidth, pixelsHeight, OF_PIXELS_RGB);//OF_IMAGE_COLOR);
    
    server = new ofxTCPServer();
    connected = server->setup(port, blocking);
    
    startThread(); // bool blocking = true, bool verbose = false
}


//--------------------------
void TCPPixelsEncoder::threadedFunction(){
    
    while(isThreadRunning() != 0) {
        
        int startTime = ofGetElapsedTimeMillis(); // start timer
        
        //mutex.lock();
        sendData();
        //mutex.unlock();
        
        // a seperate mutex for the timer/fps
        timerMutex.lock();
        processTime = ofGetElapsedTimeMillis() - startTime;
        int diff = sleepTime - processTime; //processTime- 16 = 60fps, 33 = 30fps
        if(diff < 1) diff = 1;
        timerMutex.unlock();
        
        sleep(diff); // or (sleepTime);
    }
}

void TCPPixelsEncoder::sendData() {
    
    if(!server->isConnected() && ofGetFrameNum() % 300 == 0) {
        
        ofLog() << "Server not connected, trying to reconnect...";
        connected = server->setup(port, blocking);
        
    } else if(connected) {
        
        // use the mutex lock/unlock to copy the pixels + buffer only
        // read/write style buffers
        mutex.lock();
        if(!pixels.isAllocated()) {
            mutex.unlock();
            return;
        }
        ofPixels bPx = pixels;
        ofBuffer bBuf = buffer;//(buffer);
        bool bIsBufferCompressed = isBufferCompressed;
        int bFramesSent = framesSent;
        int bNumClients = 0;
        mutex.unlock();
        
        // compress to video buffer on new frame- test different quality settings- this takes the longest
        // bottleneck is here when video is bigger than 192x144 on ios
        if(!bIsBufferCompressed) {
            //mutex.lock();
            ofSaveImage(bPx, bBuf, format, quality);
            bIsBufferCompressed = true;
        }
        //mutex.unlock();
        
        
        //mutex.unlock();
        //mutex.lock();
        
        //numClients = 0; //server->getNumClients();
        for(int i = 0; i < server->getLastID(); i++){
            
            if( !server->isClientConnected(i) )continue;
            bNumClients++;
            
            // receive a ping from a client, send back a pong with the size of the video buffer
            ofBuffer pingBuffer;
            bool pingReceived = TCPPixelsUtils::receiveRawBuffer(server, i, pingBuffer, TCPPIXELS_SMALL_BUFFER_SIZE);
            if(pingReceived) {
                
                string pingMessage = TCPPixelsUtils::parse(pingBuffer);
                if(pingMessage == TCPPIXELS_PING_MESSAGE) {
                    
                    //ofLogVerbose() << "Received: " << pingMessage << ", " << pingMessage.size() << ", " << pingBuffer.size();
                    
                    // send the pong, which is a header with the size of the video buffer
                    string p = ofToString(bBuf.size());
                    bool pongSent = TCPPixelsUtils::sendRawString(server, i, p, TCPPIXELS_SMALL_BUFFER_SIZE);
                    if(pongSent) {
                        //ofLogVerbose() << "Sent raw pong bytes from server... " << p;
                        
                        // now send the video buffer...
                        //string v = "videostuffhere";
                        //bool videoSent = TCPPixelsUtils::sendRawString(server, i, v, 10028);
                        bool videoSent = TCPPixelsUtils::sendRawBuffer(server, i, bBuf, bBuf.size());
                        if(videoSent) {
                            //ofLogVerbose() << "Sent raw video bytes from server... " << bBuf.size();
                            bFramesSent++;
                        }
                        
                        
                    }
                } else {
                    
                    ofLog() << "Error: unknown ping received: " << pingMessage;
                }
                
            }
        }
        
        
        // copy the written vars back
        // TODO: fix this like the decoder
        mutex.lock();
        isBufferCompressed = bIsBufferCompressed;
        numClients = bNumClients;
        if(bFramesSent > framesSent) {
            buffer = bBuf;
            framesSent = bFramesSent;
        }
        mutex.unlock();
    }
    
}


int TCPPixelsEncoder::getProcessingTime() {
    Poco::ScopedLock<ofMutex> lock(timerMutex);
    return processTime;
}


void TCPPixelsEncoder::writePixels(ofPixelsRef px) {
    
    // this might be causing the problems... need to display the compressed image to see
    //if(mutex.tryLock()) {
        mutex.lock();
        pixels = px; // copy pixels
        isBufferCompressed = false;
        mutex.unlock();
    //}
}

void TCPPixelsEncoder::writePixels(unsigned char *data) {
    
    // this might be causing the problems... need to display the compressed image to see
    //if(mutex.tryLock()) {
        mutex.lock();
        pixels.setFromPixels(data, pixels.getWidth(), pixels.getHeight(), pixels.getNumChannels());
        isBufferCompressed = false;
        mutex.unlock();
    //}
}

ofBuffer TCPPixelsEncoder::getBuffer() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return buffer;
}

int TCPPixelsEncoder::getNumClients() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return numClients;
}

bool TCPPixelsEncoder::isConnected() {
    return connected;
}

int TCPPixelsEncoder::getFramesSent() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return framesSent;
}

