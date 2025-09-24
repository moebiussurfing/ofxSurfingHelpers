#pragma once

#include "ofMain.h"

/*
	ofParameterGroup g{ "ofApp" };
*/

/*
	TODO:

*/

/*
* 
* NOTE
* 
	// For file settings serialize for an ofParameterGroup
	// Could avoid including this addon copying this!
	//	ofParameterGroup parameters;
	//	string path = "myFile.json";
	// Load 
	{
		ofFile f;
		bool b = f.doesFileExist(path);
		if (b)
			ofLogNotice("ofxSurfing") << "Found file: `" << path << "`";
		else
			ofLogError("ofxSurfing") << "File `" << path << "` not found!";
		if (b) {
			ofJson settings;
			settings = ofLoadJson(path);
			ofDeserialize(settings, parameters);
		}
	}
	
	// Save
	{
		if (!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
			if(ofFilePath::createEnclosingDirectory(path))
				ofLogWarning("ofxSurfingPBR") << "Created enclosing folder for: " << path;
		}
		ofJson settings;
		ofSerialize(settings, parameters);
		bool b = ofSavePrettyJson(path, settings);
		if (b)
			ofLogNotice("ofxSurfing") << "Saved: `" << parameters.getName() << "` to " << path;
		else
			ofLogError("ofxSurfing") << "Error saving: `" << parameters.getName() << "` to " << path;
	}
*/


/*
* 
* EXAMPLE
* 

#ifdef OF_APP_DEFINED_ofxSurfingHelpers
	ofxSurfingHelpers::setWindowTitleAsProjectName();
	ofxSurfingHelpers::setMonitorsLayout(0, false, true);
#endif

*/

//---------

#ifndef OF_APP_DEFINED_ofxSurfingHelpers
#define OF_APP_DEFINED_ofxSurfingHelpers 
// To avoid include it twice. 
// useful when using other helpers with the same namespace. 
// as we do in ofxSurfingImGui.

//--

// Include some classes by default. 
// Notice that there's other classes bundled on the add-on (/src or /libs) 
// that are not included here, then you have to include them manually.

#include "ofxSurfingConstants.h"

//--

#include "surfingTimers.h"
#include "ofxSurfing_Widgets.h"
//#include "ofxAutosaveGroupTimer.h"

#include "utils/surfingSmooth.h"

//--

//TODO: WIP
// test improve serialize performance..
// trying to load all the settings but not triggering each param.
// then trig all loaded params after file load.
//#define USE_FAST_SERIALIZER 

//--

namespace ofxSurfingHelpers
{
	// Monitors helpers

	// WARNING! For my personal setup!
	// I have three FHD monitors: 
	// Left monitor is portrait. 
	// Central and Right monitor are landscape.
	// Example:	
	//	ofxSurfingHelpers::setMonitorsLayout(-1, true, true); // Left monitor portrait
	//--------------------------------------------------------------
	inline void setMonitorsLayout(int pos, bool bPortrait, bool bStandardFrameRate)
	{
		// main monitor, and in fact all the monitors, 
		// are expected as FHD 1920x1080.
		// in my case my main monitor is landscape and central
		// and the left and right monitors are portrait.
		// you can modify for your own layout!

		if (bStandardFrameRate) {
			ofSetFrameRate(60);
			ofSetVerticalSync(false);
		}

		int x, y, w, h;

		// window bar height on WIN_32.
		//int yOffset = 38;
		//int hOffset = 12;
		//fix
		int yPos = 36;
		int hOffset = 36;

		x = y = 0;
		y = yPos;

		w = bPortrait ? 1080 : 1920;
		h = bPortrait ? 1920 : 1080;
		h -= hOffset;

		if (pos == -1) x = bPortrait ? -1080 : -1920;
		else if (pos == 1) x = 1920;

		ofSetWindowPosition(x, y);
		ofSetWindowShape(w, h);
	}

