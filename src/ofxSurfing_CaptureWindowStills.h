#pragma once

#include "ofMain.h"

///----
///
#define INCLUDE_RECORDER
///
///----

#include "ofxTextureRecorder.h"
#include "ofxSurfingHelpers.h"

class CaptureWindow : public ofBaseApp
{

public:
	CaptureWindow() {
		cap_w = 1920;
		cap_h = 1080;
	};
	~CaptureWindow() {};

private:
	ofxTextureRecorder recorder;

	ofFbo cap_Fbo;
	ofFbo::Settings cap_Fbo_Settings;
	int cap_w, cap_h;

	string _pathFolderStills;
	string _pathFolderSnapshots;

	bool bRecPrepared = false;
	bool bRecording;
	bool bShowInfo = true;

public:
	//--------------------------------------------------------------
	void setVisibleInfo(bool b) {
		bShowInfo = b;
	}
	//--------------------------------------------------------------
	void setToggleVisibleInfo() {
		bShowInfo = !bShowInfo;
	}

private:
	//--------------------------------------------------------------
	float getRecordedDuration() {
		return (ofGetElapsedTimeMillis() - timeStart) / 1000.f;
	}

private:
	uint32_t timeStart;
	std::string textInfo;

	ofImageFormat stillFormat;
	//enum stillsImageFormat {
	//	OF_IMAGE_FORMAT_BMP = 0,
	//	OF_IMAGE_FORMAT_JPEG = 2,
	//	OF_IMAGE_FORMAT_PNG = 13,
	//	OF_IMAGE_FORMAT_TIFF = 18,
	//	OF_IMAGE_FORMAT_RAW = 34
	//};

public:
	//--------------------------------------------------------------
	void setup(std::string path = "captures/", ofImageFormat format = OF_IMAGE_FORMAT_PNG) {///call with the path folder if you want to customize
		ofLogWarning(__FUNCTION__) << "path: " << path << " ofImageFormat: " << format;

		_pathFolderStills = path + "Stills/";
		_pathFolderSnapshots = path + "Snapshots/";
		ofxSurfingHelpers::CheckFolder(_pathFolderStills);
		ofxSurfingHelpers::CheckFolder(_pathFolderSnapshots);

		buildInfo();

		cap_w = ofGetWidth();
		cap_h = ofGetHeight();
		cap_Fbo.allocate(cap_w, cap_h, GL_RGB);

		stillFormat = format;

		buildRecorder();

		buildAllocateFbo();
	}

public:
	//--------------------------------------------------------------
	void buildAllocateFbo() {//cap_w and cap_h must be updated
		ofLogWarning(__FUNCTION__) << cap_w << ", " << cap_h;

		cap_Fbo_Settings.internalformat = GL_RGB;
		cap_Fbo_Settings.width = cap_w;
		cap_Fbo_Settings.height = cap_h;
		cap_Fbo.allocate(cap_Fbo_Settings);
	}
	//--------------------------------------------------------------
	void buildRecorder() {
		ofLogWarning(__FUNCTION__);

		ofxTextureRecorder::Settings settings(cap_Fbo.getTexture());
		settings.imageFormat = stillFormat;

		//settings.imageFormat = OF_IMAGE_FORMAT_TIFF;
		//settings.imageFormat = OF_IMAGE_FORMAT_PNG;
		//settings.imageFormat = OF_IMAGE_FORMAT_JPEG;
		//settings.imageFormat = OF_IMAGE_FORMAT_RAW;

		settings.numThreads = 12;
		settings.maxMemoryUsage = 9000000000;

		recorder.setPath(_pathFolderStills);
		recorder.setup(settings);
	}
	//--------------------------------------------------------------
	void updateRecorder() {// TODO: window resize ??
		//recorder = ofxTextureRecorder();
		//recorder.settings
		//ofxTextureRecorder::Settings
		//recorder.
	}

public:
	//--------------------------------------------------------------
	void begin() {///call before draw the scene to record

		cap_Fbo.begin();
		ofClear(0, 255);
	}

