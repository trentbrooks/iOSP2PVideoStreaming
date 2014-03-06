
#pragma  once

#include "ofMain.h"
#include "ofxNetwork.h"
#include <string>

/*
 TCPPixelsUtils.
 - util functions for encoder and decoder
 - need to fix crashing on exit/disconnection in while loops send/receive
 */

// small buffer size is for plain text messages- eg. "ping", or the header size "12345678"
#define TCPPIXELS_SMALL_BUFFER_SIZE 8
#define TCPPIXELS_PING_MESSAGE "ping"

class TCPPixelsUtils  {
public:
    
    //TCPPixelsUtils() {};
    //virtual ~TCPPixelsUtils() {};

    
    
    // parse incoming request buffer into a string
    // extract the important bit only (everything up to the first " ")
    static string parse(ofBuffer& input) {
        
        stringstream ss;
        ss << input; // input buffer inserted into stringstream
        string buffer;
        ss >> buffer; // extract first part of message only (up until " ")
        return buffer;
    }
    
    // construct string padded to required size
    // delimiter splits the message- left side is the data, right side is junk
    // eg. "ping xxxxxxxxjunkxxxxxxxx"
    // TODO: maybe have a temp buffer to store the extra bytes on the receiver instead of discarding?
    static string build(string& output, int fillSize) {
        
        stringstream ss;
        ss << output << " "; // must have the " " on the end for stringstream delimeter
        string buffer;
        buffer.assign(ss.str().c_str(),fillSize);
        return buffer;
    }
    
    // helper method when receiving a buffer for client- ensures whole buffer is full
    static bool receiveRawBuffer(ofxTCPClient* client, ofBuffer& buffer, int receiveSize) {
        
        buffer.allocate(receiveSize+1); // don't know why i have to add an extra byte to ofBuffer
        
        // while loop to get all data for client
        // TODO: have an exit in case we receive a 0 result multiple times
        int dataReceived = 0;
        while(dataReceived < receiveSize) {
            
            //int result = client->receiveRawBytes(buffer.getBinaryBuffer(), receiveSize-dataReceived);//(i);
            int result = client->receiveRawBytes(&buffer.getBinaryBuffer()[dataReceived], receiveSize-dataReceived);
            if(result > 0) {
                dataReceived += result;
            } else if(result == -1) {
                ofLogVerbose() << "Error receiving raw bytes client. Bytes:" << dataReceived;
                return false;
            }
        }
        
        return true;
        
        // simple testing- not checking size here- assume all received in 1 go.
        /*int result = client->receiveRawBytes(buffer.getBinaryBuffer(), buffer.size());
        if(result > 0) {
            return true;
        } else if(result == -1) {
            ofLog() << "error recieving raw bytes client";
            return false;
        }
        return false;*/
    }

    
    // construct outgoing request
    // buffer is padded to the required size (with junk)
    // delimiter " " splits the message- left side is the data, right side is junk
    // or have a temp buffer to store the extra bytes on the receiver? todo...
    // eg. "ping xxxxxxxxjunkxxxxxxxx"
    static bool sendRawString(ofxTCPClient* client, string& output, int fillSize) {
        
        string buffer = TCPPixelsUtils::build(output, fillSize);
        
        bool sent = client->sendRawBytes(buffer.c_str(), buffer.size());
        return sent;
    }

    
    
    
    // -----
    // helper method when receiving a buffer for server- ensures whole buffer is full
    // check same method for client, still testing
    static bool receiveRawBuffer(ofxTCPServer* server, int clientId, ofBuffer& buffer, int receiveSize) {
        
        buffer.allocate(receiveSize+1); // don't know why i have to add an extra byte to ofBuffer
        
        // while loop to get all data for client
        // TODO: have an exit in case we receive a 0 result multiple times
        int dataReceived = 0;
        while(dataReceived < receiveSize) {
            
            //int result = client->receiveRawBytes(buffer.getBinaryBuffer(), receiveSize-dataReceived);//(i);
            int result = server->receiveRawBytes(clientId, &buffer.getBinaryBuffer()[dataReceived], receiveSize-dataReceived);
            if(result > 0) {
                dataReceived += result;
            } else if(result == -1) {
                
                // if server is not blocking, this will get called a lot.
                ofLogVerbose() << "Error receiving raw bytes server. Bytes:" << dataReceived;
                return false;
            }
        }
        
        return true;
        
        // testing- not checking size here
        /*int result = server->receiveRawBytes(clientId, buffer.getBinaryBuffer(), buffer.size());
        if(result > 0) {
            return true;
        } else if(result == -1) {
            ofLogError() << "Error: recieving raw bytes server";
            return false;
        }
        
        // 0 bytes?
        return false;*/
    }
    
    // same as client method
    // creates a new string buffer from the output padded to correct size
    static bool sendRawString(ofxTCPServer* server, int clientId, string& output, int fillSize) {
        
        string buffer = TCPPixelsUtils::build(output, fillSize);
        
        bool sent = server->sendRawBytes(clientId, buffer.c_str(), buffer.size());
        return sent;
    }
    
    // send a raw ofBuffer - must be filled to correct size
    static bool sendRawBuffer(ofxTCPServer* server, int clientId, ofBuffer& buffer, int bufferSize) {
        
        bool sent = server->sendRawBytes(clientId, buffer.getBinaryBuffer(), bufferSize);
        return sent;
    }
};

