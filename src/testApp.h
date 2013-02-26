#pragma once

#include "ofMain.h"
#include "ofxTuioClient.h"
#include "tapped.h"
//#include "button.h"
#include "robo.h"

class testApp : public ofBaseApp{
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
    vector <tapped> taps; // tap expression
    vector <button> buttons; // web camera
    vector <robo> robos; // robot
    
    ofImage fujiMap;
    ofPoint fujiPoint;
    
    int eye[2]; //0:right 1:left
    int selectMode;//0:select right eye, 1:select left eye
    void setEye(int _bid);
};
