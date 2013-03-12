#include "testApp.h"
#include "ArsUITitleState.h"
#include "ArsUIMapControlState.h"

//--------------------------------------------------------------
void testApp::initOSC()
{
    stateMachine.getSharedData().oscSenderToDisplay.setup(DISPLAY_HOST, DISPLAY_PORT);
    stateMachine.getSharedData().oscReceiverFromServer.setup(INCOMING_PORT);    
    stateMachine.getSharedData().oscSenderToServer.setup(SERVER_HOST, SERVER_PORT);
}

//--------------------------------------------------------------
void testApp::initState()
{
    stateMachine.addState(new ArsUITitleState());
    stateMachine.addState(new ArsUIMapControlState());
    stateMachine.changeState("map");
}

//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofSetBackgroundAuto(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
#ifdef FULL_HD_ENABLE
    ofHideCursor();
#endif
    
    stateMachine.getSharedData().displayHost  = DISPLAY_HOST;
    stateMachine.getSharedData().displayPort  = DISPLAY_PORT;
    stateMachine.getSharedData().serverHost   = SERVER_HOST;
    stateMachine.getSharedData().serverPort   = SERVER_PORT;
    stateMachine.getSharedData().incomingPort = INCOMING_PORT;
    stateMachine.getSharedData().numRobots    = NUM_ROBOTS;
    stateMachine.getSharedData().camId1       = CAM_ID_1;
    stateMachine.getSharedData().camId2       = CAM_ID_2;
    
    stateMachine.getSharedData().tuioClient.start(3333);
    
    initOSC();
    initState();
}

//--------------------------------------------------------------
void testApp::update()
{
    
}

//--------------------------------------------------------------
void testApp::draw()
{
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == ' '){
        ofBackground(255, 255, 255);
    }
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


