//
//  TitleState.cpp
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#include "ArsUITitleState.h"

//--------------------------------------------------------------
ArsUITitleState::ArsUITitleState()
{
    init();
}

//--------------------------------------------------------------
ArsUITitleState::~ArsUITitleState()
{
    
}

//--------------------------------------------------------------
void ArsUITitleState::init()
{
    
}

//--------------------------------------------------------------
void ArsUITitleState::stateEnter()
{
    cout << "incoming port: " << getSharedData().incomingPort << endl;
    ofxOscMessage msg;
    msg.setAddress("/gianteyes/hello");
    msg.addIntArg(getSharedData().incomingPort);
    getSharedData().oscSenderToServer.sendMessage(msg);
}

//--------------------------------------------------------------
void ArsUITitleState::stateExit()
{
    
}

//--------------------------------------------------------------
void ArsUITitleState::update()
{
    
}

//--------------------------------------------------------------
void ArsUITitleState::draw()
{
    
}

//--------------------------------------------------------------
string ArsUITitleState::getName()
{
    return "title";
}