#pragma once

#include "ofMain.h"
#include "ofxGui.h"

namespace ofxSurfingHelpers {

	//--------------------------------------------------------------
	inline void loadGroup(ofParameterGroup &g, string path)
	{
		ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		ofXml settings;
		settings.load(path);
		ofDeserialize(settings, g);
	}

	//--------------------------------------------------------------
	inline void saveGroup(ofParameterGroup &g, string path)
	{
		ofLogNotice(__FUNCTION__) << g.getName() << " to " << path;
		ofLogVerbose(__FUNCTION__) << "parameters: \n" << g.toString();
		ofXml settings;
		ofSerialize(settings, g);
		settings.save(path);
	}

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
}//ofxSurfingHelpers