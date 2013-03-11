#include "testApp.h"

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
    
    ofAddListener(tuioClient.cursorAdded, this, &testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved, this, &testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated, this, &testApp::tuioUpdated);
    
    tuioClient.start(3333);
    oscSenderToDisplay.setup(DISPLAY_HOST, DISPLAY_PORT);
    
    oscReceiverFromServer.setup(INCOMING_PORT);
    ofAddListener(oscReceiverFromServer.onMessageReceived, this, &testApp::onOscMessageReceived);
    
    fujiPoint.set(964, 600);
    
    for (int i = 0; i < 10; ++i) {
        buttons.push_back(ofxArsUIButton(ofRandom(ofGetWidth()), ofRandom(ofGetHeight() - 100), i, fujiPoint, ofRandom(20, 60)));
    }
    
    for (int i = 10; i < (10 + NUM_ROBOTS); ++i) {
        robos.push_back(ofxArsUIRoboCam(ofRandom(ofGetWidth()), ofRandom(ofGetHeight() - 100), i, fujiPoint, ofRandom(20, 60)));
    }
    
    fujiMap.loadImage("map.png");
    
    eye[0] = -1;
    eye[1] = -1;
    selectMode = 0;
    
    oscSenderToServer.setup(SERVER_HOST, SERVER_PORT);
    sendInitToServer();
}

//--------------------------------------------------------------
void testApp::update()
{
    tuioClient.getMessage();
    
    for (int i = 0; i < taps.size(); ++i) {
        taps[i].update();
        
        if (false == taps[i].alive()) {
            vector<ofxArsUITappedPoint>::iterator it = taps.begin();
            taps.erase(it + i);
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
void testApp::draw()
{
    // tuioClient.drawCursors();
    //drawUpdated();
    
    ofSetColor(255, 255, 255, 180);
    fujiMap.draw(-350, -10, 1920 * 1.4 , 907 * 1.4);
    
    // ofCircle(fujiPoint, 1);
    
    ofSetColor(255, 255, 255, 255);
    
    for (int i = 0; i < taps.size(); ++i){
        taps[i].draw();
    }
    
    for (int i = 0; i<buttons.size(); ++i) {
        buttons[i].draw();
    }
    
    for (int i = 0; i < robos.size(); ++i) {
        robos[i].draw();
    }
}

//multi touch test
void testApp::drawUpdated()
{
    list<ofxTuioCursor*> cursorList = tuioClient.getTuioCursors();
    
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

void testApp::tuioAdded(ofxTuioCursor &tuioCursor)
{
	ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
    
    class ofxArsUITappedPoint tap(loc.x, loc.y, tuioCursor.getFingerId());
    taps.push_back(tap);
    
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

void testApp::tuioUpdated(ofxTuioCursor &tuioCursor)
{
	ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
    
    for (int i = 0; i < robos.size(); ++i) {
        robos[i].dragAngle(loc.x, loc.y);
    }
}

void testApp::tuioRemoved(ofxTuioCursor &tuioCursor)
{
	ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

void testApp::setEye(int _bid)
{
    int oldMode = eye[selectMode];
    
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

    eye[selectMode] = _bid;
    
    selectMode = !selectMode ? 1 : 0;
}

//--------------------------------------------------------------
void testApp::getPictureFromURL(string url)
{
    
}

//--------------------------------------------------------------
void testApp::sendInitToServer()
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/hello");
    msg.addIntArg(INCOMING_PORT);
    oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::sendViewpointToServer(int camId, int compass, int angle)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/viewpoint");
    msg.addIntArg(camId);
    msg.addIntArg(compass);
    msg.addIntArg(angle);
    oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::sendTakeTriggerToServer(int camId)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/take");
    msg.addIntArg(camId);
    oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::sendOSCToDisplay(int _bid)
{
    ofxOscMessage msg;
    
    if (1 == selectMode) {
      msg.setAddress("/right/image");
      msg.addStringArg("right file address");
    } else if (0 == selectMode)  {
      msg.setAddress("/left/image");
      msg.addStringArg("left file address");
    }
    
    msg.addIntArg(_bid);
    oscSenderToDisplay.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::onOscMessageReceived(ofxOscMessage &msg)
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


