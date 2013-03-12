//
//  ArsUISharedData.h
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#ifndef arsUI_ArsUISharedData_h
#define arsUI_ArsUISharedData_h

#include "ofxXmlSettings.h"
#include "ofxTuioClient.h"
#include "ArsUITappedPoint.h"
#include "ofxOsc.h"

class ArsUISharedData
{
    public:
        ofxXmlSettings config;
        ofxTuioClient tuioClient;
    
        vector<ArsUITappedPoint> tappedPoints;
    
        string displayHost;
        int displayPort;
    
        string serverHost;
        int serverPort;

        int incomingPort;
    
        int numRobots;
        int camId1, camId2;
    
        ofxOscSender oscSenderToDisplay;
        ofxOscSender oscSenderToServer;
        ofxOscReceiver oscReceiverFromServer;
};

#endif
