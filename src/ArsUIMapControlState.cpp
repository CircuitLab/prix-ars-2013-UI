//
//  MapControlState.cpp
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#include "ArsUIMapControlState.h"

//--------------------------------------------------------------
ArsUIMapControlState::ArsUIMapControlState()
{
    init();
}

//--------------------------------------------------------------
ArsUIMapControlState::~ArsUIMapControlState()
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::init()
{
    setupGUI();
    // gui->setDrawBack(false);
}

//--------------------------------------------------------------
void ArsUIMapControlState::stateEnter()
{
    fujiPoint.set(532.16, 633.19);
    
    for (int i = 0; i < 10; ++i) {
        buttons.push_back(ArsUIButton(ofRandom(ofGetWidth()), ofRandom(ofGetHeight() - 100), i, fujiPoint, ofRandom(20, 60)));
    }
    
    for (int i = 10; i < (10 + getSharedData().numRobots); ++i) {
        robos.push_back(ArsUIRoboCam(ofRandom(ofGetWidth()), ofRandom(ofGetHeight() - 100), i, fujiPoint, ofRandom(20, 60)));
    }
    
    eye[0] = -1;
    eye[1] = -1;
    selectedMode = 0;
    
    fujiMap.loadImage("map.png");
    
    ofAddListener(getSharedData().tuioClient.cursorAdded, this, &ArsUIMapControlState::tuioAdded);
	ofAddListener(getSharedData().tuioClient.cursorRemoved, this, &ArsUIMapControlState::tuioRemoved);
	ofAddListener(getSharedData().tuioClient.cursorUpdated, this, &ArsUIMapControlState::tuioUpdated);
    ofAddListener(getSharedData().oscReceiverFromServer.onMessageReceived, this, &ArsUIMapControlState::onOscMessageReceived);
}

//--------------------------------------------------------------
void ArsUIMapControlState::stateExit()
{
    buttons.clear();
    robos.clear();
    fujiMap.clear();
    
    ofRemoveListener(getSharedData().tuioClient.cursorAdded, this, &ArsUIMapControlState::tuioAdded);
	ofRemoveListener(getSharedData().tuioClient.cursorRemoved, this, &ArsUIMapControlState::tuioRemoved);
	ofRemoveListener(getSharedData().tuioClient.cursorUpdated, this, &ArsUIMapControlState::tuioUpdated);
    ofRemoveListener(getSharedData().oscReceiverFromServer.onMessageReceived, this, &ArsUIMapControlState::onOscMessageReceived);
}

