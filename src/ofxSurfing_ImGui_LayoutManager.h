#pragma once

#include "ofMain.h"

#include "ofxSurfing_ImGui.h"

//using namespace ImGui;
//namespace ofxSurfingHelpers
//{
class ofxSurfing_ImGui_LayoutManager
{
public:

	ofxSurfing_ImGui_LayoutManager();
	~ofxSurfing_ImGui_LayoutManager();

	void setup();
	void setup(ofxImGui::Gui & gui);
	void begin();
	void end();

private:

	void setup_ImGui();

	ofxImGui::Gui * guiPtr = NULL;
	ofxImGui::Gui gui;

	ofxImGui::Settings mainSettings = ofxImGui::Settings();
	ImFont* customFont = nullptr;

public:
	ofParameter<bool> bGui{ "Show Gui", true };
	ofParameter<bool> auto_resize{ "Auto Resize", true };//auto resize panel
	ofParameter<bool> bLockMouseByImGui{ "Mouse Locked", false };//mouse is over gui
	//ofParameter<bool> auto_lockToBorder{ "Lock GUI", false };//force position

private:
	bool bUseAdvancedSubPanel = false;//enable advanced sub panel

	//panels minimal sizes
	float xx = 10;
	float yy = 10;
	float ww = PANEL_WIDGETS_WIDTH;
	float hh = 20;
	//float hh = PANEL_WIDGETS_HEIGHT;

	/*
	//snippet
	guiManager.begin();
	{
		auto mainSettings = ofxImGui::Settings();
		ImGuiColorEditFlags _flagw = ImGuiWindowFlags_None;
		string name = "myWindow";

		////widgets sizes
		//float _spcx;
		//float _spcy;
		//float _w100;
		//float _h100;
		//float _w99;
		//float _w50;
		//float _w33;
		//float _w25;
		//float _h;

		if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagw))
		{
			ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

			//drawParams();

			guiManager.drawAdvancedSubPanel();
		}
		ofxImGui::EndWindow(mainSettings);
	}
	guiManager.end();
	*/

	//-

public:
	//--------------------------------------------------------------
	void drawAdvancedSubPanel() {
		ImGui::Dummy(ImVec2(0.0f, 2.0f));
		if (ImGui::CollapsingHeader("ADVANCED"))
		{
			ofxImGui::AddParameter(auto_resize);
			ofxImGui::AddParameter(bLockMouseByImGui);
			//ofxImGui::AddParameter(auto_lockToBorder);
		}
	}

	//--------------------------------------------------------------
	//void setUseAdvancedSubPanel(bool b) {
	//	bUseAdvancedSubPanel = b;
	//}
};
//};