	// Legacy
	//--------------------------------------------------------------
	inline void setMonitorsLayout(int layout = 0, bool bStandardFrameRate = true)
	{
		if (bStandardFrameRate) {
			ofSetFrameRate(60);
			ofSetVerticalSync(false);
		}

		// window bar height on WIN_32.
		int h = 38;
		int offset = 12;
		int hw = h - offset;
		if (layout == 0)
		{
			// move app window to the left monitor (from main) as portrait
			ofSetWindowPosition(-1080, h);
			ofSetWindowShape(1080, 1920 - hw);
		}
		else if (layout == 1)
		{
			// move app window  to the right monitor (from main) as landscape 
			ofSetWindowPosition(1920, h);
			ofSetWindowShape(1920, 1080 - hw);
		}
		else if (layout == 2)
		{
			// move app window  to the left monitor (from main) as landscape
			ofSetWindowPosition(-1920, h);
			ofSetWindowShape(1920, 1080 - hw);
		}
	}

	//----

	// Window title

	// Set title with fps. Call on update()
	//--------------------------------------------------------------
	inline void setWindowTitleDebugPerformance(string name, bool bAsciiBar = false) {
		float v = ofMap(ofGetFrameRate(), 0, 60, 0, 1, true);

		string s = name + "    |     " + ofToString(ofGetFrameRate(), 0) + " fps";
		s += " / " + ofToString(ofGetLastFrameTime() * 1000, 0) + " ms";
		s += "           ";

		if (bAsciiBar) {
			string ss = "";
			size_t a = 10;
			for (size_t i = 0; i < 10; i++)
			{
				if (i == (size_t)(v * a) - 0) ss += "|";
				else ss += "-";
			}
			s += ss;
		}

		ofSetWindowTitle(s);
	}

	// Get the OF project file path name and set to the window title.
	//--------------------------------------------------------------
	inline string getProjectName() {
		of::filesystem::path path = ofToDataPath("", true);
		string s = (path.parent_path().parent_path().filename()).string();
		return s;
	}
	//--------------------------------------------------------------
	inline void setWindowTitleAsProjectName() {
		ofSetWindowTitle(getProjectName());
	}

	//----

	// Images drawing

