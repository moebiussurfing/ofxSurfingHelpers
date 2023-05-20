#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"
#include "surfingEasyCam.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void drawScene();
	void drawGui();
	void keyPressed(int key);
	void keyReleased(int key);

	ofxSurfingGui ui;

	SurfingEasyCam cam;
};
