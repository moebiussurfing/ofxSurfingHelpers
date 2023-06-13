#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::resetCam()
{
	cam.setupPerspective();
	cam.setNearClip(0.01f);
	cam.setFarClip(1000);
	cam.lookAt(glm::vec3(0, 0, 0));
	cam.setVFlip(0);

	if (bRF) cam.setDistance(20);
	else cam.setDistance(300);

	// Custom grid sizes
	// Example for: 
	// RF 1.0 is 5x5 meters
	if (bRF) {
		sceneGrid.stepSize = 0.5;
		sceneGrid.numberOfSteps = 10;
		sceneGrid.gridSize = 5;
	}
	else {
		sceneGrid.stepSize = 10;
		sceneGrid.numberOfSteps = 10;
		sceneGrid.gridSize = 100;
	}
}

//--------------------------------------------------------------
void ofApp::setup()
{
	// Left monitor portrait
#if 1
	ofxSurfingHelpers::setMonitorsLayout(-1, true, true);
#endif

	sceneGrid.setCameraPtr(&cam);
	sceneGrid.setPathGlobal("data");

	resetCam();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0) + " FPS");

	sceneGrid.drawBg();

	//--

	cam.begin();
	{
		// Scale
		ofScale(scale); 
		// -> Important! To fix transforms to ttf labels.
		// Camera global scale must by synced.
		 
		//--

		// 3D Object
		if (bObject)
		{
			ofPushMatrix();
			ofPushStyle();
			ofNoFill();
			ofSetColor(255, 64);
			ofRotateX(-180);
			ofRotateY(ofWrap(ofGetFrameNum(), 0, 359));
			ofSetConeResolution(3, 1, 1);
			float sz;
			if (bRF) sz = 4;
			else sz = 40;
			ofTranslate(0, -sz, 0);
			ofDrawCone(sz * 0.618, sz);
			ofPopStyle();
			ofPopMatrix();
		}

		//--

		// Scale
		sceneGrid.scale = scale; 
		// -> Important! To fix transforms to ttf labels.
		// Camera global scale must by synced.

		sceneGrid.drawInsideCam();
	}
	cam.end();

	// Disable mouse cam when over gui
	bool b = ui.isMouseOverGui();
	if (b && cam.getMouseInputEnabled()) {
		cam.disableMouseInput();
	}
	else if (!b && !cam.getMouseInputEnabled()) {
		cam.enableMouseInput();
	}

	sceneGrid.drawOutsideCam();

	//--

	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	if (!bGui) return;

	ui.Begin();
	{
		if (bGui) IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

		if (ui.BeginWindow(bGui))
		{
			ui.AddLabelBig("Scene");
			ui.Add(bObject, OFX_IM_TOGGLE_SMALL);
			ui.AddSpacingSeparated();

			if (ImGui::Button("Reset Cam"))
				resetCam();
			if (ImGui::Checkbox("RF", &bRF)) {
				resetCam();
			}
			ui.AddSpacingSeparated();

			ui.Add(scale);
			ui.Add(scale, OFX_IM_STEPPER);
			if (ui.AddButton("Reset Scale"))
				scale = 1;
			ui.AddSpacingSeparated();

			ui.Add(sceneGrid.bGui, OFX_IM_TOGGLE_ROUNDED);

			ui.EndWindow();
		}

		//--

		sceneGrid.drawImGuiWindow(ui);
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ' || key == 'g') bGui = !bGui;
	if (key == OF_KEY_RETURN) sceneGrid.bForceBitmap = !sceneGrid.bForceBitmap;
	if (key == OF_KEY_BACKSPACE) resetCam();
}