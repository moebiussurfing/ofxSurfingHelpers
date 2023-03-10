
#pragma once


/*
*
* ofxAutosaveGroupTimer.h
* This class is an auto saver for ofParameterGroups.
* Each added group will be (his params) saved/serialized into his passed path.
* Will save on exit too even when auto save is disabled.
* Timer settings like enabled or waiting time is saved too.
*
*/


#include "ofMain.h"

#include "ofxSurfingHelpers.h"

namespace ofxSurfingHelpers
{
	struct SurfDataGroupSaver
	{
		string path = "";
		ofParameterGroup params;

		//TODO:
		// each group could have his own timer.
		// that could avoid storing groups that do not change too often.
		// and could be more per formant 
		// bc all files are not being saved at the exact same time!
		//ofParameter<int> timeToAutosave;
		//ofParameter<bool> bAutoSave;
		// could allow disable each group independently!
	};

} // namespace ofxSurfingHelpers 

class ofxAutosaveGroupTimer
{

public:
	ofxAutosaveGroupTimer();
	~ofxAutosaveGroupTimer();

	void addGroup(ofxSurfingHelpers::SurfDataGroupSaver data);
	void addGroup(ofParameterGroup params, string path);
	void addGroup(ofParameterGroup params);

private:
	ofParameterGroup params;

public:
	void setPathGlobal(string path) { // call before setup or adding param groups
		path_Global = path;
		ofxSurfingHelpers::CheckFolder(path_Global);
	};
	float getProgressPrc() const { return progressPrc; };

	//private:
	void startup();//public to allow callback manually.
	//if not, will be auto called on first frame in update!

	void saveAllGroups();

private:
	ofParameter<bool> bAutoSave;
	ofParameter<bool> bSilent;//not log for every param
	ofParameter<int> timeToAutosave;//period

	void setup();
	void update(ofEventArgs& args);
	void exit();

	uint64_t timerLast_Autosave = 0;
	float progressPrc;
	bool bDoneStartup = false;

	string path_Global = "ofxAutosaveGroupTimer/";
	string name_Settings = "ofxAutosaveGroupTimer_Settings";
	string fileExtension = ".json";

	vector<ofxSurfingHelpers::SurfDataGroupSaver> data;

	//workaround
	// a bit of offset to make many instances to not happen at the same time..
	bool bRandomOffset = true;
	int tOffset = 0;

	int count = 0;

};
