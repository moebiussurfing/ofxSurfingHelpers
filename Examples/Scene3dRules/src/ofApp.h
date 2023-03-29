#pragma once

/*

	TODO:
	
	make new easyCam from RF

*/


#include "ofMain.h"

#include "ofxSurfingImGui.h"

#include "surfingScene3dRules.h"
#include "surfingMaths.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void exit();
	void draw();
	void keyPressed(int key);

	void drawImGui();
	ofxSurfingGui ui;
	ofParameter<bool> bGui{ "ofApp", true };

	ofEasyCam camera;
	ofParameter<float> scale{ "Scale Cam", 1, 0.5, 10 };
	ofParameter<float> zoom{ "Zoom Cam", 0, -1.f, 1.f };
	ofParameter<bool> bRotate{ "Rotate Cam", false };
	void doResetCamera();
	
	ofParameter<bool> bObject{ "Object", true };
	ofParameter<bool> bFlipColors{ "bFlipColors", false };

	//--

	ofParameterGroup g{ "ofApp" };

	ofxSurfingHelpers::SurfSceneGrids sceneGrid;
};
