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

	ofxSurfingGui ui;
	ofParameter<bool> bGui{ "ofApp", true };

	ofParameter<float> scale{ "Scale", 1, 0.5, 10 };
	//bool bFixScale = 0;

	ofEasyCam camera;

	bool bForceBitmap = 0;
	ofTrueTypeFont font;
	ofColor cText = ofColor{ 255, 255, 255, 150 }; // text
	ofColor cLines = ofColor{ 96, 96, 96, 16 }; // lines

	void doResetCamera() {
		camera.setupPerspective();
		camera.setVFlip(0);
		camera.setPosition(100, 100, 100);
		camera.lookAt(glm::vec3(0));
		camera.enableMouseInput();
		camera.setFarClip(10000);
		camera.setNearClip(0);
	}
};
