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

#define EYE_LEFT  0
#define EYE_RIGHT 1

#include "ofxState.h"
#include "ofxXmlSettings.h"
#include "ArsUISharedData.h"
#include "ofxTween.h"
#include "ArsUIOperableCamera.h"
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
    
        vector <ArsUICamera> inoperableCameras; // web camera
        vector <ArsUIOperableCamera> operableCameras; // robot
        ArsUICamera createCamera(ofxJSONElement json, int bid);
        ArsUIOperableCamera createOperableCamera(ofxJSONElement json, int bid);
    
        void tuioAdded(ofxTuioCursor & tuioCursor);
        void tuioRemoved(ofxTuioCursor & tuioCursor);
        void tuioUpdated(ofxTuioCursor & tuioCursor);
        void drawTuioCursors();
    
        void onOscMessageReceived(ofxOscMessage &msg);
    
        void sendViewpointToServer(ArsUIOperableCamera cam);
        void sendShootingTriggerToServer(int camId);
        void sendImageUrlToDisplay(int whichEye, string url);
    
        ofImage fujiMap;
        ofPoint fujiPoint;
    
        ofPoint cam1Position, cam2Position;
    
        int eyes[2]; //0:right 1:left
        int selectedEye;//0:select right eye, 1:select left eye
        void setEye(int bid);
    
        ofxJSONElement json;

        string rightImageFile, leftImageFile;
    
        void setupGUI();
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
    
        bool isCam1Draggable, isCam2Draggable;
        int cam1FingerId, cam2FingerId;
        unsigned long long cam1TouchedStartedAt, cam2TouchedStartedAt;
        void drawCamStatuses(ArsUIOperableCamera cam);
    
        bool bShowStatus;
    
        ofxTimer doubleTapTimer;
};

#endif
