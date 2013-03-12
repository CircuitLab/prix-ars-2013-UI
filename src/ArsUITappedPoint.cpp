//
//  tapped.cpp
//  emptyExample
//
//  Created by yamamoto on 2013/02/21.
//
//

#include "ArsUITappedPoint.h"
#include "ofxEasingFunc.h"

ArsUITappedPoint::ArsUITappedPoint(float _x, float _y, int tid)
{
	x = _x;
	y = _y;
	radius=1;
	alpha = 100;
	tcnt = 0;
    cntmax = 30;
    tapid = tid;
}

void ArsUITappedPoint::update()
{
    float v = ofxEasingFunc::Circ::easeOut((float)tcnt / cntmax);
    //cout << " v " << v;
	radius = v * 100;
	alpha = (1 - v) * 80;
	tcnt++;
}

void ArsUITappedPoint::draw()
{
	ofSetColor(51, 205, 255, alpha);
    //cout<< " alpha " << alpha << " radius " << radius ;
	ofCircle(x, y, radius);
	ofSetColor(255, 255, 255, 255);
}

bool ArsUITappedPoint::alive()
{
	return tcnt <= cntmax ;
    
}