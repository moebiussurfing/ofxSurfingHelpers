#pragma once

#include "ofMain.h"

//---------
//
// OPTIONAL -> moved to ofxSurfingConstants
#include "ofxSurfingConstants.h"
//
//#define USE_IM_GUI
//#define USE_ofxGui
//#define USE_JSON		// uncomment to use default xml instead json for ofParameterGroup de/serializers
//
//---------

#ifdef USE_ofxGui
#include "ofxGui.h"
#endif
#ifdef USE_IM_GUI
#include "ofxImGui.h"
#endif

namespace ofxSurfingHelpers {
	//using namespace ofxSurfingHelpers;

	//-

	//--------------------------------------------------------------
	// circular progress bar
	//
	// example:
	//float val = ofMap(ofGetFrameNum() % 200, 0, 200, 0.f, 1.f, true);
	//ofxSurfingHelpers::drawCircleProg(val);
	//--------------------------------------------------------------
	inline void drawCircleProg(float val) {
		ofPushMatrix();
		ofPushStyle();
		ofSetLineWidth(5);

		ofRotateXDeg(-90);

		float radius = 50;
		float ratio = 0.7;

		//float val = 0.5;
		//float progress = val / 100;

		ofPoint point1(150, 120);

		ofNoFill();

		ofDrawRectangle(point1.x, point1.y, 100, 100);
		//int startAngle = -90 * 16;
		//int spanAngle = val * 360 * 16;		
		int startAngle = -90;
		int spanAngle = -90 + val * 360;
		//int spanAngle = progress * 360 * 16;

		//p.drawArc(rectangle, startAngle, spanAngle);
		//void ofPolyline_::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution=20)
		//polyline1.arc(point1, 100, 100, 0, 360);

		ofPolyline polyline1;
		polyline1.lineTo(point1.x, point1.y + radius * ratio);
		polyline1.lineTo(point1.x, point1.y + radius);
		polyline1.arc(point1, radius, radius, startAngle, spanAngle);
		polyline1.arc(point1, radius*ratio, radius*ratio, startAngle, spanAngle);
		//polyline1.lineTo(0, point1.y + radius * ratio);
		//polyline1.lineTo(0, point1.y + radius);
		ofSetColor(ofColor::blue);
		polyline1.draw();

		string str = "prog" + ofToString(val * 100);
		//string str = "prog" + ofToString(progress * 100);
		ofDrawBitmapStringHighlight(str, point1.x, point1.y + 100);

		ofPopStyle();
		ofPopMatrix();
	}

	//---

