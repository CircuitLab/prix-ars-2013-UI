#include "ArsUIButton.h"

class ArsUIRoboCam: public ArsUIButton
{
  private:
    string udid;
    double xpos, ypos, buttery;
    bool living;
    
  public:
    ArsUIRoboCam(float _x, float _y, int _bid, ofPoint _fuji, float angle);
    void draw();
    void update();
    
    int tapped(float _x, float _y);
    void dragAngle(float _, float _y);
    bool dragAngleEnded(float _, float _y);
    void setDefaultAngle();
    void setRoboStatus(string _udid, double _lat, double _long, double _robox, double _roboy, double _buttery, bool _living );
};
