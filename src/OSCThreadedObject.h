//
//  OSCThreadedObject.h
//  DieHard
//
//  Created by Koichiro Mori on 2012/11/06.
//
//

#ifndef DieHard_OSCThreadedObject_h
#define DieHard_OSCThreadedObject_h

#define PORT 5000

#import "ofmain.h"
#include "ofxOsc.h"

class OSCThreadedObject : public ofThread {
private:
    ofxOscReceiver receiver;
    string currentMessage;
    
public:
    
    int status;
    
    OSCThreadedObject(){
        status = 0;
        receiver.setup(PORT);
    }
    
    void start(){
        startThread(true, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    void threadedFunction(){
        while( isThreadRunning() != 0 ){
            if( lock() ){
                // check for waiting messages
                while(receiver.hasWaitingMessages()){
                    ofxOscMessage m;
                    receiver.getNextMessage(&m);
                    
                    // test message
                    if(m.getAddress() == "/test"){
                        //currentMessage = m.getArgAsString(0);
                        cout << "recived /test message : " << currentMessage << endl;
                        status = 1;
                    }
                }
                unlock();
            }
        }
    }
    
    
};

#endif
