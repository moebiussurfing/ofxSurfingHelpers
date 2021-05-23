#pragma once

#include "ofMain.h"

#include "ofxImGui.h"
#include "imgui_internal.h"
#include "ofxSurfing_ImGui.h"
#include "ofxSurfing_ImGui_Themes.h"
#include "ofxSurfing_ImGui_Widgets.h"

//using namespace ImGui;
//namespace ofxSurfingHelpers
//{
class ofxSurfing_ImGui_LayoutManager
{
public:
	ofxSurfing_ImGui_LayoutManager();
	~ofxSurfing_ImGui_LayoutManager();

public:
	void setup(ofxImGui::Gui & gui);

	void setup_ImGui();
	//void draw();
	void begin();
	void end();

	ofxImGui::Gui * gui;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();
	ImFont* customFont = nullptr;
	ofParameter<bool> bGui{ "Show Gui", true };
	ofParameter<bool> auto_resize{ "Auto Resize", true };
	ofParameter<bool> bLockMouseByImGui{ "Mouse Locked", false };
	ofParameter<bool> auto_lockToBorder{ "Lock GUI", false };
};
//};