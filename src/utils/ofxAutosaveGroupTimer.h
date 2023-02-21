
#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"

namespace ofxSurfingHelpers
{
	struct SurfDataGroupSaver
	{
		string path = "";
		ofParameterGroup params;

		//TODO:
		//ofParameter<int> timeToAutosave;
		//ofParameter<bool> bAutoSave;
	};

} // namespace ofxSurfingHelpers 

class ofxAutosaveGroupTimer
{

public:

	ofxAutosaveGroupTimer();
	~ofxAutosaveGroupTimer();

	void addGroup(ofxSurfingHelpers::SurfDataGroupSaver data);

	ofParameter<bool> bAutoSave;
	ofParameter<int> timeToAutosave;

	void setPathGlobal(string path) {//call before setup or adding param groups
		path_Global = path;
		ofxSurfingHelpers::CheckFolder(path_Global);
	};

	ofParameterGroup params;

	float getProgressPrc() const { return progressPrc; };

private:

	float progressPrc;
	bool bDoneStartup = false;

	void startup();
	void setup();
	void exit();

	string fileExtension = ".json";
	string name_Settings = "Settings_ofxAutosaveGroupTimer";

	string path_Global = "ofxAutosaveGroupTimer/";

	vector<ofxSurfingHelpers::SurfDataGroupSaver> data;

	// auto save
	uint64_t timerLast_Autosave = 0;

	//// easy callback
	//bool bMustUptate = false;
	//bool isTimedOut() {
	//	if (bMustUptate) {
	//		bMustUptate = false;//already checked. disable
	//		return true;
	//	}
	//	else return false;
	//}

	void update(ofEventArgs& args);

	void saveAllGroups();
};

//} // namespace ofxSurfingHelpers 
