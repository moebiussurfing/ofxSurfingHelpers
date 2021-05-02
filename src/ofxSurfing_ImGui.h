#pragma once

#include "ofMain.h"

#define PANEL_WIDGETS_WIDTH 250
#define PANEL_WIDGETS_HEIGHT 100
#define BUTTON_BIG_HEIGHT 50
#define BUTTON_COLOR_SIZE 40
#define BUTTON_SLIM_HEIGHT2 20
#define COLOR_STRIP_COLOR_HEIGHT 15
#define WIDGETS_HEIGHT 50

#define TEXT_INACTIVE_ALPHA 0.20f // for use on toggle buttons

/*

//-- GUIDE --//

if (ofxImGui::BeginWindow("SURFING COVERS", mainSettings, flags))
{

	float _spc = ImGui::GetStyle().ItemSpacing.x;
	float _w100 = ImGui::GetContentRegionAvail().x;
	float _w99 = _w100 - _spc;
	float _w50 = _w99 / 2;
	float _w33 = _w99 / 3 - _spc;
	float _h = BUTTON_BIG_HEIGHT;
	//if (ImGui::Button("Save Engine", ImVec2(_w33, _h / 2)))

	ofxSurfingHelpers::AddBigToggle(b1, _w100, _h);
	ofxSurfingHelpers::AddBigToggle(b2, _w100, _h);

	ImGui::PushItemWidth(-20);
	//..
	ImGui::PopItemWidth();

}
ofxImGui::EndWindow(mainSettings);

*/


//---------
//
#include "ofxSurfingConstants.h"

#include "ofxImGui.h"
#include "imgui_internal.h"

namespace ofxSurfingHelpers {

	//----

	//// 1. preset selector slider

	//auto parameter = PRESET_Selected_IndexMain;
	//auto tmpRef = PRESET_Selected_IndexMain.get();
	//auto name = ofxImGui::GetUniqueName(PRESET_Selected_IndexMain);
	//if (ImGui::SliderInt(ofxImGui::GetUniqueName(parameter), (int *)&tmpRef, parameter.getMin(), parameter.getMax()))
	//{
	//	parameter.set(tmpRef);
	//}

	//ofxImGui::AddParameter(PRESET_Selected_IndexMain);//main preset selector


	//--------------------------------------------------------------
	// ImGui Tools
	// why? my custom ImGui helpers
	//--------------------------------------------------------------
	////https://github.com/ocornut/imgui/issues/1537
	//--------------------------------------------------------------
	inline bool AddBigButton(ofParameter<bool>& parameter, float w = 100, float h = 30)// button but using a bool not void param
	{
		auto tmpRef = parameter.get();
		auto name = ofxImGui::GetUniqueName(parameter);

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

		return true;//not used
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

	// TODO: seems not working well linked to the param.. requires better unique name?
	//--------------------------------------------------------------
	inline bool AddBigToggle(ofParameter<bool>& parameter, float w = 100, float h = 30, bool border = true)
	{
		auto tmpRef = parameter.get();
		std::string name = parameter.getName();

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

		//return true;// not used
		return tmpRef;// used
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


	//TODO:
	// WIP
	//--------------------------------------------------------------
	inline void ImGui_FontCustom() {
		ofLogNotice(__FUNCTION__);
	}

	// WIP
	//--------------------------------------------------------------
	inline void ImGui_FontCustom(ofxImGui::Gui &gui, ImFont* customFont, string path = "assets/fonts/pathtelegrama_render.otf", float size = 10.f) {
		ofLogNotice(__FUNCTION__);

		auto normalCharRanges = ImGui::GetIO().Fonts->GetGlyphRangesDefault();
		customFont = gui.addFont(path, size, nullptr, normalCharRanges);
	}

	//inline void ImGui_FontCustom() {
	//ImGuiIO& io = ImGui::GetIO();
	////float _size = 14.f;
	////string _name = "FiraCodeRegular.ttf";
	////float _size = 13.f;
	////string _name = "overpass-mono-bold.otf";
	//float _size = 10.f;
	//string _name = "telegrama_render.otf";
	//string _path = "assets/fonts/" + _name;//assets folder
	//io.Fonts->AddFontFromFileTTF(&ofToDataPath(_path)[0], _size);
	//}

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

		// this must be called after the gui setup()!

		// style

		ImGuiStyle *style = &ImGui::GetStyle();

		style->FramePadding = ImVec2(6, 4);
		style->ItemInnerSpacing = ImVec2(5, 4);
		style->ItemSpacing = ImVec2(6, 4);
		style->Alpha = 0.97f;
		style->WindowRounding = 0;
		style->FrameBorderSize = 1;
		style->FrameRounding = 4;
		style->IndentSpacing = 3;
		style->ColumnsMinSpacing = 50;
		style->GrabMinSize = 18;
		style->ScrollbarSize = 14;
		style->ScrollbarRounding = 4;
		style->TabRounding = 4;
		style->WindowRounding = 2;
		style->GrabRounding = 2;

		// colors 

		//ImVec4* colors = ImGui::GetStyle().Colors;

		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.098f, 0.098f, 0.098f, 1.0f);

		// enabled is lighter
		style->Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);

		//// enabled is darker
		//style->Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		//style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);
		//style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.08f, 0.08f, 0.08f, 0.9f);

		style->Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.95f);
		//style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);

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
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.75f, 0.77f, 0.79f);

		style->Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.0f, 0.00f, 0.50f);
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

		//-

		// tabs

		style->Colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		style->Colors[ImGuiCol_TabHovered] = style->Colors[ImGuiCol_WindowBg];
		style->Colors[ImGuiCol_TabActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);
		style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);

		//-

		// docking

		style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.000f, 0.000f, 0.000f, 0.781f);
		style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	}

	//--------------------------------------------------------------
	inline void ImGui_ThemeGrey()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
		colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
		colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);

		// docking
		colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 2.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 14.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 4.0f;

	}

	//-

	//custom int params:
	//ImGui::InputInt(boxRowsUser.getName().c_str(), (int *)&boxRowsUser.get());
	//boxSizeUser = ofClamp(boxSizeUser.get(), boxSizeUser.getMin(), boxSizeUser.getMax());

	//-

	//mainSettings = ofxImGui::Settings();

	//mainSettings.windowPos = ofVec2f(gui_x, gui_y);
	//mainSettings.windowSize = ofVec2f(w_Gui, ofGetWindowHeight() - gui_y);

	//    static bool no_titlebar = false;
	//    static bool no_scrollbar = false;
	//    static bool no_menu = false;
	//    static bool no_move = false;
	//    static bool no_resize = false;
	//    static bool no_collapse = false;
	//    static bool no_close = false;
	//    static bool no_nav = false;
	//    ImGuiWindowFlags window_flags = 0;
	////    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
	////    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	////    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
	////    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	////    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	//    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
	////    if (no_nav)       window_flags |= ImGuiWindowFlags_NoNav;
	////    if (no_close)     p_open = NULL; // Don't pass our bool* to Begin
	//    if (ofxImGui::BeginWindow("COLOR PICKER", mainSettings, window_flags))




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

	// ofParameter
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
};
