#pragma once

#include "ofMain.h"

class SurfingWebcam
{
public:

	SurfingWebcam() { setup(); }
	~SurfingWebcam() { exitWebcam(); }

private:
	
	void setup() { setupWebcamDevice(); }
	void setupWebcamDevice();
	void exitWebcam();
	
	ofVideoGrabber webcamGrab;
	ofParameter <std::string> webcamDeviceName{ "WEBCAM_DEVICE_NAME", "" };
	int _deviceIndex;
	ofTrueTypeFont font;
	string webcamDevicesNames;
	string path;

	void update() { webcamGrab.update(); };

public:
	
	void drawWebcam();
	
	void doNextWebcam();
	void doRestartWebcam();
	
	void drawInfo();

	bool bDrawWebcamInfo = false;
	ofParameter <bool>bKeys{ "Keys", false };
};

