
#include "TCPPixelsDecoder.h"


TCPPixelsDecoder::TCPPixelsDecoder(){
    
    connected = false;
    blocking = true;
    isFrameNew = false;
    sleepTime = 16;
    receivedBytes = 0;
    processTime = 0;
    framesReceived = 0;
    client = NULL;
}

TCPPixelsDecoder::~TCPPixelsDecoder(){
    
    ofLog() << "closing TCPPixelsDecoder";
    
    waitForThread();
    if(client != NULL) {
        if(client->isConnected()) client->close();
    }
}


void TCPPixelsDecoder::setup(string host, int port, int pixelsWidth, int pixelsHeight) {
    
    this->host = host;
    this->port = port;
    
    // have to preallocate the ofPixels for some reason
    pixels.allocate(pixelsWidth, pixelsHeight, OF_PIXELS_RGB);
    
    client = new ofxTCPClient();
    connected = client->setup(host, port, blocking);
    
    startThread(); // bool blocking = true, bool verbose = false
}


//--------------------------
void TCPPixelsDecoder::threadedFunction(){
    
    while(isThreadRunning() != 0) {
        
        int startTime = ofGetElapsedTimeMillis(); // start timer
        
        receiveData();
        
        // a seperate mutex for the timer/fps
        timerMutex.lock();
        processTime = ofGetElapsedTimeMillis() - startTime;
        int diff = sleepTime - processTime; //processTime- 16 = 60fps, 33 = 30fps
        if(diff < 1) diff = 1;
        timerMutex.unlock();
        
        sleep(diff); // or (sleepTime);
    }
}



void TCPPixelsDecoder::receiveData() {
    
    if(!client->isConnected() && ofGetFrameNum() % 300 == 0) {
        cout << "client not connected, let's try reconnect" << endl;
        connected = client->setup(host, port, blocking);
        
    } else if(connected) {
        
        
        // get by pixel row- store in pixels
        /*char* receiveBuffer = (char*) pixels.getPixels();
         int totalReceivedBytes = 0;
         while (totalReceivedBytes < dataSize) {
         int receivedBytes = client->receiveRawBytes(receiveBuffer, sendSize); //returns received bytes
         totalReceivedBytes += receivedBytes;
         receiveBuffer += receivedBytes;
         }*/
        
        
        // use the mutex lock/unlock to copy the pixels + buffer only
        // read/write style buffers
        int bReceivedBytes = 0;
        mutex.lock();
        int bFramesReceived = framesReceived;
        bool bIsFrameNew = isFrameNew;
        ofPixels px = pixels;
        ofBuffer videoBuffer;
        mutex.unlock();
        
        
        
        // send a ping request / handshake to the server
        string p = TCPPIXELS_PING_MESSAGE;
        bool pingSent = TCPPixelsUtils::sendRawString(client, p, TCPPIXELS_SMALL_BUFFER_SIZE); // appends " " to a simple string message and pads with junk to fill needed size
        if(pingSent) {
            
            //ofLogVerbose() << "Sent raw ping bytes from client...";
            
            // receive a pong with the size of the next message (header)
            ofBuffer pongBuffer; // buffer to be filled
            bool pongReceived = TCPPixelsUtils::receiveRawBuffer(client, pongBuffer, TCPPIXELS_SMALL_BUFFER_SIZE);
            if(pongReceived) {
                
                string pongMessage = TCPPixelsUtils::parse(pongBuffer);
                ofLogVerbose() << "Received pong: " << pongMessage << ", " << pongMessage.size();// << ", " << pongResult;
                
                // receive the video buffer
                int incomingBufferSize = ofToInt(pongMessage);
                //ofBuffer videoBuffer; // buffer to be filled (moved up top now)
                bool videoReceived = TCPPixelsUtils::receiveRawBuffer(client, videoBuffer, incomingBufferSize);//10028);
                if(videoReceived) {
                    
                    //ofLogVerbose() << "Received video: " << videoBuffer.size() << ", " << incomingBufferSize;// << ", " << videoBuffer.getText().size();
                    //string videoMessage = TCPPixelsUtils::parse(videoBuffer);
                    //ofLog() << "R2: " << videoMessage << ", " << videoMessage.size();// << ", " << videoResult;
                    
                    // try load the video buffer into a ofPixels object
                    bReceivedBytes = videoBuffer.size();
                    if (ofLoadImage(px, videoBuffer)) {
                        bFramesReceived++;
                        bIsFrameNew = true;
                    }
                }
         
            }

        }
        
        // write back to buffers if frame is new
        mutex.lock();
        receivedBytes = bReceivedBytes;
        if(bIsFrameNew) {
            framesReceived = bFramesReceived;
            buffer = videoBuffer;
            pixels = px;
            isFrameNew = bIsFrameNew;
        }
        mutex.unlock();
        
    }
}

void TCPPixelsDecoder::readToTexture(ofTexture* texture) {
    
    mutex.lock();
    if(isFrameNew) {
        //if(mutex.tryLock()) {
        if(!texture->isAllocated()) texture->allocate(pixels);
        texture->loadData( pixels );
        //}
        isFrameNew = false;
    }
    mutex.unlock();
    //ofLoadImage(*texture, videoBuffer);
}

int TCPPixelsDecoder::getProcessingTime() {
    Poco::ScopedLock<ofMutex> lock(timerMutex);
    return processTime;
}

ofBuffer TCPPixelsDecoder::getBuffer() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return buffer;
}

int TCPPixelsDecoder::getReceivedSize() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return receivedBytes;
}

bool TCPPixelsDecoder::isConnected() {
    return connected;
}

int TCPPixelsDecoder::getFramesReceived() {
    Poco::ScopedLock<ofMutex> lock(mutex);
    return framesReceived;
}


