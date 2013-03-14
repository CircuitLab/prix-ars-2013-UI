#include "ArsUIButton.h"
#include "ofxJSONElement.h"

class ArsUIRoboCam: public ArsUIButton
{
  private:
    string udid;
    double xpos, ypos;
    float  battery;
    bool   living;
    
  public:
    ArsUIRoboCam(float _x, float _y, int _bid, ofPoint _fuji, float angle, string _udid);
    ArsUIRoboCam(double _lat, double _lon, string _udid, int _angle, int _compass, float _battery, bool _living, int _bid, ofPoint _fuji);
    void draw();
    void update();
    
    int hitTestPoint(ofPoint p);
    void dragAngle(float _, float _y);
    bool dragAngleEnded(float _, float _y);
    void setDefaultAngle();
    void setRoboStatus(string _udid, double _lat, double _long, double _robox, double _roboy, double _buttery, bool _living );
    void setCamStatus(string jsonString);
    
    int  getBattery();
    void setBattery(int value);
    
    bool getLiving();
    void setLiving(bool value);
};
