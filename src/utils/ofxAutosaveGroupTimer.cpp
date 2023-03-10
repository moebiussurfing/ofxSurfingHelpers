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

	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::loadGroup(data[i].params, data[i].path, !bSilent.get());
	}

	tOffset = (int) MAX(0, ofRandom(timeToAutosave.get() / 100));
	//cout << "tOffset:" << tOffset << endl;

	bDoneStartup = true;
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::setup()
{
	data.clear();

	bAutoSave.set("auto save", true);
	bSilent.set("silent", true);
	timeToAutosave.set("time", 10000, 100, 30000);

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
void ofxAutosaveGroupTimer::addGroup(ofParameterGroup params)
{
	string path = params.getName() + "_Settings.json";
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
		startup();
	}

	//--

	if (bAutoSave)
	{
		// elapsed
		uint64_t t;
		if (!bRandomOffset) t = ofGetElapsedTimeMillis() - timerLast_Autosave;
		else t = ofGetElapsedTimeMillis() - tOffset - timerLast_Autosave;
		
		progressPrc = ofMap(t, 0, timeToAutosave.get(), 0, 1, true);

		//static int count = 0;

		if (t >= timeToAutosave.get())
		{
			if (!bRandomOffset) timerLast_Autosave = ofGetElapsedTimeMillis();
			else timerLast_Autosave = ofGetElapsedTimeMillis() - tOffset;

			saveAllGroups();

			ofLogNotice("ofxAutosaveGroupTimer") << "Auto save DONE!";
			ofLogNotice("ofxAutosaveGroupTimer") << " " << path_Global;

			for (int i = 0; i < data.size(); i++)
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "  " << data[i].params.getName();
			}

			ofLogNotice("ofxAutosaveGroupTimer") << "#" << count++ << " | " << (int)ofGetElapsedTimef() << " secs";
			if (!bSilent.get())
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "--------------------------------------------------------------\n";
			}
		}
	}
}