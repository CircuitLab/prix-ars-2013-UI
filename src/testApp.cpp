#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofBackground(0,0,0);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofSetBackgroundAuto(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofHideCursor();
    
    ofAddListener(tuioClient.cursorAdded, this, &testApp::tuioAdded);
	ofAddListener(tuioClient.cursorRemoved, this, &testApp::tuioRemoved);
	ofAddListener(tuioClient.cursorUpdated, this, &testApp::tuioUpdated);
    
    tuioClient.start(3333);
    toDisplay.setup(DISPLAYHOST,DISPLAYPORT);
    
    fujiPoint.set(964, 600);
    
    for (int i = 0; i < 10; ++i) {
        button b(ofRandom(ofGetWidth()), ofRandom(ofGetHeight() - 100), i, fujiPoint, ofRandom(20, 60));
        buttons.push_back(b);
    }
    
    for (int i = 10; i < 16; ++i) {
        robo r(ofRandom(ofGetWidth()),ofRandom(ofGetHeight() -100),i,fujiPoint,ofRandom(20,60));
        robos.push_back(r);
    }
    
    fujiMap.loadImage("map.png");
    
    eye[0] = -1;
    eye[1] = -1;
    selectMode = 0;

}

//--------------------------------------------------------------
void testApp::update()
{
    tuioClient.getMessage();
    
    for (int i = 0; i < taps.size(); ++i) {
        taps[i].update();
        
        if (false == taps[i].alive()) {
            vector<tapped>::iterator it = taps.begin();
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
    
    class tapped tap(loc.x, loc.y, tuioCursor.getFingerId());
    taps.push_back(tap);
    
    for (int i = 0; i < buttons.size(); ++i) {
        int bid = buttons[i].tapped(loc.x, loc.y);
        
        if (0 <= bid && bid != eye[0] && bid != eye[1]){
            setEye(bid);
            buttons[i].setStatus(1);
            sendOSCtoDisplay(bid);
        }
    }
    
    for (int i = 0; i < robos.size(); ++i) {
        int bid = robos[i].tapped(loc.x, loc.y);
        
        if (0 <= bid && bid != eye[0] && bid != eye[1]){
            setEye(bid);
            robos[i].setStatus(1);
            sendOSCtoDisplay(bid);
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

void testApp::sendOSCtoDisplay(int _bid)
{
    ofxOscMessage m;
    
    if (1 == selectMode) {
      m.setAddress("/right/image");
      m.addStringArg("right file address");
    } else if (0 == selectMode)  {
      m.setAddress("/left/image");
      m.addStringArg("left file address");
    }
    
    m.addIntArg(_bid);
    toDisplay.sendMessage(m);
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(key == ' '){
        ofBackground(255,255,255);
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


