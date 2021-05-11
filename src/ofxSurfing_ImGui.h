#pragma once

#include "ofMain.h"

//#define PANEL_WIDGETS_WIDTH 250
//#define PANEL_WIDGETS_HEIGHT 100
//#define BUTTON_BIG_HEIGHT 50
//#define BUTTON_COLOR_SIZE 40
//#define BUTTON_SLIM_HEIGHT2 20
//#define COLOR_STRIP_COLOR_HEIGHT 15
//#define WIDGETS_HEIGHT 50
//
//#define TEXT_INACTIVE_ALPHA 0.20f // for use on toggle buttons


//------------------------------
//
#include "ofxImGui.h"
#include "ofxSurfing_ImGui_Themes.h"
#include "imgui_internal.h"

namespace ofxSurfingHelpers {

	//----------------------------------------
	// snippets to include when using ofxImgui
	//----------------------------------------

	//--

	// 1. window, panels, and sub panels/trees

	/*
	//window
	ImGuiColorEditFlags _flagw = ImGuiWindowFlags_None;
	string name = "myWindow"
	if (ofxImGui::BeginWindow(name.c_str(), mainSettings, flagsw))
	{
		//..
	}
	ofxImGui::EndWindow(mainSettings);

	//tree
	if (ImGui::TreeNode("_Tree"))
	{
		//..
		ImGui::TreePop();
	}

	//collapsing
	bool bOpen = false;
	ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
	if (ImGui::CollapsingHeader("_Collapsing", _flagw))
	{
		//..
	}

	//treeEx
	bool bOpen = true;
	ImGuiColorEditFlags _flagw = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagw |= ImGuiTreeNodeFlags_Framed;
	if (ImGui::TreeNodeEx("_TreeEx", _flagw)) {
		//..
		ImGui::TreePop();
	}
	*/

	//--

	// snippets

	// copy paste all this to your ofApp

	/*
	//ofApp.h
	//#include "ofxSurfing_ImGui.h"
	void setup_ImGui();
	void draw_ImGui();
	ofxImGui::Gui gui;
	ofxImGui::Settings mainSettings = ofxImGui::Settings();
	ImFont* customFont = nullptr;
	ofParameter<bool> bGui{ "Show Gui", true };
	ofParameter<bool> auto_resize{ "Auto Resize", true };
	ofParameter<bool> bLockMouseByImGui{ "Mouse Locked", false };
	ofParameter<bool> auto_lockToBorder{ "Lock GUI", false };


	//ofApp.cpp
	setup_ImGui();
	draw_ImGui();
	//--------------------------------------------------------------
	void ofApp::setup_ImGui()
	{
		ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
		bool bAutoDraw = false;
		bool bRestore = true;
		bool bMouse = false;
		gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);

		auto &io = ImGui::GetIO();
		auto normalCharRanges = io.Fonts->GetGlyphRangesDefault();

		//-

		// font
		std::string fontName;
		float fontSizeParam;
		fontName = "telegrama_render.otf"; //  WARNING: will crash if font not present!
		fontSizeParam = 11;

		//-

		std::string _path = "assets/fonts/"; // assets folder
		customFont = gui.addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
		io.FontDefault = customFont;

		// theme
		ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
		//ofxSurfingHelpers::ImGui_ThemeModernDark();
	}

	//--------------------------------------------------------------
	void ofApp::draw_ImGui()
	{
		gui.begin();
		{
			bLockMouseByImGui = false;

			//panels sizes
			float xx = 10;
			float yy = 10;
			float ww = PANEL_WIDGETS_WIDTH;
			float hh = PANEL_WIDGETS_HEIGHT;

			//widgets sizes
			float _spcx;
			float _spcy;
			float _w100;
			float _h100;
			float _w99;
			float _w50;
			float _w33;
			float _w25;
			float _h;
			ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

			static bool auto_resize = true;

			ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

			//flagsw |= ImGuiCond_FirstUseEver;
			//if (auto_lockToBorder) flagsw |= ImGuiCond_Always;
			//else flagsw |= ImGuiCond_FirstUseEver;
			//ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsw);
			//ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsw);

			ImGui::PushFont(customFont);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));
			{
				std::string n = "myPanelName";
				if (ofxImGui::BeginWindow(n.c_str(), mainSettings, flagsw))
				{
					ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

					//ImGui::PushItemWidth(-100);
					//ofxImGui::AddParameter(_param);
					//ImGui::PopItemWidth();

					//if (ImGui::Button("_Button", ImVec2(_w100, _h / 2))) {}
					//ofxImGui::AddGroup(_group, mainSettings);
					//ofxSurfingHelpers::AddBigToggle(_param, _w100, _h);

					//ImGui::PushButtonRepeat(true);
					//float __w = ofxSurfingHelpers::getImGui_WidgetWidth(w, 2);
					//if (ImGui::Button("<", ImVec2(__w, _h))) {} ImGui::SameLine();
					//if (ImGui::Button(">", ImVec2(__w, _h))) {}
					//ImGui::PopButtonRepeat();

					//ImGui::Dummy(ImVec2(0.0f, 2.0f));

					//--

					//mouse lockers

					bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
					bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
					bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

					//ImGui::Dummy(ImVec2(0.0f, 2.0f));
					//if (ImGui::CollapsingHeader("ADVANCED"))
					//{
					//	ofxImGui::AddParameter(auto_resize);
					//	ofxImGui::AddParameter(bLockMouseByImGui);
					//	//ofxImGui::AddParameter(auto_lockToBorder);
					//}
				}
				ofxImGui::EndWindow(mainSettings);
			}
			ImGui::PopStyleVar();
			ImGui::PopFont();
		}
		gui.end();

		//mouse lockers
		bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
		bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

		gui.draw();
	}
	*/

