#include "ofxAutosaveGroupTimer.h"

//--------------------------------------------------------------
ofxAutosaveGroupTimer::ofxAutosaveGroupTimer()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "ofxAutosaveGroupTimer() CONSTRUCTOR";

	ofAddListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);

	//// Fix some exit exceptions.
	//int minValue = std::numeric_limits<int>::min();//before all
	//ofAddListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit, minValue);

	setup();
}

//--------------------------------------------------------------
ofxAutosaveGroupTimer::~ofxAutosaveGroupTimer()
{
	ofLogNotice("ofxAutosaveGroupTimer") << "~ofxAutosaveGroupTimer() DESTRUCTOR";

	ofRemoveListener(ofEvents().update, this, &ofxAutosaveGroupTimer::update);

	ofRemoveListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit);

	ofRemoveListener(params.parameterChangedE(), this, &ofxAutosaveGroupTimer::Changed_Params);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::startup()
{
	if (bDoneStartup) return;

	ofLogNotice("ofxAutosaveGroupTimer") << "startup()";

	//ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::loadGroup(params, path_Global + "/" + name_Settings + fileExtension);

	// load
	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::loadGroup(data[i].params, data[i].path, !bSilent.get());
	}

	// store all in one group
	paramsQueue.clear();
	for (int i = 0; i < data.size(); i++)
	{
		paramsQueue.add(data[i].params);
	}
	eParamsQueue = paramsQueue.parameterChangedE().newListener([&](ofAbstractParameter&) {
		bFlagSaveQueued = 1;
		});

	tOffset = (int)MAX(0, ofRandom(timeToAutosave.get() / 5));
	//tOffset = (int)ofRandom(5);
	//tOffset = (int) MAX(0, ofRandom(timeToAutosave.get() / 100));

	//cout << "tOffset:" << tOffset << endl;

	bDoneStartup = true;
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::setup()
{
	data.clear();

	bAutoSaveOnChange.set("Auto Save", true);
	bAutoSaveTimer.set("Auto Save Timer", false);
	timeToAutosave.set("Time Period", 60, 1, 240);
	bSilent.set("Silent", true);

	params.setName("ofxAutosaveGroupTimer");
	params.add(bAutoSaveOnChange);
	params.add(bAutoSaveTimer);
	params.add(timeToAutosave);
	params.add(bSilent);

	ofAddListener(params.parameterChangedE(), this, &ofxAutosaveGroupTimer::Changed_Params);

	timerLast_Autosave = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::Changed_Params(ofAbstractParameter& e)
{
	string name = e.getName();

	ofLogNotice("ofxAutosaveGroupTimer") << "Changed_Params: " << name << ": " << e;

	bFlagSaveParams = 1;

	if (name == bAutoSaveOnChange.getName()) {
		if (bAutoSaveOnChange) {
			if (bAutoSaveTimer) bAutoSaveTimer = false;

			ofRemoveListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit);
		}
		return;
	}

	else if (name == bAutoSaveTimer.getName()) {
		if (bAutoSaveTimer) {
			if (bAutoSaveOnChange) bAutoSaveOnChange = false;

			timerLast_Autosave = ofGetElapsedTimef();

			//TODO
			ofRemoveListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit);
			int minValue = std::numeric_limits<int>::min();//before all
			ofAddListener(ofEvents().exit, this, &ofxAutosaveGroupTimer::exit, minValue);
		}
		return;
	}
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

	saveParams();
	saveParamsQueued();
}

//--

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
	string path = params.getName();
	//path += "_Settings.json";
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

//--

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::saveParams()
{
	if (!bSilent) ofLogNotice("ofxAutosaveGroupTimer") << "saveParams()";

	auto l = ofGetLogLevel();
	if (bSilent) ofSetLogLevel(OF_LOG_SILENT);

	// settings
	ofxSurfingHelpers::saveGroup(params, path_Global + "/" + name_Settings + fileExtension, !bSilent.get());

	if (bSilent) ofSetLogLevel(l);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::saveParamsQueued()
{
	if (!bSilent) ofLogNotice("ofxAutosaveGroupTimer") << "saveParamsQueued()";

	auto l = ofGetLogLevel();
	if (bSilent) ofSetLogLevel(OF_LOG_SILENT);

	// added groups
	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::saveGroup(data[i].params, data[i].path, !bSilent.get());
	}

	if (bSilent) ofSetLogLevel(l);
}

//--

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::update(ofEventArgs& args)
{
	if (!bDoneStartup)
	{
		startup();
	}

	//--

	if (bAutoSaveOnChange)
	{
		if (bFlagSaveQueued) {
			bFlagSaveQueued = 0;

			saveParamsQueued();
		}

		if (bFlagSaveParams) {
			bFlagSaveParams = 0;

			saveParams();
		}
	}

	//--

	if (bAutoSaveTimer)
	{
		// Elapsed
		uint64_t t;
		if (!bRandomOffset) t = ofGetElapsedTimef() - timerLast_Autosave;
		else t = ofGetElapsedTimef() + tOffset - timerLast_Autosave;

		progressPrc = ofMap(t, 0, timeToAutosave.get(), 0, 1, true);

		if (t >= timeToAutosave.get())
		{
			if (!bRandomOffset) timerLast_Autosave = ofGetElapsedTimef();
			else timerLast_Autosave = ofGetElapsedTimef() - tOffset;

			saveParamsQueued();
			saveParams();

			stringstream ss;
			if (bRandomOffset)
			{
				ss << " | " << ofToString(tOffset) << " | ";
				ss << ofToString(timeToAutosave - tOffset) << "";
			}
			ss << "  #" << (++count) << " | ";
			ss << (ofxSurfingHelpers::calculateTime(ofGetElapsedTimef()));
			ss << " | " << (timeToAutosave.get()) << "secs";

			ofLogNotice("ofxAutosaveGroupTimer") << "> DONE! Auto save to " << path_Global.c_str() << ss.str();

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