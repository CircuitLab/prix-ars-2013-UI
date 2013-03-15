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
    //setupGUI();
}

//--------------------------------------------------------------
void ArsUIMapControlState::stateEnter()
{
    bShowStatus = true;
    
    fujiPoint.set(964, 600);
    
    json.clear();
    
    if (json.open(ofToDataPath("cameras.json"))) {
        for (int i = 0; i < json["cameras"].size(); ++i) {
            ofxJSONElement camera = json["cameras"][i];
            
            if (true == camera["operable"].asBool()) {
                operableCameras.push_back(createOperableCamera(camera, i));
            } else {
                inoperableCameras.push_back(createCamera(camera, i));
            }
        }
    }
    
    cam1Position = ofPoint(operableCameras[0].getPosition().x, operableCameras[0].getPosition().y);
    cam2Position = ofPoint(operableCameras[1].getPosition().x, operableCameras[1].getPosition().y);
    
    eyes[0] = -1;
    eyes[1] = -1;
    selectedEye = EYE_LEFT;
    
    fujiMap.loadImage("map.png");
    
    ofAddListener(getSharedData().tuioClient.cursorAdded, this, &ArsUIMapControlState::tuioAdded);
	ofAddListener(getSharedData().tuioClient.cursorRemoved, this, &ArsUIMapControlState::tuioRemoved);
	ofAddListener(getSharedData().tuioClient.cursorUpdated, this, &ArsUIMapControlState::tuioUpdated);
    ofAddListener(getSharedData().oscReceiverFromServer.onMessageReceived, this, &ArsUIMapControlState::onOscMessageReceived);
    
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/hello");
    msg.addIntArg(getSharedData().incomingPort);
    getSharedData().oscSenderToServer.sendMessage(msg);
    
    sendViewpointToServer(operableCameras[0]);
    sendViewpointToServer(operableCameras[1]);
    
    for (int i = 0; i < operableCameras.size(); ++i) {
        sendViewpointToServer(operableCameras[i]);
        sendImageUrlToDisplay(operableCameras[i].getEyeDirection(), "http://pds.exblog.jp/pds/1/200802/18/73/d0116173_14571347.jpg");
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::stateExit()
{
    inoperableCameras.clear();
    operableCameras.clear();
    fujiMap.clear();
    
    ofRemoveListener(getSharedData().tuioClient.cursorAdded, this, &ArsUIMapControlState::tuioAdded);
	ofRemoveListener(getSharedData().tuioClient.cursorRemoved, this, &ArsUIMapControlState::tuioRemoved);
	ofRemoveListener(getSharedData().tuioClient.cursorUpdated, this, &ArsUIMapControlState::tuioUpdated);
    ofRemoveListener(getSharedData().oscReceiverFromServer.onMessageReceived, this, &ArsUIMapControlState::onOscMessageReceived);
    ofRemoveListener(gui->newGUIEvent, this, &ArsUIMapControlState::guiEvent);
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
    
    for (int i = 0; i < inoperableCameras.size(); ++i) {
        inoperableCameras[i].update();
    }
    
    operableCameras[0].setPosition(cam1Position);
    operableCameras[1].setPosition(cam2Position);
    operableCameras[0].update();
    operableCameras[1].update();
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
    
    for (int i = 0; i<inoperableCameras.size(); ++i) {
        inoperableCameras[i].draw();
    }
    
    for (int i = 0; i < operableCameras.size(); ++i) {
        operableCameras[i].draw();
    }
    
    ofPushStyle();
    ofSetColor(255, 0, 0);
    
    if (bShowStatus) {
        drawCamStatuses(operableCameras[0]);
        drawCamStatuses(operableCameras[1]);
    } else {
        if (isCam1Draggable) {
            drawCamStatuses(operableCameras[0]);
        }
        
        if (isCam2Draggable) {
            drawCamStatuses(operableCameras[1]);
        }
    }
    
    double cam1deg = operableCameras[0].degreesAgainstFuji();
    double cam2deg = operableCameras[1].degreesAgainstFuji();
    
    if (0 <= cam1deg && 0 <= cam2deg) {
        if (cam1deg < cam2deg) {
            operableCameras[0].setEyeDirection(EYE_RIGHT);
            operableCameras[1].setEyeDirection(EYE_LEFT);
        } else {
            operableCameras[0].setEyeDirection(EYE_LEFT);
            operableCameras[1].setEyeDirection(EYE_RIGHT);
        }
    } else if (0 <= cam1deg && 0 > cam2deg) {
        operableCameras[0].setEyeDirection(EYE_RIGHT);
        operableCameras[1].setEyeDirection(EYE_LEFT);
    } else if (0 > cam1deg && 0 > cam2deg) {
        if (cam1deg < cam2deg) {
            operableCameras[0].setEyeDirection(EYE_RIGHT);
            operableCameras[1].setEyeDirection(EYE_LEFT);
        } else {
            operableCameras[0].setEyeDirection(EYE_LEFT);
            operableCameras[1].setEyeDirection(EYE_RIGHT);
        }
    } else if (0 > cam1deg && 0 <= cam2deg) {
        operableCameras[0].setEyeDirection(EYE_LEFT);
        operableCameras[1].setEyeDirection(EYE_RIGHT);
    }
    
    ofPopStyle();
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
    gui->addLabelButton("SHOOT", false, true);
    gui->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent, this, &ArsUIMapControlState::guiEvent);
}

//--------------------------------------------------------------
ArsUICamera ArsUIMapControlState::createCamera(ofxJSONElement json, int bid)
{
    return ArsUICamera(json["latitude"].asDouble(), json["longitude"].asDouble(), json["udid"].asString(), json["angle"].asInt(),  json["compass"].asInt(), bid, fujiPoint);
}

//--------------------------------------------------------------
ArsUIOperableCamera ArsUIMapControlState::createOperableCamera(ofxJSONElement json, int bid)
{
    return ArsUIOperableCamera(json["latitude"].asDouble(), json["longitude"].asDouble(), json["udid"].asString(), json["angle"].asInt(),  json["compass"].asInt(), json["battery"].asInt(), json["living"].asBool(), bid, fujiPoint);
}

//--------------------------------------------------------------
void ArsUIMapControlState::setEye(int bid)
{
    int lastEye = eyes[selectedEye];
    
    if (-1 != lastEye) {
        for (int i = 0; i < inoperableCameras.size(); ++i) {
            if(inoperableCameras[i].getButtonId() == lastEye){
                inoperableCameras[i].setStatus(0);
            }
        }
        
        for (int i = 0; i < operableCameras.size(); ++i) {
            if(operableCameras[i].getButtonId() == lastEye){
                operableCameras[i].setStatus(0);
            }
        }
    }
    
    eyes[selectedEye] = bid;
    
    selectedEye = !selectedEye ? EYE_RIGHT : EYE_LEFT;
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioAdded(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
    
    cout << "TUIO added, x: " << loc.x << ", y: " << loc.y << endl;
    
    ArsUITappedPoint tappedPoint(loc.x, loc.y, tuioCursor.getFingerId());
    getSharedData().tappedPoints.push_back(tappedPoint);
    
    for (int i = 0; i < inoperableCameras.size(); ++i) {
        int bid = inoperableCameras[i].hitTestPoint(loc);
        
        if (0 <= bid && bid != eyes[0] && bid != eyes[1]) {
            setEye(bid);
            inoperableCameras[i].setStatus(1);
        }
    }
    
    for (int i = 0; i < operableCameras.size(); ++i) {
        int bid = operableCameras[i].hitTestPoint(loc);
        
        if (0 <= bid && bid != eyes[0] && bid != eyes[1]){
            setEye(bid);
            operableCameras[i].setStatus(1);
        }
    }
    
    
    ofPoint cam1Pos = operableCameras[0].getPosition();
    ofPoint cam2Pos = operableCameras[1].getPosition();
    
    float dist1 = ArsUIUtil::distance(loc, cam1Pos);
    float dist2 = ArsUIUtil::distance(loc, cam2Pos);
        
    if (dist1 < dist2 && dist1 <= 10) {
        cam1TouchedStartedAt = ofGetElapsedTimeMillis();
        cam1FingerId = tuioCursor.getFingerId();
        cout << "CAM 1 touched, time(millis): " << cam1TouchedStartedAt << endl;
    } else if (dist2 < dist1 && dist2 <= 10) {
        cam2TouchedStartedAt = ofGetElapsedTimeMillis();
        cam2FingerId = tuioCursor.getFingerId();
        cout << "CAM 2 touched, time(millis): " << cam2TouchedStartedAt << endl;
    }
    
    //	cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioRemoved(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
    cout << "TUIO removed, id: " << tuioCursor.getFingerId() << endl;
	//cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
        
    if (tuioCursor.getFingerId() == cam1FingerId) {
        cam1TouchedStartedAt = 0;
        cam1FingerId = -1;
        isCam1Draggable = false;
        sendViewpointToServer(operableCameras[0]);
        sendImageUrlToDisplay(operableCameras[0].getEyeDirection(), "http://cache5.amanaimages.com/cen3tzG4fTr7Gtw1PoeRer/22973000320.jpg");
    } else if (tuioCursor.getFingerId() == cam2FingerId) {
        cam2TouchedStartedAt = 0;
        cam2FingerId = -1;
        isCam2Draggable = false;
        sendViewpointToServer(operableCameras[1]);
        sendImageUrlToDisplay(operableCameras[1].getEyeDirection(), "http://cache5.amanaimages.com/cen3tzG4fTr7Gtw1PoeRer/22973000320.jpg");
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::tuioUpdated(ofxTuioCursor &tuioCursor)
{
    ofPoint loc = ofPoint(tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight());
    
    // cout << "TUIO updated, id: " << tuioCursor.getFingerId() << endl;
            
    if (0 < cam1TouchedStartedAt && 1000 <= ofGetElapsedTimeMillis() - cam1TouchedStartedAt && tuioCursor.getFingerId() == cam1FingerId) {
        isCam1Draggable = true;
        cam1Position = loc;
    } else {
        isCam1Draggable = false;
        operableCameras[0].dragAngle(loc.x, loc.y);
    }
    
    if (0 < cam2TouchedStartedAt && 1000 <= ofGetElapsedTimeMillis() - cam2TouchedStartedAt && tuioCursor.getFingerId() == cam2FingerId) {
        isCam2Draggable = true;
        cam2Position = loc;
    } else {
        isCam2Draggable = false;
        operableCameras[1].dragAngle(loc.x, loc.y);
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
void ArsUIMapControlState::drawCamStatuses(ArsUIOperableCamera cam)
{
    string str = "X      : " + ofToString(cam.getPosition().x) +
               "\nY      : " + ofToString(cam.getPosition().y) +
               "\nANGLE  : " + ofToString(cam.degreesAgainstFuji()) +
               "\nCOMPASS: " + ofToString(cam.getCompass()) +
               "\nEYE    : " + ofToString(EYE_LEFT == cam.getEyeDirection() ? "LEFT" : "RIGHT");
    ofDrawBitmapString(str, cam.getPosition() + ofPoint(70, -80));
}

//--------------------------------------------------------------
void ArsUIMapControlState::onOscMessageReceived(ofxOscMessage &msg)
{
    json.clear();
    
    string addr = msg.getAddress();
    
    cout << "OSC received. address: " << addr << endl;
    
    if ("/gianteyes/camera" == addr) {
        string jsonString = msg.getArgAsString(0);
        
        if (0 < jsonString.length()) {
            if (json.parse(jsonString)) {
                string udid = json["udid"].asString();
                
                if (true == json["operable"].asBool()) {
                    for (int i = 0; i < operableCameras.size(); ++i) {
                        if (operableCameras[i].getUDID() == udid) {
                            operableCameras[i].setCameraStatus(json);
                        }
                    }
                } else {
                    for (int i = 0; i < inoperableCameras.size(); ++i) {
                        if (inoperableCameras[i].getUDID() == udid) {
                            inoperableCameras[i].setCameraStatus(json);
                        }
                    }
                }
            }
        }
    } else if ("/gianteyes/photo" == addr) {
        string jsonString = msg.getArgAsString(0);
        
        if (0 < jsonString.length()) {
            if (json.parse(jsonString)) {
                for (int i = 0; i < operableCameras.size(); ++i) {
                    if (json["udid"] == operableCameras[i].getUDID()) {
                        sendImageUrlToDisplay(operableCameras[i].getEyeDirection(), json["url"].asString());
                    }
                }
            }
        }
    } else if ("/gianteyes/cameras" == addr) {
        string jsonString = msg.getArgAsString(0);
        
        if (0 < jsonString.length()) {
            if (json.parse(jsonString)) {
                inoperableCameras.clear();
                operableCameras.clear();
                
                for (int i = 0; i < json["cameras"].size(); ++i) {
                    ofxJSONElement camera = json["cameras"][i];
                    
                    if (true == camera["operable"].asBool()) {
                        operableCameras.push_back(createOperableCamera(camera, i));
                    } else {
                        inoperableCameras.push_back(createCamera(camera, i));
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendViewpointToServer(ArsUIOperableCamera cam)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/viewpoint");
    msg.addIntArg(cam.getButtonId());
    msg.addIntArg(cam.getCompass());
    msg.addIntArg(cam.getAngle());
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendShootingTriggerToServer(int camId)
{
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/take");
    msg.addIntArg(camId);
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::sendImageUrlToDisplay(int whichEye, string url)
{
    ofxOscMessage msg;
    
    EYE_RIGHT == whichEye ? msg.setAddress("/url/right") : msg.setAddress("/url/left");
    
    msg.addStringArg(url);
    getSharedData().oscSenderToDisplay.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUIMapControlState::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(kind == OFX_UI_WIDGET_LABELBUTTON) {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        cout << name << "\t value: " << button->getValue() << endl;
    }
}