	//-----


	//--------------------------------------------------------------
	// helpers to assist layout widget sizes to fit panel width or height
	// example: 
	// declare size vars for typical sizes 100%, 50%, 33% ..etc
	// pass external variables as references

	// snippet to use inside ImGui window/tree adapting for his shape
	/*
	float _spcx;
	float _spcy;
	float _w100;
	float _h100;
	float _w99;
	float _w50;
	float _w33;
	float _w25;
	float _h;
	ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);
	*/

	//--------------------------------------------------------------
	inline void refreshImGui_WidgetsSizes(float& __spcx, float& __spcy, float& __w100, float& __h100, float& __w99, float& __w50, float& __w33, float& __w25, float& __h)
	{
		__spcx = ImGui::GetStyle().ItemSpacing.x;
		__spcy = ImGui::GetStyle().ItemSpacing.y;
		__w100 = ImGui::GetContentRegionAvail().x;
		__h100 = ImGui::GetContentRegionAvail().y;
		__w99 = __w100 - __spcx;
		__w50 = __w100 / 2 - __spcx / 2;
		__w33 = __w100 / 3 - __spcx / 3;
		__w25 = __w100 / 4 - __spcx / 4;
		__h = BUTTON_BIG_HEIGHT;
	}

	// example: 
	// allows to make exact width of n widgets to fit panel size for two buttons:
	//float __w = getImGui_WidgetWidth(__ww, 2);
	//if (ImGui::Button("_Button", ImVec2(__w, _h))) {}
	//--------------------------------------------------------------
	inline void getImGui_WidgetWidth(float &w, int amntColumns)
	{
		float __spcx = ImGui::GetStyle().ItemSpacing.x;
		float __w100 = ImGui::GetContentRegionAvail().x;
		w = __w100 / amntColumns - __spcx / amntColumns;
	}
	//--------------------------------------------------------------
	inline void getImGui_WidgetHeight(float &h, int amntRows)
	{
		float __spcy = ImGui::GetStyle().ItemSpacing.y;
		float __h100 = ImGui::GetContentRegionAvail().y;
		h = __h100 / amntRows - __spcy / amntRows;
	}

	//----

	//--------------------------------------------------------------
	// ImGui Widgets
	// why? my custom ImGui helpers
	//--------------------------------------------------------------
	////https://github.com/ocornut/imgui/issues/1537
	//--------------------------------------------------------------
	inline bool AddBigButton(ofParameter<bool>& parameter, float w = 100, float h = 30)// button but using a bool not void param
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		//bool bChanged = false;
		bool bPre = tmpRef;

