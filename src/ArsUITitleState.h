//
//  TitleState.h
//  arsUI
//
//  Created by Circuit Lab. Mac mini on 3/12/13.
//
//

#ifndef arsUI_TitleState_h
#define arsUI_TitleState_h

#include "ofxState.h"
#include "ofxXmlSettings.h"
#include "ArsUISharedData.h"
#include "ofxOsc.h"

class ArsUITitleState : public Apex::ofxState<ArsUISharedData>
{
public:
    ArsUITitleState();
    ~ArsUITitleState();
    
    void stateEnter();
    void stateExit();
    void update();
    void draw();
    string getName();
    
private:
    void init();
    
    ofTrueTypeFont font;
    
};

#endif
