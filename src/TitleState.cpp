//
//  TitleState.cpp
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#include "TitleState.h"

//--------------------------------------------------------------
TitleState::TitleState()
{
    init();
}

//--------------------------------------------------------------
TitleState::~TitleState()
{
    
}

//--------------------------------------------------------------
void TitleState::init()
{
    
}

//--------------------------------------------------------------
void TitleState::stateEnter()
{
    cout << "incoming port: " << getSharedData().incomingPort << endl;
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/hello");
    msg.addIntArg(getSharedData().incomingPort);
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void TitleState::stateExit()
{
    
}

//--------------------------------------------------------------
void TitleState::update()
{
    
}

//--------------------------------------------------------------
void TitleState::draw()
{
    
}

//--------------------------------------------------------------
string TitleState::getName()
{
    return "title";
}