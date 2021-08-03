#pragma once

#include "ofMain.h"

#include "ofxSurfingConstants.h"

#include "ofxGui.h"

// more helpers to custpmize:
// https://github.com/openframeworks/openFrameworks/pull/6479
// https://github.com/openframeworks/openFrameworks/issues/6470

// snippets
//#include "ofxSurfing_ofxGui.h"
//ofxSurfingHelpers::setThemeDark_ofxGui();

//#define FONT_PATH_DEFAULT "assets/fonts/overpass-mono-bold.otf"
//#define FONT_SIZE_DEFAULT 9
#define FONT_PATH_DEFAULT "assets/fonts/telegrama_render.otf"
#define FONT_SIZE_DEFAULT 8

namespace ofxSurfingHelpers {

	//--------------------------------------------------------------
	// ofxGui theme
	//--------------------------------------------------------------
	inline void setThemeDark_ofxGui(std::string pathFont = FONT_PATH_DEFAULT, int size = FONT_SIZE_DEFAULT)
	{
		ofFile file(pathFont);
		if (file.exists())
		{
			ofxGuiSetFont(pathFont, size);
		}
		else {
			ofLogError(__FUNCTION__) << "file " << pathFont << " not found!";
		}

		//-

		//ofxGuiSetDefaultHeight(20);
		//ofxGuiSetBorderColor(32);
		//ofxGuiSetFillColor(ofColor(24));
		//ofxGuiSetTextColor(ofColor::white);
		//ofxGuiSetHeaderColor(ofColor(12));
		//ofxGuiSetBackgroundColor(ofColor::black);

		//-

		// based on: ImGui_ThemeMoebiusSurfingV2()

		ofFloatColor cHead = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		ofFloatColor cBg = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		ofFloatColor cBorder = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
		ofFloatColor cBut = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		ofFloatColor cSlider = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		ofFloatColor cText = ImVec4(0.76f, 0.76f, 0.76f, 0.94f);

		ofxGuiSetDefaultHeight(18);
		ofxGuiSetHeaderColor(cHead);
		ofxGuiSetBackgroundColor(cBg);
		ofxGuiSetBorderColor(cBg);
		ofxGuiSetFillColor(cSlider);
		ofxGuiSetTextColor(cText);
	}

};// ofxSurfingHelpers



// customize but using master branch
//
//public:
//	ofxPanel gui2;
//	ofParameter<float> spacing = { "spacing", 1, 0, 100 };
//	ofParameter<float> spacingNext = { "spacing next", 3, 0, 300 };
//	ofParameter<float> childrenLeftIndent = { "children Left Indent", 4, 0, 200 };
//	ofParameter<float> childrenRightIndent = { "children Right Indent", 0, 0, 200 };
//	void spacingChanged(float&);
//	ofEventListeners listeners;
//
////--------------------------------------------------------------
//void ofApp::spacingChanged(float&) {
//	ofxGuiGroup::elementSpacing = spacing;
//	ofxGuiGroup::groupSpacing = spacingNext;
//	ofxGuiGroup::childrenLeftIndent = childrenLeftIndent;
//	ofxGuiGroup::childrenRightIndent = childrenRightIndent;
//	gui_App.sizeChangedCB();
//	//{
//	//	if (key == '1') {
//	//		auto & g = gui.getGroup("params");
//	//		if (g.isMinimized()) {
//	//			g.maximize();
//	//		}
//	//		else {
//	//			g.minimize();
//	//		}
//	//	}
//	//	else if (key == '2') {
//	//		auto & g = gui.getGroup("params");
//	//		if (g.isHeaderEnabled()) {
//	//			g.disableHeader();
//	//		}
//	//		else {
//	//			g.enableHeader();
//	//		}
//	//	}
//	//	else if (key == '3') {
//	//		if (gui.isMinimized()) {
//	//			gui.maximize();
//	//		}
//	//		else {
//	//			gui.minimize();
//	//		}
//	//	}
//	//	else if (key == '4') {
//	//		if (gui.isHeaderEnabled()) {
//	//			gui.disableHeader();
//	//		}
//	//		else {
//	//			gui.enableHeader();
//	//		}
//	//	}
//	//	else if (key == '5') {
//	//		auto & g = gui.getGroup("params1");
//	//		g.setup("params1");
//	//		if (g.isMinimized()) {
//	//			g.maximize();
//	//		}
//	//		else {
//	//			g.minimize();
//	//		}
//	//	}
//	//}
//}
//
//
//gui2.setup();
//gui2.add(spacing);
//gui2.add(spacingNext);
//gui2.add(childrenLeftIndent);
//gui2.add(childrenRightIndent);
//listeners.push(spacing.newListener(this, &ofApp::spacingChanged));
//listeners.push(spacingNext.newListener(this, &ofApp::spacingChanged));
//listeners.push(childrenLeftIndent.newListener(this, &ofApp::spacingChanged));
//listeners.push(childrenRightIndent.newListener(this, &ofApp::spacingChanged));
