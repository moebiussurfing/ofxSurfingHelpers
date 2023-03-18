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

	SurfSceneGrids sceneGrid;

	ofEasyCam camera;
	ofParameter<float> scale{ "Scale", 1, 0.5, 10 };

	void doResetCamera() {
		camera.setupPerspective();
		camera.setVFlip(0);
		camera.setPosition(100, 100, 100);
		camera.lookAt(glm::vec3(0));
		camera.disableMouseInput();
		//camera.enableMouseInput();
		camera.setFarClip(10000);
		camera.setNearClip(0);

		scale = 1;
	}
};