		//float w = ImGui::GetWindowWidth()*0.9f;

		if (w == -1)
		{
			w = ImGui::GetWindowContentRegionWidth() * 0.9;
			h = 70;
		}

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

		//return true;//not used

		if (bPre != parameter) return true;
		else return false;
	}

	//--------------------------------------------------------------
	inline bool AddSmallButton(ofParameter<bool>& parameter, float w = 50, float h = 20)// button but using a bool not void param
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		//float w = 3 * h;

		if (w == -1)
		{
			w = ImGui::GetWindowContentRegionWidth() * 0.85;
			h = 70;
		}

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

		return tmpRef;
	}

	//----

	//example: blink white border depending on state
	//if you want to overwrite the border color, like when blinking.. Yo need to set border to false
	//float a;
	//bool b = myToggleState.get();
	//if (b) a = ofxSurfingHelpers::getFadeBlink();
	//else a = 1.0f;
	//if (b) ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.5f, 0.0f, 1.0f, a));//white blink border
	//ofxSurfingHelpers::AddBigToggle(myToggleState, _w100, _h, false);
	//if (b) ImGui::PopStyleColor();

	// TODO: seems not working well linked to the param.. requires better unique name?
	//--------------------------------------------------------------
	inline bool AddBigToggle(ofParameter<bool>& parameter, float w = 100, float h = 30, bool border = true)
	{
		//AddBigToggle(_pTcp, _w100, _h, false);

		auto tmpRef = parameter.get();
		std::string name = parameter.getName();

		//bool bChanged = false;
		bool bPre = tmpRef;

		//--

		// how to set colors
		//static float b = 1.0f;
		//static float c = 0.5f;
		//static int i = 3;// hue colors are from 0 to 7
		//ImVec4 _color1 = (ImVec4)ImColor::HSV(i / 7.0f, b, b);
		//ImVec4 _color2 = (ImVec4)ImColor::HSV(i / 7.0f, c, c);

		//--

		ImGuiStyle *style = &ImGui::GetStyle();

		// button toggle

		// border to selected
		//ImVec4 borderLineColor{ 1,1,1,0.25 };
		const ImVec4 borderLineColor = style->Colors[ImGuiCol_Separator];

		float borderLineWidth = 1.0;
		bool bDrawBorder = false;

		bool _boolToggle = tmpRef;  // default pre value, the button is disabled 

		if (_boolToggle == true)// enabled
		{
			// border to selected
			if (_boolToggle && border)
			{
				bDrawBorder = true;
				ImGui::PushStyleColor(ImGuiCol_Border, borderLineColor);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderLineWidth);
			}

			const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorButton = style->Colors[ImGuiCol_ButtonHovered];
			const ImVec4 colorHover = style->Colors[ImGuiCol_ButtonHovered];
			ImVec4 colorHover2 = ImVec4(colorHover.x, colorHover.y, colorHover.z, colorHover.w * 0.75);

			ImGui::PushID(name.c_str());
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorHover2);
			//ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
			//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover2);

			ImGui::Button(name.c_str(), ImVec2(w, h));
			if (ImGui::IsItemClicked(0))
			{
				_boolToggle = !_boolToggle;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();

			//-

			if (bDrawBorder && border)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleVar(1);
			}
		}
		else// disabled
		{
			ImGuiStyle *style = &ImGui::GetStyle();

			const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorHover = style->Colors[ImGuiCol_Button];
			const ImVec4 colorButton = style->Colors[ImGuiCol_Button];
			ImVec4 colorTextDisabled = style->Colors[ImGuiCol_Text];
			colorTextDisabled = ImVec4(colorTextDisabled.x, colorTextDisabled.y, colorTextDisabled.z,
				colorTextDisabled.w * TEXT_INACTIVE_ALPHA);

			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorActive);
			//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
			ImGui::PushStyleColor(ImGuiCol_Text, colorTextDisabled);

			if (ImGui::Button(name.c_str(), ImVec2(w, h)))
			{
				_boolToggle = true;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}

			ImGui::PopStyleColor(4);
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



		//return tmpRef;// used
		////return true;// not used

		if (parameter.get() != bPre) return true;
		else return false;
	}

	// two states names
	//--------------------------------------------------------------
	inline bool AddBigToggleNamed(ofParameter<bool>& parameter, float w = -1, float h = -1, std::string nameTrue = "-1", std::string nameFalse = "-1")
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		//--

		//TODO:
		//blink..
		// how to set colors
		//static float b = 1.0f;
		//static float c = 0.5f;
		//static int i = 3;// hue colors are from 0 to 7
		//ImVec4 _color1 = (ImVec4)ImColor::HSV(i / 7.0f, b, b);
		//ImVec4 _color2 = (ImVec4)ImColor::HSV(i / 7.0f, c, c);

		//--

		// button toggle

		float _spc = ImGui::GetStyle().ItemInnerSpacing.x;

		if (w == -1) w = ImGui::GetWindowWidth() - 20;
		if (h == -1) h = 30;//TODO. get widget height

		if (nameTrue == "-1") nameTrue = name;
		if (nameFalse == "-1") nameFalse = name;

		//--

		// TODO: ?
		//BUG: do not reflects the correct state...

		bool _boolToggle = tmpRef;  // default value, the button is disabled 

		// enabled
		if (_boolToggle == true)
		{
			ImGuiStyle *style = &ImGui::GetStyle();
			//const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorActive = style->Colors[ImGuiCol_Separator];
			const ImVec4 colorButton = style->Colors[ImGuiCol_ButtonHovered];
			const ImVec4 colorHover = style->Colors[ImGuiCol_ButtonHovered];

			ImGui::PushID(nameTrue.c_str());
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);

			ImGui::Button(nameTrue.c_str(), ImVec2(w, h));
			if (ImGui::IsItemClicked(0))
			{
				_boolToggle = !_boolToggle;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}

			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}

		// disabled
		else
		{
			ImGuiStyle *style = &ImGui::GetStyle();
			const ImVec4 colorActive = style->Colors[ImGuiCol_ButtonActive];
			const ImVec4 colorHover = style->Colors[ImGuiCol_Button];
			const ImVec4 colorButton = style->Colors[ImGuiCol_Button];
			ImVec4 colorTextDisabled = style->Colors[ImGuiCol_Text];
			//colorTextDisabled = ImVec4(colorTextDisabled.x, colorTextDisabled.y, colorTextDisabled.z, 
			//	colorTextDisabled.w * TEXT_INACTIVE_ALPHA);

			ImGui::PushID(nameTrue.c_str());
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
			ImGui::PushStyleColor(ImGuiCol_Button, colorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
			ImGui::PushStyleColor(ImGuiCol_Text, colorTextDisabled);

			if (ImGui::Button(nameFalse.c_str(), ImVec2(w, h))) {
				_boolToggle = true;
				tmpRef = _boolToggle;
				parameter.set(tmpRef);
			}

			ImGui::PopStyleColor(3);
			//ImGui::PopStyleColor(4);
			ImGui::PopID();
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
	inline bool AddIntStepped(ofParameter<int>& parameter)
	{
		bool bChanged = false;
		auto tmpRefi = parameter.get();
		const ImU32 u32_one = 1;
		static bool inputs_step = true;
		if (ImGui::InputScalar(parameter.getName().c_str(), ImGuiDataType_U32, (int *)&tmpRefi, inputs_step ? &u32_one : NULL, NULL, "%u"))
		{
			tmpRefi = ofClamp(tmpRefi, parameter.getMin(), parameter.getMax());
			parameter.set(tmpRefi);

			bChanged = true;
		}
		return bChanged;
	}

	//--------------------------------------------------------------
	inline bool AddBigSlider(ofParameter<float>& parameter, float w = 100, float h = 30)// button but using a bool not void param
	{
		bool bChanged = false;
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

		//float w = ImGui::GetWindowWidth()*0.9f;

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

		//TODO:

		//ImGui::SliderFloat(name, &parameter.get(), parameter.getMin(), parameter.getMax(), "ratio = %.3f");
		//if (ImGui::SliderFloat((name), parameter.get(),  parameter.getMin(), parameter.getMax(), ImVec2(w, h)))
		{
			ofLogNotice(__FUNCTION__) << name << ": BANG";

			tmpRef = parameter.get();
			parameter.set(tmpRef);

			bChanged = true;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopID();

		return bChanged;
	}


	//--

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
	//			//string name = ofToString((char)(bKeys[0][n]));
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
	//					//populatePreset(n);//trig load preset
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

	//--

	// spinner progress indicator
	//https://github.com/ocornut/imgui/issues/1901
	//https://discourse.dearimgui.org/t/spinner-modified-code-from-issue-1901/22

	//namespace ImGui {
	//
	//	bool BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
	//		ImGuiWindow* window = GetCurrentWindow();
	//		if (window->SkipItems)
	//			return false;
	//
	//		ImGuiContext& g = *GImGui;
	//		const ImGuiStyle& style = g.Style;
	//		const ImGuiID id = window->GetID(label);
	//
	//		ImVec2 pos = window->DC.CursorPos;
	//		ImVec2 size = size_arg;
	//		size.x -= style.FramePadding.x * 2;
	//
	//		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	//		ItemSize(bb, style.FramePadding.y);
	//		if (!ItemAdd(bb, id))
	//			return false;
	//
	//		// Render
	//		const float circleStart = size.x * 0.7f;
	//		const float circleEnd = size.x;
	//		const float circleWidth = circleEnd - circleStart;
	//
	//		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
	//		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);
	//
	//		const float t = g.Time;
	//		const float r = size.y / 2;
	//		const float speed = 1.5f;
	//
	//		const float a = speed * 0;
	//		const float b = speed * 0.333f;
	//		const float c = speed * 0.666f;
	//
	//		const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
	//		const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
	//		const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;
	//
	//		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
	//		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
	//		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);
	//	}
	//
	//	bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
	//		ImGuiWindow* window = GetCurrentWindow();
	//		if (window->SkipItems)
	//			return false;
	//
	//		ImGuiContext& g = *GImGui;
	//		const ImGuiStyle& style = g.Style;
	//		const ImGuiID id = window->GetID(label);
	//
	//		ImVec2 pos = window->DC.CursorPos;
	//		ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);
	//
	//		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	//		ItemSize(bb, style.FramePadding.y);
	//		if (!ItemAdd(bb, id))
	//			return false;
	//
	//		// Render
	//		window->DrawList->PathClear();
	//
	//		int num_segments = 30;
	//		int start = abs(ImSin(g.Time*1.8f)*(num_segments - 5));
	//
	//		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	//		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;
	//
	//		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);
	//
	//		for (int i = 0; i < num_segments; i++) {
	//			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
	//			window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
	//				centre.y + ImSin(a + g.Time * 8) * radius));
	//		}
	//
	//		window->DrawList->PathStroke(color, false, thickness);
	//	}
	//
	//}
		//namespace ImGui 
		//{
	//using ComVec4 = const ImVec4 &;
	//auto Spinner(float radius, float thickness, int num_segments, float speed, ComVec4 color) -> void {
	//	//auto ImGui::Spinner(float radius, float thickness, int num_segments, float speed, ComVec4 color) -> void {
	//	auto window = GetCurrentWindow();
	//	if (window->SkipItems)
	//		return;
	//
	//	auto &g = *GImGui;
	//	const auto &style = g.Style;
	//	auto &&pos = ImGui::GetCursorPos();
	//	ImVec2 size{ radius * 2, radius * 2 };
	//	const ImRect bb{ pos, pos + size };
	//	ItemSize(bb);
	//	if (!ItemAdd(bb, 0))
	//		return;
	//
	//	auto time = static_cast<float>(g.Time) * speed;
	//	window->DrawList->PathClear();
	//	int start = static_cast<int>(abs(ImSin(time) * (num_segments - 5)));
	//	const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
	//	const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;
	//	const auto &&centre = pos + radius;
	//	for (auto i = 0; i < num_segments; i++) {
	//		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
	//		window->DrawList->PathLineTo({ centre.x + ImCos(a + time * 8) * radius,
	//									  centre.y + ImSin(a + time * 8) * radius });
	//	}
	//	window->DrawList->PathStroke(GetColorU32(color), false, thickness);
	//}

	//}

	//cute widgets !
	//https://github.com/soufianekhiat/DearWidgets

	//spin arrows widget
	//https://github.com/ocornut/imgui/issues/2649

	//spinners, loading bars 
	//https://github.com/ocornut/imgui/issues/1901#issuecomment-552185000
	//https://github.com/ocornut/imgui/issues/1901

	//Rotating text and icon demo for dear imgui 
	//https://gist.github.com/ice1000/865c7666d13b945628254aa00bd9d62d


	// rounded toggle buttons: bool & ofParameter<bool>
	//--------------------------------------------------------------
	inline void ToggleButton(const char* str_id, bool* v)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float height = ImGui::GetFrameHeight();
		float width = height * 1.55f;
		float radius = height * 0.50f;

		ImGui::InvisibleButton(str_id, ImVec2(width, height));
		if (ImGui::IsItemClicked()) *v = !*v;
		ImGuiContext& gg = *GImGui;
		float ANIM_SPEED = 0.085f;
		if (gg.LastActiveId == gg.CurrentWindow->GetID(str_id))// && g.LastActiveIdTimer < ANIM_SPEED)
			float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);
		if (ImGui::IsItemHovered())
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonActive] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
		else
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
		draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

		ImGui::SameLine();
		ImGui::Text(str_id);
	}

	// ofParameter bool toggle
	//--------------------------------------------------------------
	inline bool AddToggleRounded(ofParameter<bool>& parameter, ImVec2 v = ImVec2(-1, -1))
	{
		auto tmpRef = parameter.get();
		std::string name = parameter.getName();

		ImVec4* colors = ImGui::GetStyle().Colors;
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		float width;
		float radius;
		float height;

		if (v.x == -1 && v.y == -1)
		{
			height = ImGui::GetFrameHeight();
			width = height * 1.55f;
			radius = height * 0.50f;
		}
		else
		{
			width = v.x;
			radius = v.y * 0.5f;
			height = v.y;
		}

		ImGui::InvisibleButton(name.c_str(), ImVec2(width, height));
		if (ImGui::IsItemClicked())
		{
			tmpRef = !tmpRef;

			parameter.set(tmpRef);
		}

		ImGuiContext& gg = *GImGui;
		float ANIM_SPEED = 0.085f;
		if (gg.LastActiveId == gg.CurrentWindow->GetID(name.c_str()))// && g.LastActiveIdTimer < ANIM_SPEED)
			float t_anim = ImSaturate(gg.LastActiveIdTimer / ANIM_SPEED);

		if (ImGui::IsItemHovered())
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height),
				ImGui::GetColorU32(tmpRef ?
					colors[ImGuiCol_ButtonActive] : colors[ImGuiCol_ButtonHovered]), height * 0.5f);
		else
			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height),
				ImGui::GetColorU32(tmpRef ?
					colors[ImGuiCol_ButtonActive] : colors[ImGuiCol_ButtonHovered]), height * 0.5f);

		//draw_list->AddCircleFilled(ImVec2(p.x + radius + (tmpRef ? 1 : 0) * (width - radius * 2.0f), 
		//	p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

		ImU32 c1 = IM_COL32(255 * colors[ImGuiCol_Button].x, 255 * colors[ImGuiCol_Button].y, 255 * colors[ImGuiCol_Button].z, 255);

		draw_list->AddCircleFilled(ImVec2(p.x + radius + (tmpRef ? 1 : 0) * (width - radius * 2.0f),
			p.y + radius), radius - 1.5f, c1);

		//--

		//label

		//// Render
		//const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		//RenderNavHighlight(bb, id);
		//RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

		//ImVec2 pos = window->DC.CursorPos;
		//if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		//	pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
		//ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		//const ImRect bb(pos, pos + size);
		//ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		//ImGui::RenderTextClipped(.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

		//ImGui::SameLine(0, 10);
		//ImGui::SameLine(0, 0.5);
		ImGui::SameLine();
		//ImGui::Dummy(ImVec2(-1.0f, 10.0f));
		ImGui::Text(name.c_str());


		//TODO. aligning
		//const char* fmt;
		//va_list args;
		//ImVec2 size_arg(100, 30);
		//ImVec2 align(0, 0);

		//ImGuiWindow* window = ImGui::GetCurrentWindow();

		////ImGuiContext& g = *GetCurrentContext();
		////
		////auto g = ImGui::GetStyle();
		////const ImGuiStyle& style = g.Style;
		////const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
		////const ImVec2 label_size = ImGui::CalcTextSize(g.TempBuffer, text_end, true, 0.0f);
		////const ImGuiID id = window->GetID(g.TempBuffer, text_end);

		//ImVec2 pos = window->DC.CursorPos;
		////if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
		//	//pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
		//ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);


		//const ImRect bb(pos, pos + size);
		//ImGui::ItemSize(bb, style.FramePadding.y);
		//ImGui::RenderTextClipped(bb.Min, bb.Max, g.TempBuffer, text_end, &label_size, align);


		return tmpRef;// used
	}


	//https://gist.github.com/nariakiiwatani/dabf4cd2d04ad015bb6fabdedef7b2aa
	//namespace ImGui
	//{
	//	static bool SelectFile(const std::string &path, std::string &selected, const std::vector<std::string> &ext = {}) {
	//		bool ret = false;
	//		if (ofFile(path).isDirectory()) {
	//			if (TreeNode(ofFilePath::getBaseName(path).c_str())) {
	//				ofDirectory dir;
	//				if (!ext.empty()) {
	//					dir.allowExt("");
	//					for (auto &&e : ext) {
	//						dir.allowExt(e);
	//					}
	//				}
	//				dir.listDir(path);
	//				for (auto &f : dir) {
	//					ret |= SelectFile(f.path(), selected, ext);
	//				}
	//				TreePop();
	//			}
	//		}
	//		else if (Button(ofFilePath::getFileName(path).c_str())) {
	//			selected = path;
	//			ret = true;
	//		}
	//		return ret;
	//	}
	//}

	//TODO:
	//BUG:
	//when drawing two plots both are sharing variables ?? so only drawing one
	//--------------------------------------------------------------
	inline void AddPlot(ofParameter<float>& parameter, ImVec2 v2 = ImVec2(100, 80.0f))
	{
		ImGui::PushID(1);

		static float min = parameter.getMin();
		static float max = parameter.getMax();
		static std::string name = parameter.getName();
		//ImGui::Text(name.c_str());

		static bool animate = true;

		// Fill an array of contiguous float values to plot
		// Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float
		// and the sizeof() of your structure in the "stride" parameter.

		//static const size_t duration = 60 * 4;//fps * secs
		//static float values[duration] = {};
		static float values[120] = {};

		static int values_offset = 0;
		static double refresh_time = 0.0;

		if (!animate || refresh_time == 0.0)
			refresh_time = ImGui::GetTime();
		while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
		{
			static float phase = 0.0f;
			values[values_offset] = parameter.get();
			//values[values_offset] = cosf(phase);

			values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
			phase += 0.10f * values_offset;
			refresh_time += 1.0f / 60.0f;
		}

		static bool bOverlayAvg = false;
		static char overlay[32] = "";
		if (bOverlayAvg)
		{
			float average = 0.0f;
			for (int n = 0; n < IM_ARRAYSIZE(values); n++)
				average += values[n];
			average /= (float)IM_ARRAYSIZE(values);
			sprintf(overlay, "avg %f", average);
		}

		//ImGui::PushID(1);
		//ImGui::PlotLines("Plot", values, IM_ARRAYSIZE(values), values_offset, overlay, min, max, v2);
		ImGui::PlotLines(name.c_str(), values, IM_ARRAYSIZE(values), values_offset, overlay, min, max, v2);

		//ImGui::PushID(1);
		ImGui::Checkbox("Animate", &animate);

		ImGui::PopID();
	}
};