	//--------------------------------------------------------------
	inline void drawImageFullScreenFit(ofFloatImage& imageFloat, ofScaleMode scaleMode = OF_SCALEMODE_FIT, bool bflip = false)
	{
		if (!imageFloat.isAllocated()) return;

		if (!bflip) {
			ofRectangle rr(0, 0, imageFloat.getWidth(), imageFloat.getHeight());
			rr.scaleTo(ofGetCurrentViewport(), scaleMode);
			imageFloat.draw(rr.x, rr.y, rr.width, rr.height);
		}
		else {
			ofRectangle rr(0, 0, imageFloat.getHeight(), imageFloat.getWidth());
			rr.scaleTo(ofGetCurrentViewport(), scaleMode);
			imageFloat.draw(rr.x, rr.y, rr.width, rr.height);
		}
	}
	//--------------------------------------------------------------
	inline void drawImageFullScreenFit(ofImage& image, ofScaleMode scaleMode = OF_SCALEMODE_FIT, bool bflip = false)
	{
		if (!image.isAllocated()) return;

		if (!bflip) {
			ofRectangle rr(0, 0, image.getWidth(), image.getHeight());
			rr.scaleTo(ofGetCurrentViewport(), scaleMode);
			image.draw(rr.x, rr.y, rr.width, rr.height);
		}
		//TODO:
		else {
			ofRectangle rr(0, 0, image.getHeight(), image.getWidth());
			rr.scaleTo(ofGetCurrentViewport(), scaleMode);
			ofPushMatrix();
			image.draw(rr.x, rr.y, rr.width, rr.height);
			ofPopMatrix();
		}
	}
	//--------------------------------------------------------------
	inline void drawImageAtBottom(ofImage& image, bool bLine = true)
	{
		if (!image.isAllocated()) return;

		ofPushStyle();

		ofRectangle r{ 0,0, image.getWidth(),image.getHeight() };
		r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
		r.translateY(ofGetHeight() - r.getBottomLeft().y);
		ofSetColor(255, 255);
		image.draw(r.x, r.y, r.getWidth(), r.getHeight());

		if (bLine) {
			ofSetColor(0, 200);
			ofSetLineWidth(2);
			ofDrawLine(r.getTopLeft(), r.getTopRight());
		}

		ofPopStyle();
	}
	//--------------------------------------------------------------
	inline void drawImageAtRight(ofImage& image, bool bLine = true)
	{
		if (!image.isAllocated()) return;

		ofPushStyle();

		ofRectangle r(0, 0, image.getWidth(), image.getHeight());
		r.scaleTo(ofGetCurrentViewport(), OF_SCALEMODE_FIT);
		r.translateX(ofGetWidth() - r.getBottomRight().x);
		image.draw(r.x, r.y, r.width, r.height);

		if (bLine) {
			ofSetColor(0, 200);
			ofSetLineWidth(2);
			ofDrawLine(r.getTopLeft(), r.getBottomLeft());
		}

		ofPopStyle();
	}
	//--------------------------------------------------------------
	inline void drawImageAtBottomRight(ofImage& image, string label = "")
	{
		if (!image.isAllocated()) return;

		ofPushStyle();

		float p = 6;
		float p2 = 5;

		float w = image.getWidth();
		float h = image.getHeight();
		float x = ofGetWidth() - w - p;
		float y = ofGetHeight() - h - p;

		//bg
		ofRectangle r(x, y, w, h);
		r.setWidth(r.width + p2);
		r.setHeight(r.height + p2);
		r.translateX(-p2 / 2.f);
		r.translateY(-p2 / 2.f);
		ofFill();
		ofSetColor(0, 225);
		ofDrawRectRounded(r, 3);

		//img
		ofSetColor(255, 255);
		image.draw(x, y);

		//txt
		if (label != "") {
			ofDrawBitmapStringHighlight(label, x + 4, y + 14);
		}

		ofPopStyle();
	}
	//--------------------------------------------------------------
	inline void drawImageResponsive(ofImage& image, bool bLine = true)
	{
		if (ofGetWidth() > ofGetHeight()) ofxSurfingHelpers::drawImageAtRight(image);//landscape
		else ofxSurfingHelpers::drawImageAtBottom(image);//portrait
	}

	//----

	// ofParams Serializers

	//----

	//--------------------------------------------------------------
	inline bool loadGroup(ofParameterGroup& g, string path = "", bool bSilent = false)
	{
		if (path == "")
		{
			path = g.getName() + "_Settings.json"; // a default filename
			ofLogWarning("ofxSurfingHelpers") << "loadGroup " << "Path is empty! Using a default instead!";
		}

		if (!bSilent)
		{
			ofLogNotice("ofxSurfingHelpers") << "loadGroup `" << g.getName() << "` from `" << path << "`";
			ofLogNotice("ofxSurfingHelpers") << "ofParameters: \n\n" << g.toString();
		}

		ofJson settings;
		settings = ofLoadJson(path);

#ifndef USE_FAST_SERIALIZER
		ofDeserialize(settings, g);
#endif
#ifdef USE_FAST_SERIALIZER
		ofDeserializeSilent(settings, g);
#endif

		// Returns false if no file preset yet.
		ofFile f;
		bool b = f.doesFileExist(path);
		if (b) ofLogNotice("ofxSurfingHelpers") << "loadGroup: `" << g.getName() << "` from `" << path << "`";
		else ofLogError("ofxSurfingHelpers") << "Error loading: `" << g.getName() << "` from `" << path << "` Not found!";

		return b; // Returns true if it's ok
	};

