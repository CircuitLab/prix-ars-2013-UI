#include "ofMain.h"


class ArsUITappedPoint
{
    private:
        float x, y, radius;
        float alpha;
        int tcnt,cntmax;
        int tapid;
        
    public:
        ArsUITappedPoint(float _x, float _y, int tid);
        void draw();
        void update();
        bool alive();
};
