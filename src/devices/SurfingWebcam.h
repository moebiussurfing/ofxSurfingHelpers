#pragma once

#include "ofMain.h"

class SurfingWebCam {
public:
	SurfingWebCam() { setup(); }
	~SurfingWebCam() { exitWebCam(); }

private:
	void setup() { setupWebCamDevice(); }
	void setupWebCamDevice();
	void exitWebCam();

	ofVideoGrabber webcamGrab;
	ofParameter<std::string> webcamDeviceName { "WEBCAM_DEVICE_NAME", "" };
	int _deviceIndex;
	ofTrueTypeFont font;
	string webcamDevicesNames;
	string path;

	void update() { webcamGrab.update(); };

public:
	void drawWebCam();

	void doNextWebCam();
	void doRestartWebCam();

	void drawInfo();

	bool bDrawWebCamInfo = false;
	ofParameter<bool> bKeys { "Keys", false };
};
