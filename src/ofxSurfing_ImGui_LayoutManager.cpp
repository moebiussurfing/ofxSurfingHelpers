#include "ofxSurfing_ImGui_LayoutManager.h"

//--------------------------------------------------------------
ofxSurfing_ImGui_LayoutManager::ofxSurfing_ImGui_LayoutManager() {
};

//--------------------------------------------------------------
ofxSurfing_ImGui_LayoutManager::~ofxSurfing_ImGui_LayoutManager() {
};

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::setup(ofxImGui::Gui & _gui) {
	gui = &_gui;

	setup_ImGui();
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::begin() {

	gui->begin();

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

	static bool auto_resize = true;
	ImGuiWindowFlags flagsw = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : ImGuiWindowFlags_None;

	ImGui::PushFont(customFont);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(ww, hh));
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::end() {

	ImGui::PopStyleVar();
	ImGui::PopFont();

	//mouse lockers
	bLockMouseByImGui = false;
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
	bLockMouseByImGui = bLockMouseByImGui | ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);

	gui->end();
}

//--------------------------------------------------------------
void ofxSurfing_ImGui_LayoutManager::setup_ImGui()
{
	ImGuiConfigFlags flags = ImGuiConfigFlags_DockingEnable;
	bool bAutoDraw = true;
	bool bRestore = true;
	bool bMouse = false;
	gui->setup(nullptr, bAutoDraw, flags, bRestore, bMouse);

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
	ofFile fileToRead(_path); // a file that exists
	bool b = fileToRead.exists();
	if (b) customFont = gui->addFont(_path + fontName, fontSizeParam, nullptr, normalCharRanges);
	io.FontDefault = customFont;

	// theme
	ofxSurfingHelpers::ImGui_ThemeMoebiusSurfing();
	//ofxSurfingHelpers::ImGui_ThemeModernDark();
}