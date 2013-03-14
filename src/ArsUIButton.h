#include "ofMain.h"
#include "ofxJSONElement.h"


class ArsUIButton
{
    protected:
        string udid;
        double latitude, longitude;
        int compass;
        int angle;
        
        int status, bcnt, speed, tcnt; //status 0: not selected, 1:selected, 2: broken
        ofImage mark, markShadow;
        float x, y, radius;
        int bid;
    
        ofPoint fuji;
        double directionToFuji; //radian
        double currentDirection;
        double calcDirection(ofPoint *_p1, ofPoint *_p2);
    
        int    calcCompass(ofPoint *p);
        
    public:
        ArsUIButton(float _x, float _y, int _bid, ofPoint _fuji, float _angle, string _udid);
        ArsUIButton(double _lat, double _lon, string _udid, int _angle, int _compass, int _bid, ofPoint _fuji = ofPoint(964, 600));
        virtual void draw();
        virtual void update();
        virtual int hitTestPoint(ofPoint p);
    
        void setPosition(ofPoint pos);
        ofPoint getPosition();
    
        void setFujiPosition(ofPoint p);
    
        void setAngle(float _angle);
        int getAngle();
        double getDirectionAsRadians();
        double getDirectionAsDegrees();
        void setCompass(int degrees);
        int getCompass();
        void setStatus(int _status);
        int getStatus();
        int getId();
        void setCameraStatus(double _lat, double _lon, int compass, int angle);
        void setCameraStatus(ofxJSONElement json);
        ofPoint GPStoXY(double _lat, double _lon);
};
