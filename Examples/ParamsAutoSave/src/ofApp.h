#pragma once

#include "ofMain.h"

#include "ofxAutosaveGroupTimer.h"

#include "ofxSurfingImGui.h"
#include "surfingSceneTesters.h"
#define MAX_CAMERA_DISTANCE 500.0f

class ofApp : public ofBaseApp{

	public:

		void setup();
		void draw();
		void keyPressed(int key);		

		ofxSurfingGui ui; // Not required to use this class!

		string path_GLOBAL = "ofApp/";

		surfingSceneTesters sc; // scene tester

		void setupParams();

		// Groups
		ofParameterGroup params1;
		ofParameterGroup params2;
		ofParameterGroup params3;
		ofParameterGroup params4;

		// Parameters
		ofParameter<glm::vec3> rot_1;
		ofParameter<glm::vec3> pos_1;
		ofParameter<float> value;
		ofParameter<float> valueMin;
		ofParameter<float> valueMax;
		ofParameter<bool> bPrevious;
		ofParameter<bool> bNext;
		ofParameter<float> knob1;
		ofParameter<float> knob2;
		ofParameter<int> amount2;
		ofParameter<float> lineWidth2;
		ofParameter<float> separation2;
		ofParameter<int> shapeType2;
		ofParameter<float> speed3;
		ofParameter<int> shapeType3;
		ofParameter<int> size3;
		ofParameter<int> shapeType4;
		ofParameter<int> size4;
		ofParameter<float> lineWidth4;
		ofParameter<float> separation4;
		ofParameter<float> speed4;
		ofParameter<ofColor> color1;
		ofParameter<ofFloatColor> color2;
		ofParameter<int> indexExpanded;

		ofxAutosaveGroupTimer autosaver;
};
