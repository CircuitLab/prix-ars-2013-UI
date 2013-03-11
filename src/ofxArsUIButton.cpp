#include "ofxArsUIButton.h"
#include "ofxEasingFunc.h"

ofxArsUIButton::ofxArsUIButton(float _x, float _y, int _bid, ofPoint _fuji, float _angle){
	x = _x;
	y = _y;
    bid = _bid;
    status = 0;
    speed = 3600;
    bcnt = (int)ofRandom(speed);
    mark.loadImage("mark.png");
    markShadow.loadImage("markshadow.png");
    fuji = _fuji;
    ofPoint current;
    current.set(_x, _y);
    directionToFuji = calcDirection(&current, &fuji);
    currentDirection = directionToFuji;
    viewAngle = _angle;
}

void ofxArsUIButton::update(){
    
    float v = sinf((float)bcnt/speed * 2 * PI);
    radius = 30 + v*4;
    bcnt++;
    if(bcnt == speed) bcnt =0;
}

void ofxArsUIButton::draw(){
    //status: 0:default 1:select
    
    ofColor c;
    ofColor c0(200,200,200,255);
    ofColor c1(255,255,255,255);
    ofColor c2(0,0,0,200);
    
    float distanceToFuji = sqrt((x-fuji.x)*(x-fuji.x) + (y-fuji.y)*(y-fuji.y));
    distanceToFuji +=100;
    ofPath path;
    path.setCurveResolution(120);
    path.setColor( ofColor(50, 200, 255,40));
    path.moveTo(x,y);
    float angle1 = 360 -( 360 * currentDirection/(PI*2) + viewAngle/2) ;
    float angle2 = 360 - (360 * currentDirection/(PI*2) - viewAngle/2) ;
    path.arc(x,y,distanceToFuji,distanceToFuji,angle1,angle2);
    
    
    if(status ==0){
        c = c0;
    }else if (status == 1){
        c = c1;
        radius *= 2;
         path.draw();
    }else if (status ==2){
        c = c2;
    }
    
    ofSetColor(255, 255, 255,200);
    ofLine(x, y, x + cos(currentDirection) *100  ,y - sin(currentDirection)*100);
    //ofDrawBitmapString(ofToString(bid), x,y+50);
    
    ofSetColor(c);
    //ofCircle(x, y, radius);
    ofPushMatrix();
    ofTranslate(x, y, 0);
    ofRotateZ((bcnt/2)%360);
    markShadow.draw( - radius/2 ,  - radius /2 -2 ,radius, radius);
    mark.draw( - radius/2 +2,  - radius /2 ,radius, radius);
    ofSetColor(255, 255, 255, 255);
    ofPopMatrix();
}

int ofxArsUIButton::tapped(float _x, float _y){
    double distance =   sqrt( (double)((_x - x) * (_x - x) + (_y - y)*(_y - y))) ;
    if (distance < radius/2) {
        return bid;
    }
    return -1;
}

//return radian
double ofxArsUIButton::calcDirection(ofPoint *_p1, ofPoint *_p2){
    double v1[] = {100.0, 0};
    double v2[] = { _p2->x - _p1->x, _p2->y - _p1->y };
    double cosTheta,theta,innerProduct;
    innerProduct = v1[0] *v2[0] + v1[1] * v2[1];
    cosTheta = innerProduct /  ( sqrt((v1[0]*v1[0]) + (v1[1] * v1[1])) * sqrt((v2[0]*v2[0]) + (v2[1] * v2[1])));
    theta = acos(cosTheta);
    if(v2[1] >0){
        theta = 2 * PI - theta;
    }
    return theta;
}

void ofxArsUIButton::setAngle(float _angle){
    viewAngle = _angle;
}

void ofxArsUIButton::setStatus(int _status){
    status = _status;
}
int ofxArsUIButton::getStatus(){
    return bid;
}
int ofxArsUIButton::getId(){
    return bid;
}

void ofxArsUIButton::setCameraStatus(double _lat, double _lon){
    latitude = _lat;
    longtitude = _lon;
    ofPoint place = GPStoXY(_lat, _lon);
    x = place.x;
    y = place.y;
    
}

ofPoint ofxArsUIButton::GPStoXY(double _lat,double _lon){
    //calibration set
    //fuji benchmark (富士 中心)
    double fujiLat = 35.362841;
    double fujiLon = 138.730674;
    //yamanakako bench (旭日丘 交差点)
    double yamanakaLat = 35.407591;
    double yamanakaLon = 138.876039;
    ofPoint yamanaka = ofPoint(1263,489);
    
    ofPoint newPoint;
    
    newPoint.x = fuji.x + (_lon -fujiLon) * ((yamanaka.x - fuji.x)/ (yamanakaLon - fujiLon));
    newPoint.y = fuji.y + (_lat -fujiLat) * ((yamanaka.y - fuji.y)/ (yamanakaLat - fujiLat));
    //cout << "x" << newPoint.x << " " <<  newPoint.y;
    return newPoint;
}

