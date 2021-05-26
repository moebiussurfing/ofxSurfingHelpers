#include "ofxSurfing_ImGui_LayoutManager.h"

//--------------------------------------------------------------
ofxSurfing_ImGui_LayoutManager::ofxSurfing_ImGui_LayoutManager() {
};

//--------------------------------------------------------------
ofxSurfing_ImGui_LayoutManager::~ofxSurfing_ImGui_LayoutManager() {
};

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::setup() {//using internal instantiated gui
	setup_ImGui();
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::setup(ofxImGui::Gui & _gui) {//using external instantiated gui
	guiPtr = &_gui;

	setup_ImGui();
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::setup_ImGui()
{
	ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
	bool bAutoDraw = true;
	bool bRestore = true;
	bool bMouse = true;
	//bool bMouse = false;

	if (guiPtr != nullptr) guiPtr->setup(nullptr, bAutoDraw, flags, bRestore, bMouse);
	else gui.setup(nullptr, bAutoDraw, flags, bRestore, bMouse);

	auto &io = ImGui::GetIO();
	auto normalCharRanges = io.Fonts->GetGlyphRangesDefault();

	//-

	// font
	std::string fontName;
	float fontSizeParam;
	fontName = "telegrama_render.otf"; //  WARNING: will crash if font not present!
	fontSizeParam = 11;

	std::string _path = "assets/fonts/"; // assets folder
	ofFile fileToRead(_path); // a file that exists
	bool b = fileToRead.exists();
	if (b) {
		if (guiPtr != nullptr) customFont = guiPtr->addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
		else customFont = gui.addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
	}

	if (customFont != nullptr) io.FontDefault = customFont;

	//-

	// theme
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::begin() {
	if (guiPtr != nullptr) guiPtr->begin();
	else gui.begin();

	if (customFont != nullptr) ImGui::PushFont(customFont);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));//minimal size
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::end() {

	ImGui::PopStyleVar();//minimal size
	if (customFont != nullptr) ImGui::PopFont();

	//mouse lockers
	bLockMouseByImGui = false;
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	if (guiPtr != nullptr) guiPtr->end();
	else gui.end();
}

////--------------------------------------------------------------
//void ofxSurfing_ImGui_LayoutManager::drawRangue() {
//    static float v1 = 0;
//    static float v2 = 1;
//    static float v_min = 0;
//    static float v_max = 10;
//    bool b = ImGui::RangeSliderFloat("range", &v1, &v2, v_min, v_max);
////    if (ImGui::RangeSliderFloat("range", &v1, &v2, v_min, v_max, "(%.3f, %.3f)", 1.f))
////    {
////    }
//}

