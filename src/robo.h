#include "button.h"

class robo: public button{
  private :
   
  public:
    robo(float _x, float _y, int _bid, ofPoint _fuji, float angle);
    void draw();
    void update();
    int tapped(float _x, float _y);
    void dragAngle(float _, float _y);
    bool dragAngleEnded(float _, float _y);
    void setDefaultAngle();
};
