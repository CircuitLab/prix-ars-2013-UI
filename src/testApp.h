#pragma once

#include "ofMain.h"
#include "ofxTuioClient.h"
#include "ofxArsUITappedPoint.h"
//#include "ofxArsUIButton.h"
#include "ofxArsUIRoboCam.h"
#include "ofxJSONElement.h"

#define DISPLAY_HOST "127.0.0.1"
#define DISPLAY_PORT 4444

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 5555

#define INCOMING_PORT 6666

#define NUM_ROBOTS 2
#define COM_ID_1 0
#define COM_ID_2 1


// uncomment if you use Full HD display
// #define FULL_HD_ENABLE

class testApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void tuioAdded(ofxTuioCursor & tuioCursor);
    void tuioRemoved(ofxTuioCursor & tuioCursor);
    void tuioUpdated(ofxTuioCursor & tuioCursor);
    void drawUpdated();
    
    ofxTuioClient   tuioClient;
    vector <ofxArsUITappedPoint> taps; // tap expression
    vector <ofxArsUIButton> buttons; // web camera
    vector <ofxArsUIRoboCam> robos; // robot
    
    ofImage fujiMap;
    ofPoint fujiPoint;
    
    int eye[2]; //0:right 1:left
    int selectMode;//0:select right eye, 1:select left eye
    void setEye(int _bid);
    
    ofxOscSender oscSenderToDisplay;
    ofxOscSender oscSenderToServer;
    ofxOscReceiver oscReceiverFromServer;
    
    void onOscMessageReceived(ofxOscMessage &msg);
    
    void sendInitToServer();
    void sendViewpointToServer(int camId, int compass, int angle);
    void sendTakeTriggerToServer(int camId);
    void sendOSCToDisplay(int _bid);
    
    
    string rightImageFile, leftImageFile;
    void getPictureFromURL(string url);
    
    ofxJSONElement json;
};
