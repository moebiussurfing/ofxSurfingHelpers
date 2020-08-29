#pragma once

#include "ofMain.h"
#include "ofxGui.h"

namespace ofxSurfingHelpers {
	//using namespace ofxSurfingHelpers;

	//-

	//circular progress bar
	
	//float val = ofMap(ofGetFrameNum() % 200, 0, 200, 0.f, 1.f, true);
	//ofxSurfingHelpers::drawCircleProg(val);

	inline void drawCircleProg(float val) {
		ofPushStyle();
		ofSetLineWidth(5);

		float radius = 50;
		float ratio = 0.7;

		//float val = 0.5;
		//float progress = val / 100;

		ofPoint point1(150, 120);

		ofNoFill();


		ofDrawRectangle(point1.x, point1.y, 100, 100);
		//int startAngle = -90 * 16;
		//int spanAngle = val * 360 * 16;		
		int startAngle = -90;
		int spanAngle = -90 + val * 360;
		//int spanAngle = progress * 360 * 16;

		//p.drawArc(rectangle, startAngle, spanAngle);
		//void ofPolyline_::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution=20)
		//polyline1.arc(point1, 100, 100, 0, 360);

		ofPolyline polyline1;
		polyline1.lineTo(point1.x, point1.y + radius * ratio);
		polyline1.lineTo(point1.x, point1.y + radius);
		polyline1.arc(point1, radius, radius, startAngle, spanAngle);
		polyline1.arc(point1, radius*ratio, radius*ratio, startAngle, spanAngle);
		//polyline1.lineTo(0, point1.y + radius * ratio);
		//polyline1.lineTo(0, point1.y + radius);
		ofSetColor(ofColor::blue);
		polyline1.draw();

		string str = "prog" + ofToString(val * 100);
		//string str = "prog" + ofToString(progress * 100);
		ofDrawBitmapStringHighlight(str, point1.x, point1.y + 100);

		ofPopStyle();
	}

	//-

	//--------------------------------------------------------------
	inline bool loadGroup(ofParameterGroup &g, string path)
	{
		ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		ofXml settings;
		bool b = settings.load(path);
		ofDeserialize(settings, g);
		return b;
	}

	//--------------------------------------------------------------
	inline bool saveGroup(ofParameterGroup &g, string path)
	{
		ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		ofXml settings;
		ofSerialize(settings, g);
		bool b = settings.save(path);
		return b;
	}

	////--------------------------------------------------------------
	//inline void loadGroup(ofParameterGroup &g, string path)
	//{
	//	ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
	//	ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
	//	ofXml settings;
	//	settings.load(path);
	//	ofDeserialize(settings, g);
	//}

	////--------------------------------------------------------------
	//inline void saveGroup(ofParameterGroup &g, string path)
	//{
	//	ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
	//	ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
	//	ofXml settings;
	//	ofSerialize(settings, g);
	//	settings.save(path);
	//}

	//ofxGui theme
	inline void setTheme_ofxGui()
	{
		string pathFont = "assets/fonts/overpass-mono-bold.otf";
		ofFile file(pathFont);
		if (file.exists())
		{
			ofxGuiSetFont(pathFont, 9);
		}
		ofxGuiSetDefaultHeight(20);
		ofxGuiSetBorderColor(32);
		ofxGuiSetFillColor(ofColor(24));
		ofxGuiSetTextColor(ofColor::white);
		ofxGuiSetHeaderColor(ofColor(12));
		ofxGuiSetBackgroundColor(ofColor::black);
	}

	//check if a folder path exist and creates one if not
	//many times when you try to save a file, this is not possible and do not happens bc the container folder do not exist
	//--------------------------------------------------------------
	inline void CheckFolder(string _path)
	{
		ofLogNotice(__FUNCTION__) << _path;

		////workaround to avoid error when folders are folder/subfolder
		//auto _fullPath = ofSplitString(_path, "/");
		//for (int i = 0; i < _fullPath.size(); i++) {
		//	ofLogNotice(__FUNCTION__) << ofToString(i) << " " << _fullPath[i];
		//}

		ofDirectory dataDirectory(ofToDataPath(_path, true));// /bin/data/

		//check if folder path exist
		if (!dataDirectory.isDirectory())
		{
			ofLogError(__FUNCTION__) << "FOLDER NOT FOUND! TRYING TO CREATE...";

			//try to create folder
			//bool b = dataDirectory.createDirectory(ofToDataPath(_path, true));
			bool b = dataDirectory.createDirectory(ofToDataPath(_path, true), false, true);
			//added enable recursive to allow create nested subfolders if required

			//debug if creation has been succeded
			if (b) ofLogNotice(__FUNCTION__) << "CREATED '" << _path << "'  SUCCESSFULLY!";
			else ofLogError(__FUNCTION__) << "UNABLE TO CREATE '" << _path << "' FOLDER!";
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "OK! LOCATED FOLDER: '" << _path << "'";//nothing to do
		}
	}


	///* RETURN FILE EXTENSION */
	//inline string getFileName(string _str, bool _trimExt = false) {
	//	string name = "";
	//	int slashPos = _str.rfind('/');

	//	if (slashPos != string::npos) {
	//		name = _str.substr(slashPos + 1);

	//		if (_trimExt)
	//			name = getFilePathTrimExtension(name);
	//	}

	//	return name;
	//}

	//inline string getFileFolderPath(string _str) {
	//	string name = "";
	//	int slashPos = _str.rfind('/');

	//	if (slashPos != string::npos) {
	//		name = _str.substr(0, slashPos);
	//	}

	//	return name;
	//}

	//inline string getFileExtension(string _str) {
	//	string ext = "";
	//	int extPos = _str.rfind('.');

	//	if (extPos != string::npos) {
	//		ext = _str.substr(extPos + 1);
	//	}

	//	return ext;
	//}

	//inline string getFilePathTrimExtension(string _str) {
	//	string ext = "";
	//	int extPos = _str.rfind('.');

	//	if (extPos != string::npos) {
	//		ext = _str.substr(0, extPos);
	//	}

	//	return ext;
	//}

	//inline int getNumWords(std::string str)
	//{
	//	int word_count(0);
	//	std::stringstream ss(str);
	//	std::string word;
	//	while (ss >> word) ++word_count;
	//	return word_count;
	//}

};//ofxSurfingHelpers