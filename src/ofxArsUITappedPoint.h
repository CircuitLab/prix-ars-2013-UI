#include "ofMain.h"


class ofxArsUITappedPoint
{
private:
	float x, y, radius;
	float alpha;
	int tcnt,cntmax;
    int tapid;
    
public:
	ofxArsUITappedPoint(float _x, float _y, int tid);
	void draw();
	void update();
	bool alive();
};
