#include "ofxAutosaveGroupTimer.h"

//--------------------------------------------------------------
ofxAutosaveGroupTimer::ofxAutosaveGroupTimer()
{
	ofAddListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);

	setup();
}

//--------------------------------------------------------------
ofxAutosaveGroupTimer::~ofxAutosaveGroupTimer()
{
	ofRemoveListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);

	exit();
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::startup()
{
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::loadGroup(params, path_Global + name_Settings + fileExtension);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::setup()
{
	data.clear();

	bAutoSave.set("auto save", true);
	bSilent.set("silent", false);
	timeToAutosave.set("time", 5000, 100, 10000);

	params.setName("ofxAutosaveGroupTimer");
	params.add(bAutoSave);
	params.add(timeToAutosave);
	params.add(bSilent);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::exit()
{
	saveAllGroups();

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::saveGroup(params, path_Global + name_Settings + fileExtension, !bSilent.get());
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::addGroup(ofParameterGroup params, string path)
{
	ofxSurfingHelpers::SurfDataGroupSaver d;
	d.params = params;
	d.path = path;
	this->addGroup(d);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::addGroup(ofxSurfingHelpers::SurfDataGroupSaver _data)
{
	data.push_back(_data);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::saveAllGroups()
{
	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::saveGroup(data[i].params, data[i].path, !bSilent.get());
	}

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::saveGroup(params, path_Global + name_Settings + fileExtension, !bSilent.get());
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::update(ofEventArgs& args)
{
	if (!bDoneStartup)
	{
		bDoneStartup = true;
		startup();
	}

	//--

	if (bAutoSave)
	{
		auto t = ofGetElapsedTimeMillis() - timerLast_Autosave;
		progressPrc = ofMap(t, 0, timeToAutosave.get(), 0, 1, true);

		static int count = 0;

		if (t >= timeToAutosave.get())
		{
			timerLast_Autosave = ofGetElapsedTimeMillis();

			saveAllGroups();

			ofLogNotice("ofxAutosaveGroupTimer") << "Auto save DONE!";
			if (!bSilent.get()) {
				ofLogNotice("ofxAutosaveGroupTimer") << "#" << count++ << " " << ofGetElapsedTimef();
				ofLogNotice("ofxAutosaveGroupTimer") << "--------------------------------------------------------------\n";
			}
		}
	}
}