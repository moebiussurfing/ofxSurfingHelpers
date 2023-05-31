#include "ofxAutosaveGroupTimer.h"

//--------------------------------------------------------------
ofxAutosaveGroupTimer::ofxAutosaveGroupTimer()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "ofxAutosaveGroupTimer() CONSTRUCTOR";

	ofAddListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);

	// Fix some exit exceptions.
	int minValue = std::numeric_limits<int>::min();
	ofAddListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit, minValue);

	setup();
}

//--------------------------------------------------------------
ofxAutosaveGroupTimer::~ofxAutosaveGroupTimer()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "~ofxAutosaveGroupTimer() DESTRUCTOR";

	ofRemoveListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);
	ofRemoveListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit);

	//exit();
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::startup()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "startup()";

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::loadGroup(params, path_Global + "/" + name_Settings + fileExtension);

	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::loadGroup(data[i].params, data[i].path, !bSilent.get());
	}

	//tOffset = (int)ofRandom(5);
	tOffset = (int)MAX(0, ofRandom(timeToAutosave.get() / 5));
	//tOffset = (int) MAX(0, ofRandom(timeToAutosave.get() / 100));
	//cout << "tOffset:" << tOffset << endl;

	bDoneStartup = true;
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::setup()
{
	data.clear();

	bAutoSave.set("auto save", true);
	bSilent.set("silent", true);
	timeToAutosave.set("time", 60, 1, 240);

	params.setName("ofxAutosaveGroupTimer");
	params.add(bAutoSave);
	params.add(timeToAutosave);
	params.add(bSilent);

	timerLast_Autosave = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::exit(ofEventArgs& args)
{
	ofLogNotice("ofxAutosaveGroupTimer") << "exit(ofEventArgs& args)";

	exit();
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::exit()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "exit()";

	saveAllGroups();

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::saveGroup(params, path_Global + "/" + name_Settings + fileExtension, !bSilent.get());
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
	if (!bSilent) ofLogNotice("ofxAutosaveGroupTimer") << "saveAllGroups()";

	auto l = ofGetLogLevel();
	if (bSilent) ofSetLogLevel(OF_LOG_SILENT);

	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::saveGroup(data[i].params, data[i].path, !bSilent.get());
	}

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::saveGroup(params, path_Global + "/" + name_Settings + fileExtension, !bSilent.get());

	if (bSilent) ofSetLogLevel(l);
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
		if (!bRandomOffset) t = ofGetElapsedTimef() - timerLast_Autosave;
		else t = ofGetElapsedTimef() + tOffset - timerLast_Autosave;

		//cout << "t:" << t << endl;

		progressPrc = ofMap(t, 0, timeToAutosave.get(), 0, 1, true);

		//static int count = 0;

		if (t >= timeToAutosave.get())
		{
			if (!bRandomOffset) timerLast_Autosave = ofGetElapsedTimef();
			else timerLast_Autosave = ofGetElapsedTimef() - tOffset;

			saveAllGroups();

			stringstream ss;
			if (bRandomOffset)
			{
				ss << " | " << ofToString(tOffset) << " | ";
				ss << ofToString(timeToAutosave - tOffset) << "";
			}
			else {
				//ss << "]";
			}
			ss << "  #" << (++count) << " | " << (ofxSurfingHelpers::calculateTime(ofGetElapsedTimef())) << " | " << (timeToAutosave.get()) << "secs";

			ofLogNotice("ofxAutosaveGroupTimer") << "> DONE! Autosave to " << path_Global.c_str() << ss.str();

			for (int i = 0; i < data.size(); i++)
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "> Group #" << i << ": " << data[i].params.getName();
			}

			if (!bSilent.get())
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "--------------------------------------------------------------\n";
			}
		}
	}
}