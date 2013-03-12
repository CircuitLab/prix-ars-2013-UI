//
//  MapControlState.h
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#ifndef arsUI_MapControlState_h
#define arsUI_MapControlState_h

#define GUI_CANVAS_WIDTH 200
#define GUI_CANVAS_HEIGHT ofGetHeight()

#include "ofxState.h"
#include "ofxXmlSettings.h"
#include "ArsUISharedData.h"
#include "ofxTween.h"
#include "ArsUIRoboCam.h"
#include "ofxJSONElement.h"
#include "ofxUI.h"
#include "ArsUIUtil.h"
#include "ofxTimer.h"

class ArsUIMapControlState : public Apex::ofxState<ArsUISharedData>
{
    public:
        ArsUIMapControlState();
        ~ArsUIMapControlState();
        
        void stateEnter();
        void stateExit();
        void update();
        void draw();
        string getName();
    
        void keyPressed(int key);
        void keyReleased(int key);
        void mousePressed(int x, int y, int button);
        void mouseDragged(int x, int y, int button);
        
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
    
        ofPoint cam1Position, cam2Position;
    
        int eye[2]; //0:right 1:left
        int selectedMode;//0:select right eye, 1:select left eye
        void setEye(int bid);
    
        ofxJSONElement json;

        string rightImageFile, leftImageFile;
        void getPictureFromURL(string url);
    
        void setupGUI();
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
    
        ofxTimer timer;
        void timerReached(ofEventArgs &e);
        ofPoint initialTouchPoint, lastTouchPoint;
        bool isCam1Draggable, isCam2Draggable;
};

#endif
