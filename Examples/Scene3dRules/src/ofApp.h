#pragma once

/*

	TODO:
	replace cam with surfingEasyCam.h

*/


#include "ofMain.h"

#include "ofxSurfingImGui.h"

#include "surfingScene3dRules.h"
#include "surfingMaths.h"

#include "surfingEasyCam.h"


class ofApp : public ofBaseApp
{
public:
	void setup();
	void exit();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);

	void drawImGui();
	ofxSurfingGui ui;
	ofParameter<bool> bGui{ "ofApp", true };

	SurfingEasyCam cam;

	ofParameter<float> scale{ "Scale Cam", 1, 0.5, 10 };
	ofParameter<float> zoom{ "Zoom Cam", 0, -1.f, 1.f };
	ofParameter<bool> bRotate{ "Rotate Cam", false };
	
	ofParameter<bool> bObject{ "Object", true };
	ofParameter<bool> bFlipColors{ "bFlipColors", false };

	//--

	ofParameterGroup g{ "ofApp" };

	ofxSurfingHelpers::SurfSceneGrids sceneGrid;
};
