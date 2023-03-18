#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::SurfSetMyMonitor(1);

	{
		float sz = 10;
		string path = FONT_FILES_PATH + string(FONT_FILE_SMALL);
		bool b = font.load(path, sz, true);
		if (!b) font.load(OF_TTF_MONO, sz, true);
	}

	doResetCamera();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofxSurfingHelpers::SurfDrawBgGradient();

	//float stepSize = 0.5f;
	float stepSize = 10;
	float numberOfSteps = 10;
	float size = numberOfSteps * stepSize;

	camera.begin();
	{
		ofScale(scale.get());

		if (1)
		{
			if (!bForceBitmap)
			{
				// custom font
				ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, true, false, true, false, &font, &camera, cLines, cText);
			}
			else
			{
				// bitmap font
				ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, true, false, true, false, nullptr, nullptr, cLines, cText);
			}
		}

		//ofDrawAxis(0.4f);
		ofDrawAxis(size / 20);

		// four squares and big floor
		ofxSurfingHelpers::SurfDrawFloor(size);
	}
	camera.end();

	// Draw labels, out of the camera
	if (!bForceBitmap)
	{
		//float s = -1;
		//if (bFixScale) s = scale.get();
		//ofxSurfingHelpers::ofxDrawGridPlaneLabels(stepSize, numberOfSteps, &font, &camera, cText, s); // custom font

		ofxSurfingHelpers::ofxDrawGridPlaneLabels(stepSize, numberOfSteps, &font, &camera, cText, scale.get()); // custom font
	}

	//--

	ui.Begin();
	{
		/* Put windows here */

		if (ui.BeginWindow(bGui))
		{
			ui.AddLabelHuge("Scene\n3dRules");
			ui.AddSpacing();

			ui.AddToggle("bForceBitmap", bForceBitmap, OFX_IM_TOGGLE_BIG);
			string s = ofToString(bForceBitmap ? "ofBitmapFont" : "ofTrueTypeFont");
			ui.AddLabelBig(s);

			ui.AddSpacingBigSeparated();

			if (ui.AddButton("Reset")) {
				scale = 1;

				doResetCamera();
			}
			ui.Add(scale);
			ui.Add(scale, OFX_IM_STEPPER);
			//ui.AddToggle("bFixScale", bFixScale);

			ui.EndWindow();
		}
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') bGui = !bGui;
	if (key == OF_KEY_RETURN) bForceBitmap = !bForceBitmap;
}