	// xml
#ifndef USE_JSON
	//--------------------------------------------------------------
	inline bool loadGroup(ofParameterGroup &g, string path)
	{
		ofLogVerbose(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();

		ofXml settings;
		bool b = settings.load(path);

		if (b) ofLogVerbose(__FUNCTION__) << "Loading: " << g.getName() << " at " << path;
		else ofLogError(__FUNCTION__) << "Error loading: " << g.getName() << " at " << path;

		ofDeserialize(settings, g);

		return b;
	}

	//--------------------------------------------------------------
	inline bool saveGroup(ofParameterGroup &g, string path)
	{
		ofLogVerbose(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();

		ofXml settings;
		ofSerialize(settings, g);
		bool b = settings.save(path);

		if (b) ofLogVerbose(__FUNCTION__) << "Save: " << g.getName() << " at " << path;
		else ofLogError(__FUNCTION__) << "Error saving: " << g.getName() << " at " << path;
		return b;
	}
#endif

	//----

#ifdef USE_JSON
	//--------------------------------------------------------------
	inline bool loadGroup(ofParameterGroup &g, string path)
	{
		ofLogVerbose(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		
		ofJson settings;
		settings = ofLoadJson(path);
		ofDeserialize(settings, g);
		bool b = !settings.is_null();//TODO:
		//bool b = true;//TODO:

		//bool b = settings.is_null;
		//if (b) ofLogVerbose(__FUNCTION__) << "Load: " << g.getName() << " at " << path;
		//else ofLogError(__FUNCTION__) << "Error loading: " << g.getName() << " at " << path;

		return b;
	}

	//--------------------------------------------------------------
	inline bool saveGroup(ofParameterGroup &g, string path)
	{
		ofLogVerbose(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		
		ofJson settings;
		ofSerialize(settings, g);
		bool b = ofSavePrettyJson(path, settings);

		if (b) ofLogVerbose(__FUNCTION__) << "Save: " << g.getName() << " at " << path;
		else ofLogError(__FUNCTION__) << "Error saving: " << g.getName() << " at " << path;

		return b;
	}
#endif


	//----

#ifdef USE_ofxGui

	//--------------------------------------------------------------
	// ofxGui theme
	//--------------------------------------------------------------
	inline void setThemeDark_ofxGui(string pathFont = "assets/fonts/overpass-mono-bold.otf")
	{
		ofFile file(pathFont);
		if (file.exists())
		{
			ofxGuiSetFont(pathFont, 9);
		}
		else {
			ofLogError(__FUNCTION__) << "file " << pathFont << " not found!";
		}
		ofxGuiSetDefaultHeight(20);
		ofxGuiSetBorderColor(32);
		ofxGuiSetFillColor(ofColor(24));
		ofxGuiSetTextColor(ofColor::white);
		ofxGuiSetHeaderColor(ofColor(12));
		ofxGuiSetBackgroundColor(ofColor::black);
	}
#endif

	//--------------------------------------------------------------
	// check if a folder path exist and creates one if not
	// why? many times when you try to save a file, this is not possible and do not happens bc the container folder do not exist
	//--------------------------------------------------------------
	inline void CheckFolder(string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		//// workaround to avoid error when folders are folder/subfolder
		//auto _fullPath = ofSplitString(_path, "/");
		//for (int i = 0; i < _fullPath.size(); i++) {
		//	ofLogNotice(__FUNCTION__) << ofToString(i) << " " << _fullPath[i];
		//}

		ofDirectory dataDirectory(ofToDataPath(_path, true));// /bin/data/

		// check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			// try to create folder
			//bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true), false, true);
			// added enable recursive to allow create nested subfolders if required

			// debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "' SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";// nothing to do
		}
	}


	///* RETURN FILE EXTENSION */
	//inline string getFileName(string _str, bool _trimExt = false) {
	//	string name = "";
	//	int slashPos = _str.rfind('/');

	//	if (slashPos != string::npos) {
	//		name = _str.substr(slashPos + 1);

	//		if (_trimExt)
	//			name = getFilePathTrimExtension(name);
	//	}

	//	return name;
	//}

	//inline string getFileFolderPath(string _str) {
	//	string name = "";
	//	int slashPos = _str.rfind('/');

	//	if (slashPos != string::npos) {
	//		name = _str.substr(0, slashPos);
	//	}

	//	return name;
	//}

	//inline string getFileExtension(string _str) {
	//	string ext = "";
	//	int extPos = _str.rfind('.');

	//	if (extPos != string::npos) {
	//		ext = _str.substr(extPos + 1);
	//	}

	//	return ext;
	//}

	//inline string getFilePathTrimExtension(string _str) {
	//	string ext = "";
	//	int extPos = _str.rfind('.');

	//	if (extPos != string::npos) {
	//		ext = _str.substr(0, extPos);
	//	}

	//	return ext;
	//}

	//inline int getNumWords(std::string str)
	//{
	//	int word_count(0);
	//	std::stringstream ss(str);
	//	std::string word;
	//	while (ss >> word) ++word_count;
	//	return word_count;
	//}


	//----


#ifdef USE_IM_GUI
	//--------------------------------------------------------------
	// ImGui Tools
	// why? my custom ImGui helpers
	//--------------------------------------------------------------
	////https://github.com/ocornut/imgui/issues/1537
	//--------------------------------------------------------------
	inline bool AddBigButton(ofParameter<bool>& parameter, float h)// button but using a bool not void param
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		float w;
		w = ImGui::GetWindowWidth()*0.9f;

		ImGuiStyle *style = &ImGui::GetStyle();

		const ImVec4 colorButton = style->Colors[ImGuiCol_Button];// better for my theme
		const ImVec4 colorHover = style->Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
		//const ImVec4 colorButton = style->Colors[ImGuiCol_ButtonHovered];// better for default theme
		//const ImVec4 colorHover = style->Colors[ImGuiCol_ButtonHovered];
		//const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];

		ImGui::PushID(name);
		ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);

		if (ImGui::Button((name), ImVec2(w, h)))
		{
			ofLogNotice(__FUNCTION__) << name << ": BANG";

			tmpRef = true;
			parameter.set(tmpRef);
		}

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		return true;//not used
	}

	//--------------------------------------------------------------
	// TODO: seems not working well linked to the param.. requires better unique name?
	inline bool AddBigToggle(ofParameter<bool>& parameter, float h)
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		//--

		// how to set colors
		//static float b = 1.0f;
		//static float c = 0.5f;
		//static int i = 3;// hue colors are from 0 to 7
		//ImVec4 _color1 = (ImVec4)ImColor::HSV(i / 7.0f, b, b);
		//ImVec4 _color2 = (ImVec4)ImColor::HSV(i / 7.0f, c, c);

		//--

		// button toggle

		float w;
		//float h;
		//h = 30;
		//w = 200;
		w = ImGui::GetWindowWidth()*0.9f;

		static bool _boolToggle = tmpRef;  // default value, the button is disabled 
		if (_boolToggle == true)// enabled
		{
			ImGuiStyle *style = &ImGui::GetStyle();
			const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorButton = style->Colors[ImGuiCol_ButtonHovered];
			const ImVec4 colorHover = style->Colors[ImGuiCol_ButtonHovered];
			ImGui::PushID(name);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);

			ImGui::Button(name, ImVec2(w, h));
			if (ImGui::IsItemClicked(0))
			{
				_boolToggle = !_boolToggle;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else// disabled
		{
			ImGuiStyle *style = &ImGui::GetStyle();
			const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorHover = style->Colors[ImGuiCol_Button];
			const ImVec4 colorButton = style->Colors[ImGuiCol_Button];//better for my theme
			//const ImVec4 colorButton = style->Colors[ImGuiCol_ButtonHovered];//better for default theme
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
			if (ImGui::Button(name, ImVec2(w, h))) {
				_boolToggle = true;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}
			ImGui::PopStyleColor(3);
		}

		//--

		// checkbox
		//ImGui::PushID(name);
		//ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
		//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, b, b));
		//ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, c, c));
		//if (ImGui::Checkbox(name, (bool *)&tmpRef))
		//	//if (ImGui::Checkbox(ofxImGui::GetUniqueName(parameter), (bool *)&tmpRef))
		//{
		//	parameter.set(tmpRef);
		//	ImGui::PopStyleColor(3);
		//	ImGui::PopID();
		//	return true;
		//}
		//ImGui::PopStyleColor(3);
		//ImGui::PopID();
		//return false;

		return true;// not used
	}

	//--------------------------------------------------------------
	inline void ImGui_FontCustom() {
		ofLogNotice(__FUNCTION__);

		ImGuiIO& io = ImGui::GetIO();
		float _size = 14.f;
		//string _name = "FiraCodeRegular.ttf";
		//float _size = 13.f;
		string _name = "overpass-mono-bold.otf";
		string _path = "assets/fonts/" + _name;//assets folder
		io.Fonts->AddFontFromFileTTF(&ofToDataPath(_path)[0], _size);
	}

	//// TODO:
	////--------------------------------------------------------------
	//inline int ImGui_ButtonsMatrix(int amountButtons, ofParameter<int> selectorTarget, string name)
	//{
	//	{
	//		int _selected = -1;

	//		//preset selector
	//		//toggle button matrix
	//		ImVec2 button_sz(40, 40);
	//		//Manually wrapping
	//		//(we should eventually provide this as an automatic layout feature, but for now you can do it manually)
	//		//ImGui::Text("PRESET SELECTOR:");
	//		ImGuiStyle& style = ImGui::GetStyle();
	//		int _amtButtons = mainGroupAmtPresetsFav;
	//		float _windowVisible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	//		for (int n = 0; n < _amtButtons; n++)
	//		{
	//			ImGui::PushID(n);
	//			//string name = ofToString((char)(keys[0][n]));
	//			//customize colors
	//			{
	//				if (PRESET_Selected_IndexMain.get() == n)//when selected
	//				{
	//					const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonHovered];//changes the color
	//					ImGui::PushStyleColor(ImGuiCol_Button, colorActive);
	//				}
	//				else {
	//					const ImVec4 colorButton = style.Colors[ImGuiCol_Button];//do not changes the color
	//					ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
	//				}
	//				//draw button
	//				if (ImGui::Button(name.c_str(), button_sz))
	//				{
	//					//loadPreset(n);//trig load preset
	//					_selected = n;
	//				}
	//				//customize colors
	//				ImGui::PopStyleColor();
	//			}
	//			float last_button_x2 = ImGui::GetItemRectMax().x;
	//			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
	//			if (n + 1 < _amtButtons && next_button_x2 < _windowVisible_x2) ImGui::SameLine();
	//			ImGui::PopID();
	//		}

	//		return _selected;
	//	}
	//}

	//--------------------------------------------------------------
	inline void ImGui_ThemeModernDark()
	{
		ofLogNotice(__FUNCTION__);

		auto& style = ImGui::GetStyle();
		style.ChildRounding = 0;
		style.GrabRounding = 0;
		style.FrameRounding = 2;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 0;
		style.TabRounding = 2;
		style.WindowRounding = 0;
		style.FramePadding = { 4, 4 };

		style.WindowTitleAlign = { 0.0, 0.5 };
		style.ColorButtonPosition = ImGuiDir_Left;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.00f };				//
		colors[ImGuiCol_TextDisabled] = { 0.25f, 0.25f, 0.25f, 1.00f };		//
		colors[ImGuiCol_WindowBg] = { 0.09f, 0.09f, 0.09f, 0.94f };			//
		colors[ImGuiCol_ChildBg] = { 0.11f, 0.11f, 0.11f, 1.00f };			//
		colors[ImGuiCol_PopupBg] = { 0.11f, 0.11f, 0.11f, 0.94f };			//
		colors[ImGuiCol_Border] = { 0.07f, 0.08f, 0.08f, 1.00f };
		colors[ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
		colors[ImGuiCol_FrameBg] = { 0.35f, 0.35f, 0.35f, 0.54f };			//
		colors[ImGuiCol_FrameBgHovered] = { 0.3f, 0.3f, 0.3f, 1.00f };
		//colors[ImGuiCol_FrameBgHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
		colors[ImGuiCol_FrameBgActive] = { 0.40f, 0.36f, 0.33f, 0.67f };
		colors[ImGuiCol_TitleBg] = { 0.1f, 0.1f, 0.1f, 1.00f };
		colors[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.00f };		//
		colors[ImGuiCol_TitleBgCollapsed] = { 0.0f, 0.0f, 0.0f, 0.61f };
		colors[ImGuiCol_MenuBarBg] = { 0.18f, 0.18f, 0.18f, 0.94f };		//
		colors[ImGuiCol_ScrollbarBg] = { 0.00f, 0.00f, 0.00f, 0.16f };
		colors[ImGuiCol_ScrollbarGrab] = { 0.24f, 0.22f, 0.21f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabHovered] = { 0.3f, 0.3f, 0.3f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
		colors[ImGuiCol_CheckMark] = { 0.84f, 0.84f, 0.84f, 1.0f };			//
		colors[ImGuiCol_SliderGrab] = { 0.4f, 0.4f, 0.4f, 1.0f };			//		
		//colors[ImGuiCol_SliderGrab] = { 0.8f, 0.8f, 0.8f, 1.0f };			//		
		colors[ImGuiCol_SliderGrabActive] = { 0.35f, 0.35f, 0.35f, 1.00f }; //
		//colors[ImGuiCol_SliderGrabActive] = { 0.55f, 0.55f, 0.55f, 1.00f }; //
		colors[ImGuiCol_Button] = { 0.55f, 0.55f, 0.55f, 0.40f };			//
		colors[ImGuiCol_ButtonHovered] = { 0.15f, 0.15f, 0.15f, 0.62f };	//	
		colors[ImGuiCol_ButtonActive] = { 0.60f, 0.60f, 0.60f, 1.00f };		//
		colors[ImGuiCol_Header] = { 0.84f, 0.36f, 0.05f, 0.0f };			//
		colors[ImGuiCol_HeaderHovered] = { 0.25f, 0.25f, 0.25f, 0.80f };	//
		colors[ImGuiCol_HeaderActive] = { 0.42f, 0.42f, 0.42f, 1.00f };
		colors[ImGuiCol_Separator] = { 0.35f, 0.35f, 0.35f, 0.50f };		//
		colors[ImGuiCol_SeparatorHovered] = { 0.3f, 0.3f, 0.3f, 0.78f };
		colors[ImGuiCol_SeparatorActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
		colors[ImGuiCol_ResizeGrip] = { 1.0f, 1.0f, 1.0f, 0.25f };			//
		colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.0f, 1.0f, 0.4f };	//
		colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.0f, 0.95f };	//
		colors[ImGuiCol_Tab] = { 0.18f, 0.18f, 0.18f, 1.0f };				//
		colors[ImGuiCol_TabHovered] = { 0.58f, 0.58f, 0.58f, 0.80f };		//
		colors[ImGuiCol_TabActive] = { 0.6f, 0.60f, 0.60f, 1.00f };
		colors[ImGuiCol_TabUnfocused] = { 0.07f, 0.10f, 0.15f, 0.97f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.14f, 0.26f, 0.42f, 1.00f };
		colors[ImGuiCol_PlotLines] = { 0.66f, 0.60f, 0.52f, 1.00f };
		colors[ImGuiCol_PlotLinesHovered] = { 0.98f, 0.29f, 0.20f, 1.00f };
		colors[ImGuiCol_PlotHistogram] = { 0.60f, 0.59f, 0.10f, 1.00f };
		colors[ImGuiCol_PlotHistogramHovered] = { 0.72f, 0.73f, 0.15f, 1.00f };
		colors[ImGuiCol_TextSelectedBg] = { 0.27f, 0.52f, 0.53f, 0.35f };
		colors[ImGuiCol_DragDropTarget] = { 0.60f, 0.59f, 0.10f, 0.90f };
		colors[ImGuiCol_NavHighlight] = { 0.51f, 0.65f, 0.60f, 1.00f };
		colors[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
		colors[ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
		colors[ImGuiCol_ModalWindowDimBg] = { 0.11f, 0.13f, 0.13f, 0.35f };
	}

	//--

	//// https://github.com/erickjung/SwiftGUI
	//// ImGui theme
	//inline void ImGui_ThemeDarcula()
	//{
	//	//		//
	//	//// Copyright (c) 2020, Erick Jung.
	//	//// All rights reserved.
	//	////
	//	//// This source code is licensed under the MIT-style license found in the
	//	//// LICENSE file in the root directory of this source tree.
	//	////
	//	//
	//	//		import Foundation
	//	//
	//	//			public class DarculaTheme : Theme {
	//	//
	//	//			public var colors : [GuiColorProperty:GuiColor]{
	//	//
	//	//				return[
	//	//					.text:.white,
	//	//					.textDisabled : GuiColor(r : 0.54, g : 0.54, b : 0.54, a : 1),
	//	//					.windowBg : GuiColor(r : 0.23, g : 0.24, b : 0.25, a : 1),
	//	//					.childBg : GuiColor(r : 0.23, g : 0.24, b : 0.25, a : 1),
	//	//					.popupBg : GuiColor(r : 0.23, g : 0.24, b : 0.25, a : 1),
	//	//					.border : GuiColor(r : 0.36, g : 0.36, b : 0.36, a : 1),
	//	//					.borderShadow : GuiColor(r : 0.15, g : 0.15, b : 0.15, a : 0),
	//	//					.frameBg : GuiColor(r : 0.27, g : 0.28, b : 0.29, a : 1),
	//	//					.frameBgHovered : GuiColor(r : 0.27, g : 0.28, b : 0.29, a : 1),
	//	//					.frameBgActive : GuiColor(r : 0.47, g : 0.47, b : 0.47, a : 0.67),
	//	//					.titleBg : GuiColor(r : 0.04, g : 0.04, b : 0.04, a : 1),
	//	//					.titleBgActive : GuiColor(r : 0, g : 0, b : 0, a : 0.51),
	//	//					.titleBgCollapsed : GuiColor(r : 0.16, g : 0.29, b : 0.48, a : 1),
	//	//					.menuBarBg : GuiColor(r : 0.27, g : 0.28, b : 0.29, a : 0.8),
	//	//					.scrollbarBg : GuiColor(r : 0.39, g : 0.4, b : 0.4, a : 0),
	//	//					.scrollbarGrab : GuiColor(r : 0.39, g : 0.4, b : 0.4, a : 1),
	//	//					.scrollbarGrabHovered : GuiColor(r : 0.39, g : 0.4, b : 0.4, a : 1),
	//	//					.scrollbarGrabActive : GuiColor(r : 0.39, g : 0.4, b : 0.4, a : 1),
	//	//					.checkMark : GuiColor(r : 0.65, g : 0.65, b : 0.65, a : 1),
	//	//					.sliderGrab : GuiColor(r : 0.7, g : 0.7, b : 0.7, a : 0.62),
	//	//					.sliderGrabActive : GuiColor(r : 0.3, g : 0.3, b : 0.3, a : 0.84),
	//	//					.button : GuiColor(r : 0.29, g : 0.31, b : 0.32, a : 1),
	//	//					.buttonHovered : GuiColor(r : 0.29, g : 0.31, b : 0.32, a : 1),
	//	//					.buttonActive : GuiColor(r : 0.21, g : 0.34, b : 0.5, a : 1),
	//	//					.header : GuiColor(r : 0.32, g : 0.33, b : 0.34, a : 1),
	//	//					.headerHovered : GuiColor(r : 0.30, g : 0.32, b : 0.32, a : 1),
	//	//					.headerActive : GuiColor(r : 0.47, g : 0.47, b : 0.47, a : 0.67),
	//	//					.separator : GuiColor(r : 0.31, g : 0.31, b : 0.31, a : 1),
	//	//					.separatorHovered : GuiColor(r : 0.31, g : 0.31, b : 0.31, a : 1),
	//	//					.separatorActive : GuiColor(r : 0.31, g : 0.31, b : 0.31, a : 1),
	//	//					.resizeGrip : GuiColor(r : 1, g : 1, b : 1, a : 0.85),
	//	//					.resizeGripHovered : GuiColor(r : 1, g : 1, b : 1, a : 0.6),
	//	//					.resizeGripActive : GuiColor(r : 0.47, g : 0.47, b : 0.47, a : 0.67),
	//	//					.tab : GuiColor(r : 0.32, g : 0.33, b : 0.34, a : 1),
	//	//					.tabHovered : GuiColor(r : 0.21, g : 0.34, b : 0.5, a : 1),
	//	//					.tabActive : GuiColor(r : 0.21, g : 0.34, b : 0.5, a : 1),
	//	//					.tabUnfocused : GuiColor(r : 0.06, g : 0.53, b : 0.98, a : 0.8),
	//	//					.tabUnfocusedActive : GuiColor(r : 0.06, g : 0.53, b : 0.98, a : 0.4),
	//	//					.plotLines : GuiColor(r : 0.61, g : 0.61, b : 0.61, a : 1),
	//	//					.plotLinesHovered : GuiColor(r : 1, g : 0.43, b : 0.35, a : 1),
	//	//					.plotHistogram : GuiColor(r : 0.9, g : 0.7, b : 0, a : 1),
	//	//					.plotHistogramHovered : GuiColor(r : 1, g : 0.6, b : 0, a : 1),
	//	//					.textSelectedBg : GuiColor(r : 0.18, g : 0.39, b : 0.79, a : 0.9),
	//	//					.modalWindowDimBg : GuiColor(r : 0.18, g : 0.39, b : 0.79, a : 1)
	//	//				]
	//	//			}
	//	//
	//	//				public var windowRounding : Float{
	//	//					return 5.3
	//	//					}
	//	//
	//	//						public var grabRounding : Float{
	//	//							return 2.3
	//	//					}
	//	//
	//	//						public var frameRounding : Float{
	//	//							return 2.3
	//	//					}
	//	//
	//	//						public var scrollbarRounding : Float{
	//	//							return 5
	//	//					}
	//	//
	//	//						public var frameBorderSize : Float{
	//	//							return 1
	//	//					}
	//	//
	//	//						public var itemSpacing : GuiPoint{
	//	//							return GuiPoint(x: 8, y : 6.5)
	//	//					}
	//	//
	//	//						public init() {}
	//	//		}

	//	auto& style = ImGui::GetStyle();
	//	style.ChildRounding = 0;
	//	style.GrabRounding = 0;
	//	style.FrameRounding = 2;
	//	style.PopupRounding = 0;
	//	style.ScrollbarRounding = 0;
	//	style.TabRounding = 2;
	//	style.WindowRounding = 0;
	//	style.FramePadding = { 4, 4 };
	//	style.WindowTitleAlign = { 0.0, 0.5 };
	//	style.ColorButtonPosition = ImGuiDir_Left;
	//	ImVec4* colors = ImGui::GetStyle().Colors;
	//	colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.00f };				//
	//	colors[ImGuiCol_TextDisabled] = { 0.25f, 0.25f, 0.25f, 1.00f };		//
	//	colors[ImGuiCol_WindowBg] = { 0.09f, 0.09f, 0.09f, 0.94f };			//
	//	colors[ImGuiCol_ChildBg] = { 0.11f, 0.11f, 0.11f, 1.00f };			//
	//	colors[ImGuiCol_PopupBg] = { 0.11f, 0.11f, 0.11f, 0.94f };			//
	//	colors[ImGuiCol_Border] = { 0.07f, 0.08f, 0.08f, 1.00f };
	//	colors[ImGuiCol_BorderShadow] = { 0.00f, 0.00f, 0.00f, 0.00f };
	//	colors[ImGuiCol_FrameBg] = { 0.35f, 0.35f, 0.35f, 0.54f };			//
	//	colors[ImGuiCol_FrameBgHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
	//	colors[ImGuiCol_FrameBgActive] = { 0.40f, 0.36f, 0.33f, 0.67f };
	//	colors[ImGuiCol_TitleBg] = { 0.1f, 0.1f, 0.1f, 1.00f };
	//	colors[ImGuiCol_TitleBgActive] = { 0.3f, 0.3f, 0.3f, 1.00f };		//
	//	colors[ImGuiCol_TitleBgCollapsed] = { 0.0f, 0.0f, 0.0f, 0.61f };
	//	colors[ImGuiCol_MenuBarBg] = { 0.18f, 0.18f, 0.18f, 0.94f };		//
	//	colors[ImGuiCol_ScrollbarBg] = { 0.00f, 0.00f, 0.00f, 0.16f };
	//	colors[ImGuiCol_ScrollbarGrab] = { 0.24f, 0.22f, 0.21f, 1.00f };
	//	colors[ImGuiCol_ScrollbarGrabHovered] = { 0.31f, 0.29f, 0.27f, 1.00f };
	//	colors[ImGuiCol_ScrollbarGrabActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
	//	colors[ImGuiCol_CheckMark] = { 0.84f, 0.84f, 0.84f, 1.0f };			//
	//	colors[ImGuiCol_SliderGrab] = { 0.8f, 0.8f, 0.8f, 1.0f };			//		
	//	colors[ImGuiCol_SliderGrabActive] = { 0.55f, 0.55f, 0.55f, 1.00f }; //
	//	colors[ImGuiCol_Button] = { 0.55f, 0.55f, 0.55f, 0.40f };			//
	//	colors[ImGuiCol_ButtonHovered] = { 0.15f, 0.15f, 0.15f, 0.62f };	//	
	//	colors[ImGuiCol_ButtonActive] = { 0.60f, 0.60f, 0.60f, 1.00f };		//
	//	colors[ImGuiCol_Header] = { 0.84f, 0.36f, 0.05f, 0.0f };			//
	//	colors[ImGuiCol_HeaderHovered] = { 0.25f, 0.25f, 0.25f, 0.80f };	//
	//	colors[ImGuiCol_HeaderActive] = { 0.42f, 0.42f, 0.42f, 1.00f };
	//	colors[ImGuiCol_Separator] = { 0.35f, 0.35f, 0.35f, 0.50f };		//
	//	colors[ImGuiCol_SeparatorHovered] = { 0.31f, 0.29f, 0.27f, 0.78f };
	//	colors[ImGuiCol_SeparatorActive] = { 0.40f, 0.36f, 0.33f, 1.00f };
	//	colors[ImGuiCol_ResizeGrip] = { 1.0f, 1.0f, 1.0f, 0.25f };			//
	//	colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.0f, 1.0f, 0.4f };	//
	//	colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.0f, 0.95f };	//
	//	colors[ImGuiCol_Tab] = { 0.18f, 0.18f, 0.18f, 1.0f };				//
	//	colors[ImGuiCol_TabHovered] = { 0.58f, 0.58f, 0.58f, 0.80f };		//
	//	colors[ImGuiCol_TabActive] = { 0.6f, 0.60f, 0.60f, 1.00f };
	//	colors[ImGuiCol_TabUnfocused] = { 0.07f, 0.10f, 0.15f, 0.97f };
	//	colors[ImGuiCol_TabUnfocusedActive] = { 0.14f, 0.26f, 0.42f, 1.00f };
	//	colors[ImGuiCol_PlotLines] = { 0.66f, 0.60f, 0.52f, 1.00f };
	//	colors[ImGuiCol_PlotLinesHovered] = { 0.98f, 0.29f, 0.20f, 1.00f };
	//	colors[ImGuiCol_PlotHistogram] = { 0.60f, 0.59f, 0.10f, 1.00f };
	//	colors[ImGuiCol_PlotHistogramHovered] = { 0.72f, 0.73f, 0.15f, 1.00f };
	//	colors[ImGuiCol_TextSelectedBg] = { 0.27f, 0.52f, 0.53f, 0.35f };
	//	colors[ImGuiCol_DragDropTarget] = { 0.60f, 0.59f, 0.10f, 0.90f };
	//	colors[ImGuiCol_NavHighlight] = { 0.51f, 0.65f, 0.60f, 1.00f };
	//	colors[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
	//	colors[ImGuiCol_NavWindowingDimBg] = { 0.80f, 0.80f, 0.80f, 0.20f };
	//	colors[ImGuiCol_ModalWindowDimBg] = { 0.11f, 0.13f, 0.13f, 0.35f };
	//}

	//--

	//// slider enum
	//// Using the format string to display a name instead of an integer.
	//// Here we completely omit '%d' from the format string, so it'll only display a name.
	//// This technique can also be used with DragInt().
	////DemoCode_("Widgets/Basic/Inputs/Slider Enum");
	//enum Element { Element_Fire, Element_Earth, Element_Air, Element_Water, Element_COUNT };
	//static int elem = Element_Fire;
	//const char* elems_names[Element_COUNT] = { "Fire", "Earth", "Air", "Water" };
	//const char* elem_name = (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown";
	//ImGui::SliderInt("slider enum", &elem, 0, Element_COUNT - 1, elem_name);
	////ImGui::SameLine(); 
	////HelpMarker("Using the format string parameter to display a name instead of the underlying integer.");

	//// drop list
	//// Using the _simplified_ one-liner Combo() api here
	//// See "Combo" section for examples of how to use the more complete BeginCombo()/EndCombo() api.
	//const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
	//static int item_current = 0;
	//ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));

	//--------------------------------------------------------------
	inline void ImGui_ThemeMoebiusSurfing()
	{
		ofLogNotice(__FUNCTION__);

		// must be done after setup the gui

		ImGuiStyle *style = &ImGui::GetStyle();

		style->FramePadding = ImVec2(4, 2);
		//style->WindowMinSize = ImVec2(160, 65);
		//style->ItemSpacing = ImVec2(6, 2);
		style->ItemSpacing = ImVec2(6, 4);
		style->ItemInnerSpacing = ImVec2(6, 4);
		style->Alpha = 1.0f;
		style->WindowRounding = 0.0f;
		style->FrameRounding = 0.0f;
		style->IndentSpacing = 6.0f;
		style->ItemInnerSpacing = ImVec2(2, 4);
		style->ColumnsMinSpacing = 50.0f;
		style->GrabMinSize = 14.0f;
		style->GrabRounding = 0.0f;
		style->ScrollbarSize = 12.0f;
		style->ScrollbarRounding = 0.0f;

		// my dark theme

		//ImVec4* colors = ImGui::GetStyle().Colors;
		style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.54f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 0.68f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.67f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.11f, 0.11f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.79f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.29f, 0.29f, 0.29f, 0.78f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.44f, 0.44f, 0.44f, 0.25f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.67f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.68f, 0.68f, 0.68f, 0.95f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.81f, 0.79f, 0.79f, 1.00f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);
		style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
		style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
		style->Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style->Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		//   //2. defaut dark theme exported
		//   style->WindowMinSize = ImVec2(160, 65);
		//   style->FramePadding = ImVec2(4, 2);
		//   style->ItemSpacing = ImVec2(6, 2);
		//   style->ItemInnerSpacing = ImVec2(6, 4);
		//   style->Alpha = 1.0f;
		//   style->WindowRounding = 0.0f;
		//   style->FrameRounding = 0.0f;
		//   style->IndentSpacing = 6.0f;
		//   style->ItemInnerSpacing = ImVec2(2, 4);
		//   style->ColumnsMinSpacing = 50.0f;
		//   style->GrabMinSize = 14.0f;
		//   style->GrabRounding = 0.0f;
		//   style->ScrollbarSize = 12.0f;
		//   style->ScrollbarRounding = 0.0f;
	}


	//----


	//--------------------------------------------------------------
	// draws a box with text
	//--------------------------------------------------------------
	inline void drawTextBoxed(ofTrueTypeFont &font, string text, int x, int y, ofColor colorText = 255, ofColor colorBackground = 0, bool useShadow = false, ofColor colorShadow = 128)
	{
		int _pad = 50;
		float _round = 5;

		ofPushStyle();
		//float fontSize = font.getSize();

		if (!font.isLoaded()) {
			ofDrawBitmapStringHighlight(text, x, y);
		}
		else {
			// bbox
			ofSetColor(colorBackground);
			ofFill();

			ofRectangle _r(font.getStringBoundingBox(text, x, y));
			_r.setWidth(_r.getWidth() + _pad);
			_r.setHeight(_r.getHeight() + _pad);
			_r.setX(_r.getPosition().x - _pad / 2.);
			_r.setY(_r.getPosition().y - _pad / 2.);

			ofDrawRectRounded(_r, _round);
			//ofDrawRectangle(_r);

			// text shadow
			if (useShadow) {
				ofSetColor(colorShadow);
				font.drawString(text, x+1, y+1);
			}

			// text
			ofSetColor(colorText);
			font.drawString(text, x, y);
		}

		ofPopStyle();
	}
	//--------------------------------------------------------------
	// get box width
	//--------------------------------------------------------------
	inline float getWidthBBtextBoxed(ofTrueTypeFont &font, string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getWidth();
	}
	inline float getHeightBBtextBoxed(ofTrueTypeFont &font, string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getHeight();
	}
#endif

};// ofxSurfingHelpers