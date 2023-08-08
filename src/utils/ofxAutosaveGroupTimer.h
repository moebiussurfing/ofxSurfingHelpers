
#pragma once
#include "ofMain.h"

/*
*
* ofxAutosaveGroupTimer.h
* This class is an auto saver for ofParameterGroups.
* Each added group (and his params) will be saved/serialized to a passed file path.
*
*/

#include "ofxSurfingHelpers.h"

namespace ofxSurfingHelpers
{
	struct SurfDataGroupSaver
	{
		string path = "";
		ofParameterGroup params;
	};

}

//--

class ofxAutosaveGroupTimer
{

public:
	ofxAutosaveGroupTimer();
	~ofxAutosaveGroupTimer();

	void addGroup(ofxSurfingHelpers::SurfDataGroupSaver data);
	void addGroup(ofParameterGroup &params, string path);
	void addGroup(ofParameterGroup &params);

public:
	ofParameterGroup params;//internal params
private:
	void Changed_Params(ofAbstractParameter& e);
	void saveParams();
	bool bFlagSaveParams = 0;

private:
	ofParameterGroup paramsQueue;//added params
	bool bFlagSaveQueued = 0;
	ofEventListener eParamsQueue;

	//TODO: improve by saving only the group that changed
	//ofEventListeners esParamsQueue;

	uint64_t timerLast_AutoSaveQueuedOnChange = 0;
	uint64_t timeGap = 250;
	//uint64_t timeGap = 5000;//debug

public:
	// call before setup or adding param groups
	void setPathGlobal(string path) { 
		path_Global = path;
		ofxSurfingHelpers::CheckFolder(path_Global);
	};
	float getProgressPrc() const { return progressPrc; };

	//private:
	void startup();
	// public to allow call manually.
	// if not called, it will be auto called on first frame in update!

	void saveParamsQueued();

private:

	ofParameter<bool> bAutoSaveOnChange;
	ofParameter<bool> bAutoSaveTimer;
	ofParameter<bool> bSilent;//not log for every param
	ofParameter<int> timePeriodToAutosave;//period in seconds

	void setup();
	void update(ofEventArgs& args);
	void exit(ofEventArgs& args);
	void exit();

	uint64_t timerLast_AutosaveTimer = 0;

	float progressPrc;
	bool bDoneStartup = false;

	string path_Global = "ofxAutosaveGroupTimer/";
	string name_Settings = "ofxAutosaveGroupTimer_Settings";
	string fileExtension = ".json";

	vector<ofxSurfingHelpers::SurfDataGroupSaver> data;

	size_t count = 0;

	////TODO
	//// workaround
	//// a bit of offset to make many instances to not happen at the same time..
	//bool bRandomOffset = 0;
	//int tOffset = 0;
};