	//--------------------------------------------------------------
	inline bool saveGroup(ofParameterGroup& g, string path = "", bool bSilent = false)
	{
		if (path == "") {
			path = g.getName() + "_Settings.json";
			ofLogWarning("ofxSurfingHelpers") << "saveGroup " << "Path is empty! Using a default instead!";
		}

		if (!bSilent) {
			ofLogNotice("ofxSurfingHelpers") << g.getName() << " to `" << path << "`";
			ofLogNotice("ofxSurfingHelpers") << "ofParameters: \n\n" << g.toString();
		}

		// Create folder if folder do not exist!
		// From now, will not require to call manually:
		//ofxSurfingHelpers::CheckFolder(path);
		if (!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
			ofFilePath::createEnclosingDirectory(path);
			ofLogWarning("ofxSurfingHelpers") << "Created enclosing folder for: " << path;
		}

		ofJson settings;
		ofSerialize(settings, g);
		bool b = ofSavePrettyJson(path, settings);

		if (b) ofLogVerbose("ofxSurfingHelpers") << "Save: `" << g.getName() << "` to " << path;
		else ofLogError("ofxSurfingHelpers") << "Error saving: `" << g.getName() << "` to " << path;

		return b;
	}

	//----

	// Shorten versions:
	// will use a default path
	//--------------------------------------------------------------
	inline bool load(ofParameterGroup& g)
	{
		return loadGroup(g);
	}

	//--------------------------------------------------------------
	inline bool save(ofParameterGroup& g)
	{
		return saveGroup(g);
	}

	//--

	// Files

	//--------------------------------------------------------------
	// Check if a folder path exist and creates one if not
	// why? many times when you try to save a file, 
	// this is not possible and do not happens 
	// bc the container folder do not exist
	//--------------------------------------------------------------
	inline void CheckFolder(string _path)
	{
		//ofLogNotice("ofxSurfingHelpers")<<(__FUNCTION__) << _path;

		//TODO: A clean alternative..
		/*
		///string jsonfilepath;
		if (!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(jsonfilepath))) {
			ofFilePath::createEnclosingDirectory(jsonfilepath);
		}
		*/

		//if (ofFile::doesFileExist(_path)) {

		//// Workaround to avoid error when folders are folder/subfolder
		//auto _fullPath = ofSplitString(_path, "/");
		//for (int i = 0; i < _fullPath.size(); i++) {
		//	ofLogNotice("ofxSurfingHelpers")<<(__FUNCTION__) << ofToString(i) << " " << _fullPath[i];
		//}

		// /bin/data/
		ofDirectory dataDirectory(ofToDataPath(_path, true));

		// Check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError("ofxSurfingHelpers") << (__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			// Try to create folder
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true), false, true);
			//bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));
			// Added enable recursive to allow create nested subfolders if required

			// Debug if creation has been succeded
			if (b) ofLogNotice("ofxSurfingHelpers") << "CREATED '" << _path << "' SUCCESSFULLY!";
			else ofLogError("ofxSurfingHelpers") << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogVerbose("ofxSurfingHelpers") << _path << " Found!";// nothing to do
		}
	}

	//--

	//TODO:
	// Testing for improving performance
	// changing the mode we trig the loaded params...
	// load file without trigging. trig after all is loaded.

	//--------------------------------------------------------------
	inline void ofDeserializeSilent(const ofJson& json, ofAbstractParameter& parameter) {
		ofLogNotice("ofxSurfingHelpers") << (__FUNCTION__) << parameter.getName();

		if (!parameter.isSerializable()) {
			return;
		}
		std::string name = parameter.getEscapedName();
		if (json.find(name) != json.end()) {
			if (parameter.type() == typeid(ofParameterGroup).name()) {
				ofParameterGroup& group = static_cast <ofParameterGroup&>(parameter);
				for (auto& p : group) {
					ofDeserializeSilent(json[name], *p);
				}
			}
			else {
				if (parameter.type() == typeid(ofParameter <int>).name() && json[name].is_number_integer()) {
					parameter.cast <int>().setWithoutEventNotifications(json[name].get<int>());
					//parameter.cast <int>() = json[name].get<int>();
				}
				else if (parameter.type() == typeid(ofParameter <float>).name() && json[name].is_number_float()) {
					parameter.cast <float>().setWithoutEventNotifications(json[name].get<float>());
					//parameter.cast <float>() = json[name].get<float>();
				}
				else if (parameter.type() == typeid(ofParameter <bool>).name() && json[name].is_boolean()) {
					parameter.cast <bool>().setWithoutEventNotifications(json[name].get<bool>());
					//parameter.cast <bool>() = json[name].get<bool>();
				}
				else if (parameter.type() == typeid(ofParameter <int64_t>).name() && json[name].is_number_integer()) {
					parameter.cast <int64_t>().setWithoutEventNotifications(json[name].get<int64_t>());
					//parameter.cast <int64_t>() = json[name].get<int64_t>();
				}
				else if (parameter.type() == typeid(ofParameter <std::string>).name()) {
					parameter.cast <std::string>().setWithoutEventNotifications(json[name].get<std::string>());
					//parameter.cast <std::string>() = json[name].get<std::string>();
				}
				else {
					parameter.fromString(json[name]);
				}
			}
		}
	}


	//----


	//TODO: add to API
	/*
	// Return file extension

	inline string getFileName(string _str, bool _trimExt = false) {
		string name = "";
		int slashPos = _str.rfind('/');

		if (slashPos != string::npos) {
			name = _str.substr(slashPos + 1);

			if (_trimExt)
				name = getFilePathTrimExtension(name);
		}

		return name;
	}

	inline string getFileFolderPath(string _str) {
		string name = "";
		int slashPos = _str.rfind('/');

		if (slashPos != string::npos) {
			name = _str.substr(0, slashPos);
		}

		return name;
	}

	inline string getFileExtension(string _str) {
		string ext = "";
		int extPos = _str.rfind('.');

		if (extPos != string::npos) {
			ext = _str.substr(extPos + 1);
		}

		return ext;
	}

	inline string getFilePathTrimExtension(string _str) {
		string ext = "";
		int extPos = _str.rfind('.');

		if (extPos != string::npos) {
			ext = _str.substr(0, extPos);
		}

		return ext;
	}

	inline int getNumWords(std::string str)
	{
		int word_count(0);
		std::stringstream ss(str);
		std::string word;
		while (ss >> word) ++word_count;
		return word_count;
	}
	*/


	//----


	// Time in seconds to string min::sec
	// Original code taken from ofxFilikaUtils.h