	//--------------------------------------------------------------
	void end() {///call after draw the scene to record

		cap_Fbo.end();

		//-

		if (bRecPrepared)
		{
			if (bRecording && ofGetFrameNum() > 0)
			{
				recorder.save(cap_Fbo.getTexture());
			}
		}
	}

	//--------------------------------------------------------------
	void draw() {
		cap_Fbo.draw(0, 0);//drawing is required outside fbo
	}

	//--------------------------------------------------------------
	void drawInfo() {///draw the gui info if desired
		if (bShowInfo) {

			int x = 40;
			int y = ofGetHeight() - 340;

			//--

			if (bRecPrepared || bRecording)
			{
				//cap info
				string str = "";

				str += "FPS " + ofToString(ofGetFrameRate(), 0); str += +"\n";
				str += "WINDOW   " + ofToString(ofGetWidth()) + "x" + ofToString(ofGetHeight()); str += +"\n";
				str += "FBO SIZE " + ofToString(cap_w) + "x" + ofToString(cap_h); str += +"\n";
				str += "RECORDER " + ofToString(recorder.getWidth()) + "x" + ofToString(recorder.getHeight()) + " frame " + ofToString(recorder.getFrame());
				str += +"\n"; str += +"\n";

				//draw red circle and info when recording
				ofPushStyle();

				//refresh window size
				str += "KEY F7: REFRESH WINDOW SIZE  "; str += +"\n";
				str += "KEY F8: SET FULL HD SIZE"; str += +"\n";
				if (bRecording)
				{
					str += "KEY U : STOP"; str += +"\n"; str += +"\n";
					str += "RECORD DURATION: " + ofToString(getRecordedDuration(), 1); str += +"\n";
					//error
					if (bRecording) {
						if (recorder.getAvgTimeSave() == 0) {
							std::string ss;
							const int p = 30;//period in frames
							int fn = ofGetFrameNum() % p;
							if (fn < p / 2.f)
								ss = "ERROR RECORDING!";
							else ss = "                ";
							str += ss + "\n";
							//y += 20;
						}
					}
				}
				else if (bRecPrepared)
				{
					str += "KEY U : START"; str += +"\n";
					str += "KEY u : UNMOUNT"; str += +"\n";

					//animated points..
					const int p = 30;//period in frames
					int fn = ofGetFrameNum() % (p * 4);
					bool b0, b1, b2;
					b0 = (fn > p * 3);
					b1 = (fn > p * 2);
					b2 = (fn > p * 1);
				std:string sp = "";
					if (b0) sp += ".";
					if (b1) sp += ".";
					if (b2) sp += ".";
					str += "MOUNTED > READY" + sp; str += "\n";
				}

				//-

				//red rec circle
				if (bRecording)
				{
					ofFill();
					ofSetColor(ofColor::red);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					ofNoFill();
					ofSetLineWidth(2.f);
					ofSetColor(ofColor::black);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					y += 28;
				}
				else if (bRecPrepared)
				{
					if (ofGetFrameNum() % 60 < 20) {
						ofFill();
						ofSetColor(ofColor::red);
						ofDrawCircle(ofPoint(x + 8, y), 8);
					}
					ofNoFill();
					ofSetLineWidth(2.f);
					ofSetColor(ofColor::black);
					ofDrawCircle(ofPoint(x + 8, y), 8);
					y += 28;
				}

				//info
				ofDrawBitmapStringHighlight(str, x, y);
				y += 20;

				ofPopStyle();
			}

			//-

			//help
			//y += 200;
			y = y + 135;
			//y = ofGetHeight() - 300;
			drawHelp(x, y);
		}
	}

