#pragma once

#include "ofMain.h"

#include "ofxSurfingConstants.h"

#include "ofxGui.h"

// More helpers to customize:
// https://github.com/openframeworks/openFrameworks/pull/6479
// https://github.com/openframeworks/openFrameworks/issues/6470

// Example snippets
// #include "ofxSurfing_ofxGui.h"
// call before gui.setup()!
// ofxSurfingHelpers::setThemeDark_ofxGui();

namespace ofxSurfingHelpers {

//--------------------------------------------------------------
// My ofxGui Dark Theme
//--------------------------------------------------------------

//--------------------------------------------------------------
inline void setThemeDark_ofxGui(std::string pathFont = FONT_PATH_DEFAULT, int size = FONT_SIZE_DEFAULT, bool bMini = false) {

#ifdef SURFING__DISABLE_CUSTOMIZE_OFX_GUI 
	return;
#endif

	if (bMini) {
		size = 8;
		//size = 7;
	}

	ofFile file(pathFont);
	bool b = file.exists();
	if (b) {
		ofxGuiSetFont(pathFont, size);
	} else {
		ofLogWarning(__FUNCTION__) << "file " << pathFont << " not found!";
		pathFont = FONT_PATH_DEFAULT_LEGACY;
		ofLogWarning(__FUNCTION__) << "Try to load Legacy font " << pathFont << "!";

		ofFile file2(pathFont);
		b = file2.exists();
		if (b)
			ofxGuiSetFont(pathFont, size);
		else {
			ofLogError(__FUNCTION__) << "File " << pathFont << " not found!";
			ofLogError(__FUNCTION__) << "Custom font can not be loaded!";
		}
	}

	//-

	//ofxGuiSetDefaultHeight(20);
	//ofxGuiSetBorderColor(32);
	//ofxGuiSetFillColor(ofColor(24));
	//ofxGuiSetTextColor(ofColor::white);
	//ofxGuiSetHeaderColor(ofColor(12));
	//ofxGuiSetBackgroundColor(ofColor::black);

	//-

	//ofFloatColor cHead = ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f);
	ofFloatColor cHead = ofFloatColor(0.05f, 0.05f, 0.05f, 0.90f);
	//ofFloatColor cBg = ofFloatColor(0.12f, 0.12f, 0.12f, 0.8f);
	ofFloatColor cBg = ofFloatColor(0.2f, 0.2f, 0.2f, 0.8f);
	//ofFloatColor cBg = ofFloatColor(0.16f, 0.16f, 0.16f, 0.8f);
	ofFloatColor cBorder = ofFloatColor(0.10f, 0.10f, 0.10f, 0.7f);
	ofFloatColor cSlider = ofFloatColor(0.03f, 0.03f, 0.03f, 0.8f);
	ofFloatColor cText = ofFloatColor(0.76f, 0.76f, 0.76f, 0.94f);
	//ofFloatColor cBut = ofFloatColor(0.18f, 0.18f, 0.18f, 1.00f);

	ofxGuiSetHeaderColor(cHead);
	ofxGuiSetBackgroundColor(cBg);
	ofxGuiSetBorderColor(cBorder);
	ofxGuiSetFillColor(cSlider);
	ofxGuiSetTextColor(cText);

	// default
	//ofxGuiSetDefaultHeight(18);
	//ofxGuiSetTextPadding(4);

	if (bMini) {
		//ofxGuiSetDefaultHeight(19);
		//ofxGuiSetTextPadding(4);
		////ofxGuiSetDefaultHeight(18);
		////ofxGuiSetDefaultHeight(17);
		////ofxGuiSetTextPadding(8);
	} else {
		//ofxGuiSetDefaultHeight(21);
		//ofxGuiSetTextPadding(14);
	}
}

//--------------------------------------------------------------
inline void setThemeDarkMini_ofxGui() { 
	setThemeDark_ofxGui(FONT_PATH_DEFAULT, FONT_SIZE_DEFAULT, true);
}

//--------------------------------------------------------------
inline void setThemeSurfing_ofxGui(std::string pathFont = FONT_PATH_DEFAULT, int size = FONT_SIZE_DEFAULT, bool bMini = false) {

#ifdef SURFING__DISABLE_CUSTOMIZE_OFX_GUI 
	return;
#endif

	if (bMini) {
		size = 7;
	}

	ofFile file(pathFont);
	if (file.exists()) {
		ofxGuiSetFont(pathFont, size);
	} else {
		ofLogError(__FUNCTION__) << "file " << pathFont << " not found!";
	}

	//-

	ofxGuiSetBorderColor(32);
	ofxGuiSetFillColor(ofColor(48));
	ofxGuiSetTextColor(ofColor::white);
	ofxGuiSetHeaderColor(ofColor(24));

	if (bMini) {
		ofxGuiSetDefaultHeight(17);
		ofxGuiSetTextPadding(4);
		//ofxGuiSetDefaultHeight(17);
		//ofxGuiSetTextPadding(8);
	} else {
		ofxGuiSetDefaultHeight(20);
		//ofxGuiSetTextPadding(14);
	}
}

}; // ofxSurfingHelpers

//--

// App code to live customize styles,
// but using master branch
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
// Setup
//gui2.setup();
//gui2.add(spacing);
//gui2.add(spacingNext);
//gui2.add(childrenLeftIndent);
//gui2.add(childrenRightIndent);
//listeners.push(spacing.newListener(this, &ofApp::spacingChanged));
//listeners.push(spacingNext.newListener(this, &ofApp::spacingChanged));
//listeners.push(childrenLeftIndent.newListener(this, &ofApp::spacingChanged));
//listeners.push(childrenRightIndent.newListener(this, &ofApp::spacingChanged));
