#pragma once

#include "ofMain.h"

#include "surfingParamIndexToggles.h"

#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void keyPressed(int key);

	ofxSurfingGui ui;

	// Current selected index
	ofParameter<int> index{ "Index", 0, 0, 8 }; 

	surfingParamIndexToggles pIt;

	vector<char> keyCommandsChars;
	vector<ofColor> colors;
};
