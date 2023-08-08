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

	ofxSurfingHelpers::loadGroup(params, path_Global + "/" + name_Settings + fileExtension);

	// load
	for (int i = 0; i < data.size(); i++)
	{
		ofxSurfingHelpers::loadGroup(data[i].params, data[i].path, !bSilent.get());
	}

	// store all queued in one group
	paramsQueue.clear();
	for (int i = 0; i < data.size(); i++)
	{
		paramsQueue.add(data[i].params);
	}
	eParamsQueue = paramsQueue.parameterChangedE().newListener([&](ofAbstractParameter&) {
		bFlagSaveQueued = 1;
		timerLast_AutoSaveQueuedOnChange = ofGetElapsedTimeMillis();
		});

	//tOffset = (int)MAX(0, ofRandom(timePeriodToAutosave.get() / 5));
	////tOffset = (int)ofRandom(5);
	////tOffset = (int) MAX(0, ofRandom(timePeriodToAutosave.get() / 100));

	//cout << "tOffset:" << tOffset << endl;

	bDoneStartup = true;
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::setup()
{
	data.clear();

	bAutoSaveOnChange.set("Auto Save OnChange", true);
	bAutoSaveTimer.set("Auto Save Timer", false);
	timePeriodToAutosave.set("Time Period ms", 60000, 2000, 20000);
	bSilent.set("Silent", true);

	params.setName("ofxAutosaveGroupTimer");
	params.add(bSilent);
	params.add(bAutoSaveOnChange);
	params.add(bAutoSaveTimer);
	params.add(timePeriodToAutosave);

	ofAddListener(params.parameterChangedE(), this, &ofxAutosaveGroupTimer::Changed_Params);

	timerLast_AutosaveTimer = ofGetElapsedTimeMillis();
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

			timerLast_AutosaveTimer = ofGetElapsedTimeMillis();

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
void ofxAutosaveGroupTimer::addGroup(ofParameterGroup &params, string path)
{
	ofxSurfingHelpers::SurfDataGroupSaver d;
	d.params = params;
	d.path = path;
	this->addGroup(d);
}

//--------------------------------------------------------------
void ofxAutosaveGroupTimer::addGroup(ofParameterGroup &params)
{
	string path = params.getName();
	path += "_Settings";
	path += fileExtension;
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

	stringstream ss;
	ss << "Saved Queue >  #" << (++count) << " | ";
	ss << (ofxSurfingHelpers::calculateTime(ofGetElapsedTimef()));
	ss << " to " << path_Global;
	ofLogNotice("ofxAutosaveGroupTimer") << "> DONE " << ss.str() ;
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
		// make a time gap to reduce the amount of file savings!
		uint64_t t = ofGetElapsedTimeMillis();

		if (bFlagSaveQueued && (t - timerLast_AutoSaveQueuedOnChange >= timeGap)) {
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
		t = ofGetElapsedTimeMillis() - timerLast_AutosaveTimer;

		//if (!bRandomOffset) t = ofGetElapsedTimeMillis() - timerLast_AutosaveTimer;
		//else t = ofGetElapsedTimeMillis() + tOffset - timerLast_AutosaveTimer;

		progressPrc = ofMap(t, 0, timePeriodToAutosave.get(), 0, 1, true);

		if (t >= timePeriodToAutosave.get())
		{
			timerLast_AutosaveTimer = ofGetElapsedTimeMillis();

			//if (!bRandomOffset) timerLast_AutosaveTimer = ofGetElapsedTimeMillis();
			//else timerLast_AutosaveTimer = ofGetElapsedTimeMillis() - tOffset;

			saveParamsQueued();
			saveParams();

			stringstream ss;

			//if (bRandomOffset)
			//{
			//	ss << " | " << ofToString(tOffset) << " | ";
			//	ss << ofToString(timePeriodToAutosave - tOffset) << "";
			//}

			ss << "  #" << (++count) << " | ";
			ss << (ofxSurfingHelpers::calculateTime(ofGetElapsedTimef()));
			ss << " | Period " << ofToString(timePeriodToAutosave.get() / 1000.f, 1) << " secs";

			ofLogNotice("ofxAutosaveGroupTimer") << "> DONE! Autosave to " << path_Global.c_str() << ss.str();

			for (int i = 0; i < data.size(); i++)
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "> Group #" << i << ": "
					<< data[i].params.getName()
					<< "  " << data[i].path;
			}

			if (!bSilent.get())
			{
				ofLogNotice("ofxAutosaveGroupTimer") << "--------------------------------------------------------------\n";
			}
		}
	}
}