	//--------------------------------------------------------------
	void drawHelp(int x = 50, int y = 50) {
		// help info
		ofDrawBitmapStringHighlight(textInfo, x, y);

		if (bRecording)
		{
			if (ofGetFrameNum() % 60 == 0) {
				ofLogWarning(__FUNCTION__) << ofGetFrameRate();
				ofLogWarning(__FUNCTION__) << "texture copy: " << recorder.getAvgTimeTextureCopy();
				ofLogWarning(__FUNCTION__) << "gpu download: " << recorder.getAvgTimeGpuDownload();
				ofLogWarning(__FUNCTION__) << "image encoding: " << recorder.getAvgTimeEncode();
				ofLogWarning(__FUNCTION__) << "file save: " << recorder.getAvgTimeSave() << endl;
			}
		}
	}

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs &eventArgs) {///to received short keys control commands
		const int key = eventArgs.key;

		// modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		//-

		switch (key)
		{
			//toggle show info
		case 'h':
			setToggleVisibleInfo();
			break;

			//mount prepare record
		case 'u':
			bRecPrepared = !bRecPrepared;
			ofLogWarning(__FUNCTION__) << "Mount: " << (bRecPrepared ? "ON" : "OFF");
			break;

			//start recording
		case 'U':
		{
			if (bRecording)//do stop
			{
				ofLogWarning(__FUNCTION__) << "Stop Recording";

				//bRecPrepared = false;
				bRecording = false;
			}
			else//do start
			{
				bRecording = true;
				timeStart = ofGetElapsedTimeMillis();
				ofLogWarning(__FUNCTION__) << "Start Recording into: " << _pathFolderStills;
			}
		}
		break;

		//take screenshot
		case OF_KEY_F11:
		{
			string _fileName = "snapshot_" + ofGetTimestampString() + ".png";
			string _pathFilename = ofToDataPath(_pathFolderSnapshots + _fileName, true);//bin/data

			ofImage img;
			img.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			bool b = img.save(_pathFilename);

			if (b) cout << __FUNCTION__ << " Saved screenshot successfully: " << _pathFilename << endl;
			else cout << __FUNCTION__ << " Error saving screenshot:" << _pathFilename << endl;
		}
		break;

		//set instagram size
		case 'i':
		{
			int w, h;
			w = 864;
			h = 1080 + 19;
			ofSetWindowShape(w, h);
			windowResized(w, h);

			cap_w = w;
			cap_h = h;
			buildAllocateFbo();
		}
		break;

		//refresh window size to update fbo settings
		case OF_KEY_F7:
			windowResized(ofGetWidth(), ofGetHeight());
			break;

			//set full hd
		case OF_KEY_F8:
			windowResized(1920, 1080);
			break;

			//remove all captures stills
		case OF_KEY_BACKSPACE: // ctrl + alt + backspace
			if (!mod_COMMAND && !mod_SHIFT && mod_ALT && mod_CONTROL)
			{
				std::string _path = _pathFolderStills;
				ofDirectory dataDirectory(ofToDataPath(_path, true));
				dataDirectory.remove(true);
				ofxSurfingHelpers::CheckFolder(_path);

				CaptureWindow.get
			}
			break;
		}
	}

	//--------------------------------------------------------------
	void windowResized(int w, int h) {///must be called to resize the fbo and video resolution
		cap_w = w;
		cap_h = h;
		buildAllocateFbo();
		//buildRecorder();
	}

private:
	//--------------------------------------------------------------
	void buildInfo() {///must be called after bitrate, framerate and size w/h are setted

		//build help info
		textInfo = "";
		textInfo += "HELP KEYS"; textInfo += "\n";
		textInfo += "h  : Show Help info"; textInfo += "\n";
		textInfo += "u  : Mount Recorder"; textInfo += "\n";
		textInfo += "U  : Start Recording"; textInfo += "\n";
		//textInfo += "i  : Set optimal Instagram size"; textInfo += "\n";
		textInfo += "F8 : Refresh Window size"; textInfo += "\n";
		textInfo += "F11: Capture Screenshot"; textInfo += "\n";
		textInfo += "Ctrl + Alt + BackSpace: Clear"; //textInfo += "\n";
	}
};