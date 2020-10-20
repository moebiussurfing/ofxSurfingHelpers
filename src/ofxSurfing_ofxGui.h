#pragma once

#include "ofMain.h"

#include "ofxSurfingConstants.h"

#include "ofxGui.h"

namespace ofxSurfingHelpers {
	//using namespace ofxSurfingHelpers;

	//--------------------------------------------------------------
	// ofxGui theme
	//--------------------------------------------------------------
	inline void setThemeDark_ofxGui(std::string pathFont = "assets/fonts/overpass-mono-bold.otf", int size = 9)
	{
		ofFile file(pathFont);
		if (file.exists())
		{
			ofxGuiSetFont(pathFont, size);
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

};// ofxSurfingHelpers