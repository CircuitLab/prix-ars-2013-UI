#include "ofMain.h"


class ArsUIButton
{
    protected:
        int status, bcnt, speed, tcnt; //status 0: not selected, 1:selected, 2: broken 
        ofImage mark, markShadow;
        float x, y, radius;
        int bid;
        ofPoint fuji;
        double directionToFuji; //radian
        double currentDirection;
        double calcDirection(ofPoint *_p1, ofPoint *_p2);
        int    calcCompass(ofPoint *p);
        double viewAngle;
        double latitude;
        double longtitude;
        
    public:
        ArsUIButton(float _x, float _y, int _bid, ofPoint _fuji, float _angle );
        virtual void draw();
        virtual void update();
        virtual int hitTestPoint(ofPoint p);
    
        void setPosition(ofPoint pos);
        ofPoint getPosition();
    
        void setAngle(float _angle);
        int getAngle();
        double getDirectionAsRadians();
        double getDirectionAsDegrees();
        int getCompass();
        void setStatus(int _status);
        int getStatus();
        int getId();
        void setCameraStatus(double _lat, double _lon);
        ofPoint GPStoXY(double _lat,double _lon);
};
