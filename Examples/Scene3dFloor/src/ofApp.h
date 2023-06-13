#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"
#include "surfingScene3dRules.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void draw();
	void keyPressed(int key);
	void resetCam();

	void drawImGui();
	ofxSurfingGui ui;
	ofParameter<bool> bGui{ "ofApp", true };

	ofEasyCam cam;
	ofParameter<float> scale{ "Scale Cam", 1, 0.5, 10 };
	ofParameter<bool> bObject{ "Object", 0 };

	ofxSurfingHelpers::SurfSceneGrids sceneGrid;

	bool bRF = 1;
};
