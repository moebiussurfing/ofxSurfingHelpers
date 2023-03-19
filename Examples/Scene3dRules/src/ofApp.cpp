#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetBackgroundColor(24);

	ofxSurfingHelpers::SurfSetMyMonitor(1);
	sceneGrid.setCameraPtr(&camera);

	doResetCamera();
	camera.enableMouseInput();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	sceneGrid.drawBg();

	//--

	camera.begin();
	{
		ofScale(scale.get());//need fix transforms..

		ofEnableDepthTest();

		// 3D object
		{
			ofEnableDepthTest();
			ofPushMatrix();
			ofPushStyle();
			if (0) {
				ofFill();
				ofSetColor(222, 255);
			}
			else {
				ofNoFill();
				ofSetColor(222, 32);
			}
			ofRotateX(-180);
			ofRotateY(ofWrap(ofGetFrameNum(), 0, 359));
			ofSetConeResolution(3, 1, 1);
			float sz = 40;
			ofTranslate(0, -sz, 0);
			ofDrawCone(sz * 0.618, sz);
			ofPopStyle();
			ofPopMatrix();
		}

		ofDisableDepthTest();//optional
		sceneGrid.scale = scale.get();//workaround trick
		sceneGrid.drawInsideCam();
	}
	camera.end();

	ofDisableDepthTest();//optional
	sceneGrid.drawOutsideCam();

	//--

	ofDisableDepthTest();
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	ui.Begin();
	{
		if (ui.BeginWindow(bGui))
		{
			static bool bInit = false;
			if (!bInit) {
				bInit = true;
				ui.AddStyle(sceneGrid.bEnable, OFX_IM_TOGGLE);
				ui.AddStyle(sceneGrid.bEnableBg, OFX_IM_TOGGLE);
				ui.AddStyle(sceneGrid.bForceBitmap, OFX_IM_TOGGLE_SMALL_BORDER_BLINK);
				ui.AddStyle(sceneGrid.bLabels, OFX_IM_TOGGLE_ROUNDED_SMALL);
				//ui.AddStyle(bGridInFront, OFX_IM_TOGGLE_ROUNDED_MINI);
			}

			ui.AddLabelHuge("Scene\n3dRules");
			ui.AddSpacing();

			ui.AddGroup(sceneGrid.params);
			ui.AddSpacing();

			if (ui.AddButton("Reset", OFX_IM_BUTTON, 3, true)) {
				sceneGrid.doResetSettings();
			}
			if (ui.AddButton("Reset Colors", OFX_IM_BUTTON, 3, true)) {
				sceneGrid.doResetColors();
			}
			if (ui.AddButton("Reset All", OFX_IM_BUTTON, 3, false)) {
				sceneGrid.doResetAll();
			}

			if (ui.AddButton("Save", OFX_IM_BUTTON, 2)) {
				sceneGrid.doSave();
			}
			ui.SameLine();
			if (ui.AddButton("Load", OFX_IM_BUTTON, 2)) {
				sceneGrid.doLoad();
			}
			ui.AddSpacing();

			string s = "DEBUG";
			s += "\nstepSize: " + ofToString(sceneGrid.stepSize);
			s += "\numberOfSteps: " + ofToString(sceneGrid.numberOfSteps);
			s += "\nsize: " + ofToString(sceneGrid.gridSize);
			s += "\nscale: " + ofToString(sceneGrid.scale);
			ui.AddLabel(s);
			ui.AddSpacingBigSeparated();

			//--

			ui.AddLabelBig("Scene");
			ui.Add(scale);
			ui.Add(scale, OFX_IM_STEPPER);
			if (ui.AddButton("Reset Scale")) {
				scale = 1;
			}
			ui.AddSpacingSeparated();

			ui.AddLabelBig("Camera");
			bool b = camera.getMouseInputEnabled();
			if (ui.AddButton("MouseCam", !b ? OFX_IM_BUTTON_MEDIUM : OFX_IM_BUTTON_MEDIUM_BORDER_BLINK)) {
				if (b) camera.disableMouseInput();
				else camera.enableMouseInput();
			}
			//ui.AddLabel(b ? "Enabled" : "Disabled");
			if (ui.AddButton("Reset Camera")) {
				doResetCamera();
			}

			ui.EndWindow();
		}
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') bGui = !bGui;
	if (key == OF_KEY_RETURN) sceneGrid.bForceBitmap = !sceneGrid.bForceBitmap;
	if (key == OF_KEY_LEFT_CONTROL) {
		bool b = camera.getMouseInputEnabled();
		if (b) camera.disableMouseInput();
		else camera.enableMouseInput();
	}
}