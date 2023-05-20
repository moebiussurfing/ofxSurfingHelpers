#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	cam.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(48);

	ofEnableDepthTest();
	cam.begin();
	drawScene();
	cam.end();

	ofDisableDepthTest();
	cam.drawInteractionArea();
	cam.drawHelpText();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	ofPushStyle();
	ofSetConeResolution(20, 2);
	ofSetCylinderResolution(20, 2);
	ofSetColor(ofColor::red);//RIGHT
	ofDrawCone(100, 0, 0, 50, 100);

	ofSetColor(ofColor::white);//LEFT
	ofDrawSphere(-100, 0, 0, 50);

	ofSetColor(ofColor::blue);//BOTTOM
	ofDrawBox(0, 100, 0, 100);

	ofSetColor(ofColor::cyan);//TOP
	ofDrawCylinder(0, -100, 0, 50, 100);

	ofSetColor(ofColor::yellow);//FRONT
	ofDrawBox(0, 0, 100, 100);

	ofSetColor(ofColor::magenta);//BACK
	ofDrawBox(0, 0, -100, 100);

	ofDrawGrid(20, 10, true, true, true, true);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGui()
{
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp"))
		{
			ui.AddMinimizerToggle();
			if (ui.isMaximized()) {
				ui.Add(ui.bAutoResize, OFX_IM_TOGGLE_ROUNDED_MINI);
			}
			ui.AddSpacingSeparated();
			ui.Add(cam.bGui, OFX_IM_TOGGLE_ROUNDED);
			ui.EndWindow();
		};

		cam.drawImGui(ui);
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	cam.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	cam.keyReleased(key);
}

