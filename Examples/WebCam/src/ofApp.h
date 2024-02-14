#pragma once
#include "ofMain.h"

#include "SurfingWebcam.h"
#include "ofxSurfingBoxInteractive.h"
#include "ofxGui.h"

//#define USE_WEBCAM // WebCam as camera input.
//TODO:
// WIP: BUG: could break WebCam.
// Requires ofxChildFrame.
// For transform the WebCam content:
// zoom and translate / crop viewport to send.
//#define USE_OFX_CHILD_FRAME

#ifdef USE_OFX_CHILD_FRAME
	#undef USE_OFX_CHILD_FRAME
	#include "ofxChildFrame.h"
	#define CHILD_FRAME_MAX_SCALE 5.f
#endif

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void drawScene();
	void drawGui();
	void exit();
	void keyPressed(int key);
	void keyReleased(int key);

	void Changed_WebCam(ofAbstractParameter & e);

	ofParameter<bool> bGui_WebCam;

private:
	ofVideoGrabber webcam_Grabber;
	void setup_WebCam(); // setup webcam from name device nor index
	void setup_WebCam(int index); // setup webcam from device index
	//void exit_WebCam(); // store camera device name to xml
	vector<ofVideoDevice> _devs;

	//CHILD_FRAME
	struct ChildFrame {
		ofVec3f anchor;
		ofVec3f translat;
		ofVec3f scale;
	};
	ChildFrame childFrame;
	//char key_ChildFrame = OF_KEY_COMMAND;
	bool bKeyChildFrameState = false;

	//--

	void setup_WebCam_Params();

	void webcam_SaveSettings();
	void webcam_LoadSettings();
	void webcam_Next();

public:
	void doRestart_WebCam(); // restart camera using the current index camera

	void draw_WebCam(); //-> for internal use
	void draw_WebCam_Full();
	void draw_WebCam_MiniPreview(bool bInfo = false);

	//private:

	ofParameter<bool> bWebCam_LockRatio;
	ofParameter<bool> bWebCam_Next;
	ofParameter<int> scaleMode_Index;
	ofParameter<string> scaleMode_Name;
	vector<std::string> scaleMode_Names;
	ofParameter<bool> bWebCam_Enable;
	ofParameter<bool> bWebCam_Restart;
	ofParameter<bool> bWebCam_Draw;
	std::string webcam_Names_InputDevices;
	vector<std::string> webcam_Names;
	ofParameter<std::string> webcam_Name_ { "WEBCAM_DEVICE_NAME", "" };
	ofParameter<std::string> webcam_Name; // can be merged both vars?
	ofParameter<bool> bWebCam_DrawMini;
	ofParameter<int> webcam_Index_Device;

public:
	ofParameterGroup params_WebCam;

private:
	std::string path_WebCamSettings;
	std::string name_WebCamSettings;

	std::string path_rect_WebCam = "WebCam_Mini";

public:
	ofxSurfingBoxInteractive rect_WebCam;

	ofxPanel gui_WebCam;
	int pad = 10;

string path_GLOBAL = "";
};
