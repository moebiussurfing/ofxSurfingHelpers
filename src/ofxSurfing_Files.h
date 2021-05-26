#pragma once

#include "ofMain.h"

namespace ofxSurfingHelpers {

//--------------------------------------------------------------------------------
inline bool ofxKuFileExists( string fileName )
{
	fileName = ofToDataPath(fileName);
	ifstream inp;
	inp.open(fileName.c_str(), ifstream::in);
	inp.close();
	return !inp.fail();
}	

};// ofxSurfingHelpers
