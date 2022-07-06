#include "SurfingWebcam.h"


//--------------------------------------------------------------
void SurfingWebcam::setupWebcamDevice()
{
	path = "Webcam_Settings.xml";

	string str = "JetBrainsMono-ExtraBold.ttf";
	//string str = "overpass-mono-bold.otf";

	string pathFont = "assets/fonts/" + str; // data/assets/
	bool b = font.load(pathFont, 10);
	if (!b) font.load(OF_TTF_MONO, 10);

	// load settings
	ofXml _xml;
	bool _isLoaded;
	_isLoaded = _xml.load(path);
	ofDeserialize(_xml, webcamDeviceName);
	ofLogNotice(__FUNCTION__) << _xml.toString();

	// start
	auto _devices = webcamGrab.listDevices();
	_deviceIndex = -1;
	webcamDevicesNames = "";

	if (_isLoaded)
	{
		for (int i = 0; i < _devices.size(); i++)
		{
			if (_devices[i].deviceName == webcamDeviceName.get())
			{
				_deviceIndex = i;
				ofLogNotice(__FUNCTION__) << "device name: \t" << webcamDeviceName.get();
				ofLogNotice(__FUNCTION__) << "device index: \t" << _deviceIndex;
			}

			webcamDevicesNames += ofToString(i) + " " + _devices[i].deviceName + "\n";
		}
	}
	// error. try to load first device...
	if (_deviceIndex == -1)
	{
		_deviceIndex = 0; // select cam device
		webcamDeviceName = _devices[_deviceIndex].deviceName;
	}

	// Start device
	webcamGrab.setVerbose(true);
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}

//--------------------------------------------------------------
void SurfingWebcam::drawInfo()
{
	if (!bDrawWebcamInfo) return;

	// Display device name
	string str;
	str += "WEBCAM \n";
	str += webcamDeviceName.get();
	str += " #" + ofToString(_deviceIndex) /*+ " \n"*/;
	//str += " " + ofToString(webcamGrab.isInitialized() ? "[ON]" : "[OFF]");
	str += "\n\n";
	str += webcamDevicesNames;
	str += "\n";
	if (bKeys)
	{
		str += "KEY COMMANDS \n";
		str += "R RESTART \n";
		str += "D NEXT \n";
	}

	//int xx = 30;
	//int yy = 40;
	int pd = 10;
	int ww = font.getStringBoundingBox(str, 0, 0).getWidth();
	int hh = font.getStringBoundingBox(str, 0, 0).getHeight();
	int xx = ofGetWidth() / 2 - ww / 2;
	int yy = ofGetHeight() - hh - pd;

	if (!font.isLoaded()) ofDrawBitmapStringHighlight(str, xx, yy);
	else
	{
		ofPushStyle();

		// bbox
		ofSetColor(0, 150);
		ofFill();
		ofRectangle _r(font.getStringBoundingBox(str, xx, yy));
		int pad = 20;
		_r.setWidth(_r.getWidth() + pad);
		_r.setHeight(_r.getHeight() + pad);
		_r.setX(_r.getPosition().x - pad / 2.);
		_r.setY(_r.getPosition().y - pad / 2.);
		//_r.scaleFromCenter(1.05, 1.5);
		ofDrawRectRounded(_r, 5.);

		// text
		ofSetColor(255);
		ofNoFill();
		font.drawString(str, xx, yy);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void SurfingWebcam::drawWebcam() {
	update();

	ofRectangle r(0, 0, webcamGrab.getWidth(), webcamGrab.getHeight());
	r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_CENTER);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_STRETCH_TO_FILL);
	//r.scaleTo(ofGetWindowRect(), OF_SCALEMODE_FILL);
	webcamGrab.draw(r.x, r.y, r.width, r.height);
}

//--------------------------------------------------------------
void SurfingWebcam::exitWebcam() {
	ofXml _xml;
	ofSerialize(_xml, webcamDeviceName);
	_xml.save(path);
}

//--------------------------------------------------------------
void SurfingWebcam::doRestartWebcam() {

	webcamGrab.close();
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}

//--------------------------------------------------------------
void SurfingWebcam::doNextWebcam() {
	auto _devs = webcamGrab.listDevices();
	_deviceIndex++;
	if (_deviceIndex > _devs.size() - 1) _deviceIndex = 0;
	webcamDeviceName = _devs[_deviceIndex].deviceName;

	webcamGrab.close();
	webcamGrab.setDeviceID(_deviceIndex);
	webcamGrab.setup(1920, 1080);
}