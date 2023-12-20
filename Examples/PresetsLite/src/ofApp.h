#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"
#include "ofxSurfingPresetsLite.h"
#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void draw();

	ofxSurfingGui ui; // required to use this add-on!

	ofxSurfingPresetsLite pm; // presets manager
	string path_GLOBAL = "ofApp/";

	surfingSceneTesters sc; // scene tester
};
