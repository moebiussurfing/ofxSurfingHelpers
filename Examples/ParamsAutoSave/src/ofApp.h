#pragma once


/*
* 
* ofxAutosaveGroupTimer.h
* This class is an auto saver for ofParameterGroups.
* Each added group will be (his params) saved/serialized into his passed path.
* Will save on exit too even when auto save is disabled.
* Timer settings like enabled or waiting time is saved too.
* 
*/


#include "ofMain.h"

//#include "ofxSurfingHelpers.h"
#include "ofxAutosaveGroupTimer.h"

#include "ofxSurfingImGui.h" // Not required to use this class!
#include "surfingSceneTesters.h"
#define MAX_DIST 500.0f

class ofApp : public ofBaseApp{

	public:

		void setup();
		void draw();
		void keyPressed(int key);		

		ofxSurfingGui ui; // Not required to use this class!

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

		surfingSceneTesters sc; // scene tester to pass another group param

		// Manager
		ofxAutosaveGroupTimer autosaver;
};
