#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//guiManager.setup(gui); // can be instantiated out of the class, localy
	guiManager.setup();

	// parameters
	params.setName("paramsGroup");// main container
	params2.setName("paramsGroup2");// nested
	params3.setName("paramsGroup3");// nested
	params.add(bPrevious.set("<", false));
	params.add(bNext.set(">", false));
	params.add(bEnable.set("Enable", false));
	params.add(lineWidth.set("lineWidth", 0.5, 0, 1));
	params.add(separation.set("separation", 50, 1, 100));
	params.add(speed.set("speed", 0.5, 0, 1));
	params.add(shapeType.set("shapeType", 0, -50, 50));
	params.add(size.set("size", 100, 0, 100));
	params.add(amount.set("amount", 10, 0, 25));
	params2.add(shapeType2.set("shapeType2", 0, -50, 50));
	params2.add(size2.set("size2", 100, 0, 100));
	params2.add(amount2.set("amount2", 10, 0, 25));
	params3.add(lineWidth3.set("lineWidth3", 0.5, 0, 1));
	params3.add(separation3.set("separation3", 50, 1, 100));
	params3.add(speed3.set("speed3", 0.5, 0, 1));
	params2.add(params3);
	params.add(params2);
}

//--------------------------------------------------------------
void ofApp::draw() {
	guiManager.begin();
	{
		auto mainSettings = ofxImGui::Settings();

		ImGuiColorEditFlags _flagw = ImGuiWindowFlags_None;
		string name = "myWindow";
		if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagw))
		{
			drawWidgets();
		}
		ofxImGui::EndWindow(mainSettings);
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofApp::drawWidgets() {

	auto mainSettings = ofxImGui::Settings();

	// typical width sizes from 1 (_w100) to 4 (_w25) widgets per row
	float _spcx;
	float _spcy;
	float _w100;
	float _h100;
	float _w99;
	float _w50;
	float _w33;
	float _w25;
	float _h;
	ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);// we will update the sizes on any gui drawing point, like inside a new foldered sub-window that could be indendeted and full size is being smaller.

	// an ofParameterGroup
	ofxImGui::AddGroup(params3, mainSettings);
	ImGui::Dummy(ImVec2(0.0f, 2.0f));// spacing

	// add big toggle full width
	ofxSurfingHelpers::AddBigToggle(bEnable, _w100, _h / 2);
	ImGui::Dummy(ImVec2(0.0f, 2.0f));// spacing

	// two buttons on same line
	ofxImGui::AddGroup(params2, mainSettings);
	if (ofxSurfingHelpers::AddBigButton(bPrevious, _w50, _h)) {
		bPrevious = false;
	}
	ImGui::SameLine();
	ofxSurfingHelpers::AddBigButton(bNext, _w50, _h);
	ImGui::Dummy(ImVec2(0.0f, 5.0f));// spacing

	// full width buttons. half height
	if (ImGui::Button("RANDOMIZE!", ImVec2(_w100, _h / 2)))
	{
	}
	if (ImGui::Button("RESET", ImVec2(_w100, _h / 2)))
	{
	}
	ofxImGui::AddGroup(params, mainSettings);

	ImGui::Dummy(ImVec2(0.0f, 2.0f));
}
