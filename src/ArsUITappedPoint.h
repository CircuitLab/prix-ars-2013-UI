#include "ofMain.h"


class ArsUITappedPoint
{
    private:
        float x, y, radius;
        float alpha;
        int tcnt,cntmax;
        int tapid;
        unsigned long long touchStartedAt;
    
    public:
        ArsUITappedPoint(float _x, float _y, int tid, bool isTap = true);
        void draw();
        void update();
        bool alive();
        unsigned long long getTouchStartedAt();
        long getElapsedTimeFromTouchStarted();
};
