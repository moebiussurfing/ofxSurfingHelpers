#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setupParams()
{
	// Parameters

	bPrevious.set("<", false);
	bNext.set(">", false);
	value.set("value", 0.f, -MAX_DIST, MAX_DIST);
	valueMin.set("valueMin", 0.f, -MAX_DIST, MAX_DIST);
	valueMax.set("valueMax", 0.f, -MAX_DIST, MAX_DIST);
	pos_1.set("pos_1", glm::vec3(0.f), glm::vec3(-MAX_DIST), glm::vec3(MAX_DIST));
	rot_1.set("rot_1", glm::vec3(0.f), glm::vec3(-2.f * MAX_DIST), glm::vec3(2.f * MAX_DIST));
	lineWidth2.set("linew2", 0.5, 0, 1);
	separation2.set("sep2", 50, 1, 100);
	shapeType2.set("shape2", 0, -50, 50);
	amount2.set("amount2", 10, 0, 25);
	speed3.set("speed3", 0.5, 0, 1);
	knob1.set("knob1", 0.5f, 0.f, 1.0f);
	knob2.set("knob2", 5.f, -10.f, 10.0f);
	shapeType3.set("shape3", 0, -50, 50);
	size3.set("size3", 100, 0, 100);
	shapeType4.set("shape4", 0, -50, 50);
	size4.set("size4", 100, 0, 100);
	lineWidth4.set("linew4", 0.5, 0, 1);
	separation4.set("sep4", 50, 1, 100);
	speed4.set("speed4", 0.5, 0, 1);
	color1.set("color1", ofColor(0, 0, 255, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255));
	color2.set("color2", ofFloatColor(0, 1, 0, 1), ofColor(0, 0, 0, 0), ofColor(1, 1, 1, 1));
	indexExpanded.set("SelectExpanded", 0, 0, 4);

	//--

	// Feed groups

	params1.setName("params1");
	params2.setName("params2");
	params3.setName("params3");
	params4.setName("params4");

	params1.add(pos_1);
	params1.add(rot_1);
	params1.add(indexExpanded);

	params2.add(lineWidth2);
	params2.add(separation2);

	params3.add(speed3);
	params3.add(shapeType3);
	params3.add(knob1);
	params3.add(knob2);
	params3.add(size3);

	params4.add(size4);
	params4.add(speed4);
	params4.add(shapeType4);
	params4.add(lineWidth4);
	params4.add(separation4);
	params4.add(color1);
	params4.add(color2);
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowPosition(-1920, 28);
	ofSetVerticalSync(true);

	setupParams();

	//--

	// API. 
	// Queue Groups.
	// How to add groups.
	// Different approaches.

	ofxSurfingHelpers::SurfDataGroupSaver d;
	d.params = params1;
	d.path = "params1_Settings.json";
	settingsManager.addGroup(d);

	settingsManager.addGroup(params2, "params2_Settings.json");

	settingsManager.addGroup(params3);
	
	settingsManager.addGroup(params4);

	settingsManager.addGroup(scene.params, "sceneParams_Settings.json");
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(scene.amount % 2 == 0 ? scene.c1 : scene.c2);
	scene.draw();

	//--

	ui.Begin();
	{
		if (ui.BeginWindow("ofApp")) 
		{
			ui.AddLabelBig("Group Params", true, true);

			ui.AddGroup(params1);
			ui.AddGroup(params2);
			ui.AddGroup(params3);
			ui.AddGroup(params4);
			ui.AddGroup(scene.params);
			ui.AddSpacingBigSeparated();

			ui.AddGroup(settingsManager.params);
			ui.AddProgressBar(settingsManager.getProgressPrc());
			
			ui.EndWindow();
		};
	}
	ui.End();
}

