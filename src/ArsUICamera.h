#include "ofMain.h"
#include "ofxJSONElement.h"


class ArsUICamera
{
    protected:
        string udid;
        double latitude, longitude;
        int compass;
        int angle;
        bool operable;
    
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
        ArsUICamera(float _x, float _y, int _bid, ofPoint _fuji, float _angle, string _udid);
        ArsUICamera(double _lat, double _lon, string _udid, int _angle, int _compass, int _bid, ofPoint _fuji = ofPoint(964, 600));
        virtual void draw();
        virtual void update();
    
        virtual int hitTestPoint(ofPoint p);
        
        void setPosition(ofPoint pos);
        ofPoint getPosition();
        
        void setFujiPosition(ofPoint p);
    
        string getUDID();
        
        void setAngle(float _angle);
        int getAngle();
    
        double getDirectionAsRadians();
        double getDirectionAsDegrees();
    
        void setCompass(int degrees);
        int getCompass();
    
        void setStatus(int _status);
        int getStatus();
    
        int getButtonId();
    
        bool getOperable();
    
        virtual void setCameraStatus(double _lat, double _lon, int compass, int angle);
        virtual void setCameraStatus(ofxJSONElement json);
    
        ofPoint GPStoXY(double _lat, double _lon);
    
        double degreesAgainstFuji();
};
