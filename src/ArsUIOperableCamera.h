#include "ArsUICamera.h"
#include "ofxJSONElement.h"

class ArsUIOperableCamera: public ArsUICamera
{
  private:
    string udid;
    double xpos, ypos;
    float  battery;
    bool   living;
    bool   operable;
    int    eyeDirection;
    
  public:
    ArsUIOperableCamera(float _x, float _y, int _bid, ofPoint _fuji, float angle, string _udid);
    ArsUIOperableCamera(double _lat, double _lon, string _udid, int _angle, int _compass, float _battery, bool _living, int _bid, ofPoint _fuji);
    void draw();
    void update();
    
    int hitTestPoint(ofPoint p);
    
    void dragAngle(float _, float _y);
    bool dragAngleEnded(float _, float _y);
    
    void setDefaultAngle();
    
    int getEyeDirection();
    void setEyeDirection(int direction);
    
    void setRoboStatus(string _udid, double _lat, double _long, double _robox, double _roboy, double _buttery, bool _living );
    void setCamStatus(string jsonString);
    
    int  getBattery();
    void setBattery(int value);
    
    bool getLiving();
    void setLiving(bool value);
    
    void setCameraStatus(double _lat, double _lon, int compass, int angle);
    void setCameraStatus(ofxJSONElement json);
};
