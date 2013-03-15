#pragma once

#include "ofMain.h"
#include "ofxTuioClient.h"
#include "ofxStateMachine.h"
#include "ArsUISharedData.h"
#include "ofxJSONElement.h"

#define SERVER_HOST_DEVELOPMENT "localhost"
#define SERVER_HOST_PRODUCTION  "prix-ars-2013.aws.uniba.jp"

#define DISPLAY_HOST "127.0.0.1"
#define DISPLAY_PORT 4444

#define SERVER_HOST SERVER_HOST_PRODUCTION
#define SERVER_PORT 50001

#define INCOMING_PORT 5000

#define NUM_ROBOTS 2
#define CAM_ID_1 0
#define CAM_ID_2 1


// uncomment if you use Full HD display
#define FULL_HD_ENABLE

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
    void initOSC();
    void initState();
    
    Apex::ofxStateMachine<ArsUISharedData> stateMachine;
};
