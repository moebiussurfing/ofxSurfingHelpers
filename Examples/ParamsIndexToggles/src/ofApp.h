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

	// Selector index
	// We will auto populate toggles for each index possible values.
	// that toggles will be exclusive: 
	// only one can be active at the same time!

	ofParameter<int> index{ "Index", 0, 0, 8 }; 

	surfingParamIndexToggles pIt;

	vector<char> keyCommandsChars;
	vector<ofColor> colors;
};
