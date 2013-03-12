//
//  MapControlState.h
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#ifndef arsUI_MapControlState_h
#define arsUI_MapControlState_h

#include "ofxState.h"
#include "ofxXmlSettings.h"
#include "ArsUISharedData.h"
#include "ofxTween.h"
#include "ArsUIRoboCam.h"
#include "ofxJSONElement.h"

class MapControlState : public Apex::ofxState<ArsUISharedData>
{
    public:
        MapControlState();
        ~MapControlState();
        
        void stateEnter();
        void stateExit();
        void update();
        void draw();
        string getName();
        
    private:
        void init();
    
        vector <ArsUIButton> buttons; // web camera
        vector <ArsUIRoboCam> robos; // robot
    
        void tuioAdded(ofxTuioCursor & tuioCursor);
        void tuioRemoved(ofxTuioCursor & tuioCursor);
        void tuioUpdated(ofxTuioCursor & tuioCursor);
        void drawTuioCursors();
    
        void onOscMessageReceived(ofxOscMessage &msg);
    
        void sendViewpointToServer(int camId, int compass, int angle);
        void sendTakeTriggerToServer(int camId);
        void sendOSCToDisplay(int bid);
    
        ofImage fujiMap;
        ofPoint fujiPoint;
        
        int eye[2]; //0:right 1:left
        int selectedMode;//0:select right eye, 1:select left eye
        void setEye(int bid);
    
        ofxJSONElement json;

        string rightImageFile, leftImageFile;
        void getPictureFromURL(string url);
};

#endif
