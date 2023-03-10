#pragma once

#include "ofMain.h"

#include "surfingMouseHandler.h"

class ofApp : public ofBaseApp{

public:
	//void setup();
	//void update();
	void draw();

	void keyPressed(int key);
	//void keyReleased(int key);

	//void mouseMoved(ofMouseEventArgs &args);
	//void mouseDragged(ofMouseEventArgs &args);
	//void mousePressed(ofMouseEventArgs &args);
	//void mouseReleased(ofMouseEventArgs &args);
	
	//void windowResized(int w, int h);
	//void dragEvent(ofDragInfo dragInfo);
	//void gotMessage(ofMessage msg);

private:
	surfingMouseHandler m;
};

