#include "ofMain.h"


class button{
protected:
    int status,bcnt,speed,tcnt; //status 0: not selected, 1:selected, 2: broken 
    ofImage mark,markShadow;
    float x,y,radius;
    int bid;
    ofPoint fuji;
    double directionToFuji;
    double calcDirection(ofPoint *_p1, ofPoint *_p2);
    double viewAngle;
    
public:
	button(float _x, float _y, int _bid, ofPoint _fuji, float _angle );
	virtual void draw();
	virtual void update();
	virtual int tapped(float _x, float _y);
    void setAngle(float _angle);
    void setStatus(int _status);
    int getStatus();
    int getId();
};