//--------------------------------------------------------------
void ArsUIMapControlState::update()
{
    getSharedData().tuioClient.getMessage();
    
    for (int i = 0; i < getSharedData().tappedPoints.size(); ++i) {
        getSharedData().tappedPoints[i].update();
        
        if (false == getSharedData().tappedPoints[i].alive()) {
            vector<ArsUITappedPoint>::iterator it = getSharedData().tappedPoints.begin();
            getSharedData().tappedPoints.erase(it + i);
        }
    }
    
    for (int i = 0; i < buttons.size(); ++i) {
        buttons[i].update();
    }
    
    for (int i = 0; i < robos.size(); ++i) {
        robos[i].update();
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::draw()
{
    // tuioClient.drawCursors();
    //drawUpdated();
    
    ofSetColor(255, 255, 255);
    fujiMap.draw(-350, -10, 1920 * 1.4, 907 * 1.4);
    
    // ofCircle(fujiPoint, 1);
    
    ofSetColor(255, 255, 255, 255);
    
    for (int i = 0; i < getSharedData().tappedPoints.size(); ++i){
        getSharedData().tappedPoints[i].draw();
    }
    
    for (int i = 0; i<buttons.size(); ++i) {
        buttons[i].draw();
    }
    
    for (int i = 0; i < robos.size(); ++i) {
        robos[i].draw();
    }
}

//--------------------------------------------------------------
string ArsUIMapControlState::getName()
{
    return "map";
}

//--------------------------------------------------------------
void ArsUIMapControlState::keyPressed(int key)
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::setupGUI()
{
    gui = new ofxUICanvas(0, 0, GUI_CANVAS_WIDTH, GUI_CANVAS_HEIGHT);
    gui->setColorBack(ofColor(255, 255, 255, 127));
    gui->setColorFill(ofColor(0));
    gui->addWidgetDown(new ofxUILabel("CONTROL PANEL", OFX_UI_FONT_LARGE));
    gui->addSpacer();
    gui->addFPSSlider("FPS SLIDER", GUI_CANVAS_WIDTH - 10, 18, 60);
    gui->addSpacer();
    gui->addWidgetDown(new ofxUILabel("FUJI POSITION", OFX_UI_FONT_MEDIUM));
    gui->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void ArsUIMapControlState::setEye(int bid)
{
    int oldMode = eye[selectedMode];
    
    if (-1 != oldMode) {
        for (int i = 0; i < buttons.size(); ++i) {
            if(buttons[i].getId() == oldMode){
                buttons[i].setStatus(0);
            }
        }
        
        for (int i = 0; i < robos.size(); ++i) {
            if(robos[i].getId() == oldMode){
                robos[i].setStatus(0);
            }
        }
    }
    
    eye[selectedMode] = bid;
    
    selectedMode = !selectedMode ? 1 : 0;
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioAdded(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
    
    ArsUITappedPoint tappedPoint(loc.x, loc.y, tuioCursor.getFingerId());
    getSharedData().tappedPoints.push_back(tappedPoint);
    
    for (int i = 0; i < buttons.size(); ++i) {
        int bid = buttons[i].tapped(loc.x, loc.y);
        
        if (0 <= bid && bid != eye[0] && bid != eye[1]){
            setEye(bid);
            buttons[i].setStatus(1);
            sendOSCToDisplay(bid);
        }
    }
    
    for (int i = 0; i < robos.size(); ++i) {
        int bid = robos[i].tapped(loc.x, loc.y);
        
        if (0 <= bid && bid != eye[0] && bid != eye[1]){
            setEye(bid);
            robos[i].setStatus(1);
            sendOSCToDisplay(bid);
        }
    }
    
    //	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioRemoved(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioUpdated(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    for (int i = 0; i < robos.size(); ++i) {
        robos[i].dragAngle(loc.x, loc.y);
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::drawTuioCursors()
{
    list<ofxTuioCursor*> cursorList = getSharedData().tuioClient.getTuioCursors();
    
    if (0 < cursorList.size()) {
        list<ofxTuioCursor*>::iterator tcursor;
        
        for (tcursor = cursorList.begin(); tcursor != cursorList.end(); ++tcursor) {
            ofxTuioCursor *cursor = (*tcursor);
            float x = (*cursor).getX() * ofGetWidth();
            float y = (*cursor).getY() * ofGetHeight();
            // cout << "x" <<  (*cursor).getXSpeed() <<" ";
            // ofSetColor(100,100, (*cursor).getFingerId() ,127);
            
            ofSetColor(55, 55, 55, 100);
            ofCircle(x, y, 5 - (*cursor).getXSpeed() * ofGetWidth() * 5 + 5 - (*cursor).getYSpeed() * ofGetWidth() * 5);
        }
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::onOscMessageReceived(ofxOscMessage &msg)
{
    string addr = msg.getAddress();
    
    if ("/gianteyes/camera" == addr) {
        int udid = msg.getArgAsInt32(0);
        float lat = msg.getArgAsFloat(1);
        float lon = msg.getArgAsFloat(2);
        float compass = msg.getArgAsFloat(3);
        int angle = msg.getArgAsInt32(4);
        int battery = msg.getArgAsInt32(5);
        int operable = msg.getArgAsInt32(6);
        int living = msg.getArgAsInt32(7);
    } else if ("/gianteyes/photo" == addr) {
        int udid = msg.getArgAsInt32(0);
        string url = msg.getArgAsString(1);
        float correction = msg.getArgAsFloat(2);
        float lat = msg.getArgAsFloat(3);
        float lon = msg.getArgAsFloat(4);
        float compass = msg.getArgAsFloat(5);
        int angle = msg.getArgAsInt32(6);
        
        getPictureFromURL(url);
    } else if ("/gianteyes/cameras" == addr) {
        string jsonString = msg.getArgAsString(0);
        
        if (0 < jsonString.length()) {
            json.parse(jsonString);
        }
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::getPictureFromURL(string url)
{
    
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendViewpointToServer(int camId, int compass, int angle)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/viewpoint");
    msg.addIntArg(camId);
    msg.addIntArg(compass);
    msg.addIntArg(angle);
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendTakeTriggerToServer(int camId)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/take");
    msg.addIntArg(camId);
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendOSCToDisplay(int bid)
{
    ofxOscMessage msg;
    
    if (1 == selectedMode) {
        msg.setAddress("/right/image");
        msg.addStringArg("right file address");
    } else if (0 == selectedMode)  {
        msg.setAddress("/left/image");
        msg.addStringArg("left file address");
    }
    
    msg.addIntArg(bid);
    getSharedData().oscSenderToDisplay.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::guiEvent(ofxUIEventArgs &e)
{
    
}