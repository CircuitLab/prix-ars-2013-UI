#include "ofMain.h"


class tapped{
private:
	float x,y,radius;
	float alpha;
	int tcnt,cntmax;
    int tapid;
public:
	tapped(float _x, float _y, int tid);
	void draw();
	void update();
	bool alive();
};
