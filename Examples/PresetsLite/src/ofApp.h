#pragma once

#include "ofMain.h"

#include "ofxSurfingPresetsLite.h"
#include "ofxSurfingImGui.h"
#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void draw();
		void keyPressed(int key);		

		ofxSurfingGui ui; // required to use this add-on!

		ofxSurfingPresetsLite pm; // presets manager
		string path_GLOBAL = "ofApp/";

		surfingSceneTesters sc; // scene tester
};