#define SECS_PER_MIN 60
#define SECS_PER_HOUR 3600

	//--------------------------------------------------------------
	inline std::string calculateTime(float _time) {

		int seconds;
		int minutes;
		int mins_left;
		int secs_left;

		seconds = (/*gameTimeSeconds - */int(_time));
		minutes = (/*gameTimeSeconds - */int(_time)) / SECS_PER_MIN;
		mins_left = minutes % SECS_PER_MIN;
		secs_left = seconds % SECS_PER_MIN;

		std::string mins;
		std::string secs;

		if (mins_left < 10) {
			mins = "0" + ofToString(mins_left);
		}
		else {
			mins = ofToString(mins_left);
		}

		if (secs_left < 10) {
			secs = "0" + ofToString(secs_left);
		}
		else {
			secs = ofToString(secs_left);
		}

		//cout << ofGetElapsedTimeMillis() / 1000 << endl;
		//cout << "remaining time : " << mins_left << " : " <<  secs_left << endl;
		//cout << "remaining time : " << mins << " : " <<  secs << endl;

		if (mins_left < 0 || secs_left < 0)
			return "00:00";
		else
			return (mins + ":" + secs);
	}
	inline std::string getTimeFromSecond(float _time) { return calculateTime(_time); }

	//----


	// Debug Helpers

	//--------------------------------------------------------------
	inline void debugCoutParam(ofAbstractParameter& ap)
	{
		cout << ap.getName() << ": " << ap << endl;
	}

}; // namespace ofxSurfingHelpers 

#endif
