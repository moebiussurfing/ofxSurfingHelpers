#pragma once

#include "ofMain.h"

#include "surfingPresets.h"
#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp{

	public:
		
		string pathGlobal = "ofApp/";

		ofParameterGroup params;
		ofParameter<bool> bEnable;
		ofParameter<float> scale;
		ofParameter<float> thresh;
		ofParameter<float> brightness;

		ofxSurfingGui ui;

		SurfingPresets surfingPresets;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
