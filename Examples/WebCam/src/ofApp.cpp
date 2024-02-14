#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	bGui_WebCam.set("WEBCAM", true);

	setup_WebCam_Params();
	
	// gui
	gui_WebCam.setup("WEBCAM");
	//gui_WebCam.setup("WEBCAM | " + webcam_Name.get());
	gui_WebCam.add(params_WebCam);
	//gui_WebCam.minimizeAll();
	
	//rect_WebCam.loadSettings(path_rect_WebCam, path_GLOBAL, false);
	rect_WebCam.setModeLayout(ofxSurfingBox::FREE_LAYOUT);
	rect_WebCam.setName(path_rect_WebCam);
	//rect_WebCam.setPathGlobal(path_GLOBAL + path_WebCamSettings);
	rect_WebCam.setup();
	rect_WebCam.setUseBorder(true);

#ifdef USE_OFX_CHILD_FRAME
	x = rect_WebCam.getX();
	y = rect_WebCam.getY();
	w = rect_WebCam.getWidth();
	h = rect_WebCam.getHeight();
	wc = w / 2.f;
	hc = h / 2.f;

	frame_.setSize(w, h);
	frame_.getInnerTransformNode().setAnchorPoint(x + wc, y + hc, 0);
	frame_.getInnerTransformNode().setTranslation(wc, hc, 0);
#endif

	name_WebCamSettings = "Settings_WebCam.xml";
	path_WebCamSettings = "WebCam/";

#ifdef USE_OFX_CHILD_FRAME
	ofAddListener(ofEvents().update, this, &ofApp
				  : update);
	ofAddListener(ofEvents().mouseScrolled, this, &ofApp
				  : mouseScrolled);
	ofAddListener(ofEvents().mouseDragged, this, &ofApp
				  : mouseDragged);
#endif

	//----

	webcam_LoadSettings();
}

//--------------------------------------------------------------
void ofApp::update() {
#ifdef FIX_WORKAROUND_STARTUP_FREEZE // Sometimes WebCam hangs on startup
	// workaround
	// restart camera after startup to make sure will be initialized fine.
	static const float _timeWait = 5.0f;
	// in x seconds at 60fps

	if (bDoRestartup && (ofGetFrameNum() == (int)(_timeWait * 60))) {
		//webcam_Grabber.close();
		doRestart_WebCam();
		bWebCam_Enable = true;
	}
#endif

	//--

	if (bWebCam_Enable.get()) {
		webcam_Grabber.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// WEBCAM
	if (bWebCam_Enable && bWebCam_Draw) {
		draw_WebCam();
	}

	// WEBCAM preview
	if (bWebCam_Enable && bWebCam_Draw) {
		if (bWebCam_Enable)
			if (bWebCam_Draw)
				if (bWebCam_DrawMini) draw_WebCam_MiniPreview(true);
	}

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene() {
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	//if (bGui_WebCam) {
	//	//gui_WebCam.setPosition(p);
	//	//p = gui_WebCam.getShape().getTopRight() + pad;
	//}
	if (bGui_WebCam) gui_WebCam.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//TODO
	//bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_ALT = false;

	if (key == 'I') {
		webcam_Next();
	}

	else if (mod_ALT)
	//else if (key == key_ChildFrame)
	{
		bKeyChildFrameState = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	//TODO
	//bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_ALT = false;

	//if (key == key_ChildFrame)
	if (mod_ALT || key == 4) {
		bKeyChildFrameState = false;
	}
}

//--------------------------------------------------------------
void ofApp::webcam_Next() {
	auto _devs = webcam_Grabber.listDevices();
	webcam_Index_Device++;
	if (webcam_Index_Device.get() > _devs.size() - 1) webcam_Index_Device = 0;
	webcam_Name_ = _devs[webcam_Index_Device].deviceName;
	webcam_Name = webcam_Name_;

	// select cam device
	webcam_Grabber.close();
	webcam_Grabber.setDeviceID(webcam_Index_Device.get());
	webcam_Grabber.setup(1920, 1080);
}

//--------------------------------------------------------------
void ofApp::setup_WebCam_Params() {
	bWebCam_Restart.set("RESTART", false);
	bWebCam_Next.set("NEXT", false);
	bWebCam_Enable.set("ENABLE", false);
	bWebCam_LockRatio.set("LOCK ASPECT", true);
	bWebCam_Draw.set("DRAW", true);
	bWebCam_DrawMini.set("MINI", true);
	webcam_Index_Device.set("DEVICE", 0, 0, 1);
	scaleMode_Index.set("SCALE", 0, 0, 3);
	scaleMode_Name.set("NAME", "");
	webcam_Name.set("", "");

	webcam_Index_Device.setSerializable(false);
	webcam_Name.setSerializable(false);
	scaleMode_Name.setSerializable(false);
	bWebCam_Restart.setSerializable(false);
	bWebCam_Next.setSerializable(false);

	params_WebCam.setName("WEBCAM");
	params_WebCam.add(bWebCam_Enable);
	params_WebCam.add(bWebCam_Draw);
	params_WebCam.add(bWebCam_DrawMini);
	params_WebCam.add(bWebCam_Next);
	params_WebCam.add(webcam_Index_Device);
	params_WebCam.add(webcam_Name);
	params_WebCam.add(scaleMode_Index);
	params_WebCam.add(scaleMode_Name);
	params_WebCam.add(bWebCam_LockRatio);
	params_WebCam.add(rect_WebCam.bEdit);
	params_WebCam.add(bWebCam_Restart);

	scaleMode_Names.clear();
	scaleMode_Names.push_back("FIT");
	scaleMode_Names.push_back("FILL");
	scaleMode_Names.push_back("CENTER");
	scaleMode_Names.push_back("STRETCH_TO_FILL");

	ofAddListener(params_WebCam.parameterChangedE(), this, &ofApp::Changed_WebCam);
}

//--

// TODO:
//--------------------------------------------------------------
void ofApp::doRestart_WebCam() {
	// must close before reopen
	webcam_Grabber.close();

	// start device
	{
		webcam_Grabber.setDeviceID(webcam_Index_Device.get());
		//webcam_Grabber.setDesiredFrameRate(60);
		webcam_Grabber.setup(1920, 1080);
	}
}

//--------------------------------------------------------------
void ofApp::setup_WebCam(int index) {
	// will load from index (webcam_Index_Device) not from name

	// get back a list of devices.
	webcam_Grabber.setVerbose(true);
	vector<ofVideoDevice> _devs = webcam_Grabber.listDevices();
	webcam_Index_Device.setMax(_devs.size() - 1);

	// get all dev names
	webcam_Names_InputDevices = "";
	webcam_Names.clear();
	for (size_t i = 0; i < _devs.size(); i++) {
		// queue devices names
		webcam_Names_InputDevices += ofToString(i) + " " + _devs[i].deviceName;
		if (i != _devs.size() - 1) webcam_Names_InputDevices += "\n";

		webcam_Names.push_back(_devs[i].deviceName);
	}

	//-

	// check valid index and clamp
	bool bError = false;
	if (webcam_Index_Device > _devs.size() - 1) {
		ofLogError(__FUNCTION__) << "CAMERA INDEX OUT OF RANGE";
		webcam_Index_Device = -1;
		webcam_Name = webcam_Name_ = "UNKNOWN DEVICE";

		bError = true;
	}

	// valid index
	if (!bError) {
		// must close before reopen
		webcam_Grabber.close();

		// start device
		{
			webcam_Grabber.setDeviceID(webcam_Index_Device.get());

			//TODO:
			// now size is hardcoded
			//
			//webcam_Grabber.setDesiredFrameRate(60);
			webcam_Grabber.setup(1920, 1080);
		}

		//--

		// debug connected
		bool _isConnected = webcam_Grabber.isInitialized();
		ofLogNotice(__FUNCTION__) << "webcam_Grabber INITIALIZED: " << (_isConnected ? "TRUE" : "FALSE");

		if (!_isConnected) {
			ofLogError(__FUNCTION__) << "CAN'T INITIALIZE webcam_Grabber!";
			ofLogError(__FUNCTION__) << "CAMERA DISABLED";

			bWebCam_Enable = false;

			//return;
		}

		//--

		if (webcam_Index_Device < _devs.size()) {
			webcam_Name_ = _devs[webcam_Index_Device].deviceName;
			webcam_Name = webcam_Name_;
		}

		//--

		////TODO: child frame
		//float w = webcam_Grabber.getWidth();
		//float h = webcam_Grabber.getHeight();
		//float wc = w / 2.f;
		//float hc = h / 2.f;
		//frame_.setSize(w, h);
		//frame_.getInnerTransformNode().setAnchorPoint(wc, hc, 0);
		//frame_.getInnerTransformNode().setTranslation(wc, hc, 0);
	}
}

#ifdef USE_OFX_CHILD_FRAME

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs & mouse) {

	int x = mouse.x;
	int y = mouse.y;
	int button = mouse.button;

	if (!bEnable_ChildFrame) return;
	if (!bKeyChildFrameState) return;

	switch (button) {

		//case OF_MOUSE_BUTTON_2:
	case OF_MOUSE_BUTTON_LEFT: {
		TransformNode & node = frame_.getInnerTransformNode();
		node.addTranslationX(x - ofGetPreviousMouseX());
		node.addTranslationY(y - ofGetPreviousMouseY());

		childFrame.anchor = node.getAnchorPoint();
		childFrame.translat = node.getTranslation();
		childFrame.scale = node.getScale();
	} break;

	case OF_MOUSE_BUTTON_RIGHT: {
		TransformNode & node = frame_.getInnerTransformNode();

		float sz = -100;
		float diff = y - ofGetPreviousMouseY();
		//float diff = MAX(x - ofGetPreviousMouseX(), y - ofGetPreviousMouseY());
		ofVec3f v(sz + diff, sz + diff, sz);
		node.mulScale(v / sz);

		// Clamp
		auto s = node.getScale();
		float sxclamp = ofClamp(s.x, 1, CHILD_FRAME_MAX_SCALE);
		float syclamp = ofClamp(s.y, 1, CHILD_FRAME_MAX_SCALE);
		node.setScaleX(sxclamp);
		node.setScaleY(syclamp);

		childFrame.anchor = node.getAnchorPoint();
		childFrame.translat = node.getTranslation();
		childFrame.scale = node.getTranslation();
	} break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(ofMouseEventArgs & mouse) {
	if (!bEnable_ChildFrame) return;
	if (!bKeyChildFrameState) return;

	float ms = mouse.scrollY;
	float step = 5;
	float st = ofMap(ms, -2, 2, -step, step);

	TransformNode & node = frame_.getInnerTransformNode();

	float n = 100; //bigger is slower/sensible

	ofVec3f v(n + st, n + st, n);

	//v = ofVec3f (
	//	ofClamp(v.x, 1, CHILD_FRAME_MAX_SCALE),
	//	ofClamp(v.y, 1, CHILD_FRAME_MAX_SCALE),
	//	ofClamp(n, 1, CHILD_FRAME_MAX_SCALE));

	node.mulScale(v / n);

	// Clamp
	auto s = node.getScale();
	float sxclamp = ofClamp(s.x, 1, CHILD_FRAME_MAX_SCALE);
	float syclamp = ofClamp(s.y, 1, CHILD_FRAME_MAX_SCALE);
	node.setScaleX(sxclamp);
	node.setScaleY(syclamp);

	childFrame.anchor = node.getAnchorPoint();
	childFrame.translat = node.getTranslation();
	childFrame.scale = node.getTranslation();
}

#endif

//--------------------------------------------------------------
void ofApp::setup_WebCam() {
	ofLogNotice(__FUNCTION__);

	// List the cam devices. nothing more yet!

	// will initialized the device from the loaded name from the xml settings, not from the index number!
	// that's because the index of a device could be changed if a new device is connected.

	// get back a list of devices.
	webcam_Grabber.setVerbose(true);
	//vector<ofVideoDevice> _devs = webcam_Grabber.listDevices();
	_devs = webcam_Grabber.listDevices();

	// get all dev names
	webcam_Names_InputDevices = "";
	webcam_Index_Device.setMax(_devs.size() - 1);
	ofLogNotice(__FUNCTION__) << "LIST WEBCAM DEVICES:";

	webcam_Names.clear();

	// log list devices
	for (size_t i = 0; i < _devs.size(); i++) {
		////TODO:
		////should skip names for not available devices...
		//if (_devs[i].bAvailable)
		//{

		// queue devices names
		webcam_Names_InputDevices += ofToString(i) + " " + _devs[i].deviceName;

		webcam_Names_InputDevices += "\n";
		//if (i != _devs.size() - 1) webcam_Names_InputDevices += "\n";

		if (_devs[i].bAvailable) {
			// log the device
			ofLogNotice(__FUNCTION__) << _devs[i].id << ": " << _devs[i].deviceName;
		} else {
			// log the device and note it as unavailable
			ofLogNotice(__FUNCTION__) << _devs[i].id << ": " << _devs[i].deviceName << " - unavailable ";
		}

		webcam_Names.push_back(_devs[i].deviceName);
	}

	//--

#ifdef USE_WEBCAM
	//rect_WebCam.loadSettings(path_rect_WebCam, path_GLOBAL, false);
	rect_WebCam.setModeLayout(ofxSurfingBox::FREE_LAYOUT);
	rect_WebCam.setName(path_rect_WebCam);
	rect_WebCam.setPathGlobal(path_GLOBAL + path_WebCamSettings);
	rect_WebCam.setup();
	rect_WebCam.setUseBorder(true);

	#ifdef USE_OFX_CHILD_FRAME
	x = rect_WebCam.getX();
	y = rect_WebCam.getY();
	w = rect_WebCam.getWidth();
	h = rect_WebCam.getHeight();
	wc = w / 2.f;
	hc = h / 2.f;

	frame_.setSize(w, h);
	frame_.getInnerTransformNode().setAnchorPoint(x + wc, y + hc, 0);
	frame_.getInnerTransformNode().setTranslation(wc, hc, 0);
	#endif
#endif
}

//--------------------------------------------------------------
void ofApp::draw_WebCam_MiniPreview(bool bInfo) {
	if (!bWebCam_Enable) return;
	if (!bWebCam_Draw) return;
	if (!bWebCam_DrawMini) return;

	//----

	ofPushStyle();
//ofSetColor(255, 255);

//TODO:
#ifdef USE_OFX_CHILD_FRAME

	//TODO: child frame

	if (rect_WebCam.isChanged()) {
		//cout << __FUNCTION__ << "rect_WebCam.isChanged()" << endl;

		bEnable_ChildFrame = !rect_WebCam.isEditing();

		x = rect_WebCam.getX();
		y = rect_WebCam.getY();
		w = rect_WebCam.getWidth();
		h = rect_WebCam.getHeight();
		wc = w / 2.f;
		hc = h / 2.f;

		frame_.setSize(w, h);

		////if (bEnable_ChildFrame)
		//{
		//  frame_.setSize(w, h);
		//	TransformNode& node = frame_.getInnerTransformNode();
		//	node.setAnchorPoint(x + wc, y + hc, 0);
		//	node.setTranslation(wc, hc, 0);
		//	//node.setAnchorPoint(childFrame.anchor);
		//	//node.setTranslation(childFrame.translat);
		//	//node.setScale(childFrame.scale);
		//}
	}

	//if (bEnable_ChildFrame)
	{
		x = rect_WebCam.getX();
		y = rect_WebCam.getY();
		w = rect_WebCam.getWidth();
		h = rect_WebCam.getHeight();
		wc = w / 2.f;
		hc = h / 2.f;

		// required to mantain position after moving box..
		//frame_.setSize(w, h);
		TransformNode & node = frame_.getInnerTransformNode();
		node.setAnchorPoint(x + wc, y + hc, 0);
		node.setTranslation(wc, hc, 0);

		/*
		TransformNode& node = frame_.getInnerTransformNode();
		node.setAnchorPoint(childFrame.anchor);
		node.setTranslation(childFrame.translat);
		node.setScale(childFrame.scale);
		*/
	}

	// draw
	frame_.begin();
	ofSetColor(255, 255);
	webcam_Grabber.draw(x, y, w, h);
	frame_.end();
	frame_.draw(x, y);

	// debug
	if (0) {
		stringstream ss;
		ss << "anchor     : " << childFrame.anchor << endl;
		ss << "translate  : " << childFrame.translat << endl;
		ss << "scale      : " << childFrame.scale << endl
		   << endl;
		ofDrawBitmapStringHighlight(ss.str(), x + w + 10, y + h + 10);
	}
#endif

	int x, y, w, h;

#ifndef USE_OFX_CHILD_FRAME
	x = rect_WebCam.getX();
	y = rect_WebCam.getY();
	w = rect_WebCam.getWidth();
	h = rect_WebCam.getHeight();
	//ofSetColor(255, 255);
	webcam_Grabber.draw(x, y, w, h);
#endif

	//--

	//TODO:

	// Viewport

	ofRectangle rSrc(0, 0, webcam_Grabber.getWidth(), webcam_Grabber.getHeight());
	ofRectangle rOut(rect_WebCam.getX(), rect_WebCam.getY(), rect_WebCam.getWidth(), rect_WebCam.getHeight());

	if (bWebCam_LockRatio.get()) {
		float _ratio = webcam_Grabber.getHeight() / webcam_Grabber.getWidth();
		rect_WebCam.setHeight(rect_WebCam.getWidth() * _ratio);
	}

	ofScaleMode scaleMode;
	switch (scaleMode_Index) {
	case 0:
		scaleMode = OF_SCALEMODE_FIT;
		scaleMode_Name = "FIT";
		break;
	case 1:
		scaleMode = OF_SCALEMODE_FILL;
		scaleMode_Name = "FILL";
		break;
	case 2:
		scaleMode = OF_SCALEMODE_CENTER;
		scaleMode_Name = "CENTER";
		break;
	case 3:
		scaleMode = OF_SCALEMODE_STRETCH_TO_FILL;
		scaleMode_Name = "STRETCH_TO_FILL";
		break;
	}

	//rOut.scaleTo(rSrc, scaleMode);
	rSrc.scaleTo(rOut, scaleMode);

	// image

	////webcam_Grabber.draw(rect_WebCam.getX(), rect_WebCam.getY(), rect_WebCam.getWidth(), rect_WebCam.getHeight());

	////webcam_Grabber.draw(rOut.getX(), rOut.getY(), rOut.getWidth(), rOut.getHeight());
	//webcam_Grabber.draw(rOut.getX(), rOut.getY(), rOut.getWidth(), rOut.getHeight());

	// interactive box
	rect_WebCam.draw();

	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::webcam_SaveSettings() {
	ofLogNotice(__FUNCTION__);

	ofXml _xml;
	ofSerialize(_xml, webcam_Name_);
	ofxSurfingHelpers::CheckFolder(path_GLOBAL + path_WebCamSettings);
	_xml.save(path_GLOBAL + path_WebCamSettings + name_WebCamSettings);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogNotice(__FUNCTION__);

	webcam_SaveSettings();
}

//--------------------------------------------------------------
void ofApp::webcam_LoadSettings() {
	ofLogNotice(__FUNCTION__);

	//--

#ifdef FIX_WORKAROUND_STARTUP_FREEZE // Sometimes WebCam hangs on startup

	// fix workaround startup
	if (bWebCam_Enable.get()) {
		bDoRestartup = true;
		bWebCam_Enable = false;
		webcam_Grabber.close();
	}

#endif

	//--

	//TODO: use this file for settings.
	//TODO: camera is loading from index on app settings...

	ofXml _xml;
	bool _isLoaded = _xml.load(path_GLOBAL + path_WebCamSettings + name_WebCamSettings);
	ofDeserialize(_xml, webcam_Name_);

	ofLogNotice(__FUNCTION__) << endl
							  << "XML CONTENT : \n"
							  << _xml.toString();
	ofLogNotice(__FUNCTION__) << endl
							  << "LOOKING FOR DEVICE : \n"
							  << webcam_Name_.get();

	//--

	webcam_Index_Device.setWithoutEventNotifications(-1);

	if (_isLoaded) {
		// Iterate devices names to found the device name read from the file settings.
		for (int i = 0; i < _devs.size(); i++) {
			ofLogNotice(__FUNCTION__) << "DEVICE #" << i << " " << _devs[i].deviceName;

			if (_devs[i].deviceName == webcam_Name_.get()) {
				webcam_Index_Device = i;

				ofLogNotice(__FUNCTION__) << "DEVICE FROM SETTINGS FOUND!";
				ofLogNotice(__FUNCTION__) << "DEVICE NAME  : \t" << webcam_Name_.get();
				ofLogNotice(__FUNCTION__) << "DEVICE INDEX : \t" << webcam_Index_Device;

				break;
			}
		}
	} else {
		ofLogError(__FUNCTION__) << "SETTINGS FILE NOT FOUND!";
		ofLogError(__FUNCTION__) << "FORCE TRY TO LOAD THE FIRST AVAILABLE DEVICE";
	}

	// Error. Desired name is not on the current list of devices!
	// Then we try to load the first device...

	if (webcam_Index_Device == -1) {
		webcam_Index_Device = 0; // force select first cam device

		if (webcam_Index_Device < _devs.size()) {
			webcam_Name_ = _devs[webcam_Index_Device].deviceName;
			webcam_Name = _devs[webcam_Index_Device].deviceName;
		} else {
			ofLogError(__FUNCTION__) << "CAMERA INDEX OUT OF RANGE";
			webcam_Name = webcam_Name_ = "UNKNOWN DEVICE";
			webcam_Grabber.close();
			return; //cancel and exit!
		}
	}

	webcam_Name = webcam_Name_;

	//--

	// 1. Must close before reopen

	webcam_Grabber.close();

	//--

	// 2. Start device

	//if (bWebCam_Enable.get())
	{
		ofLogNotice(__FUNCTION__) << "OPENING DEVICE!";

		webcam_Grabber.setDeviceID(webcam_Index_Device.get());
		//webcam_Grabber.setDesiredFrameRate(60);
		webcam_Grabber.setup(1920, 1080);
	}

	//--

	//// debug connected
	//bool _isConnected = webcam_Grabber.isInitialized();
	//ofLogNotice(__FUNCTION__) << "webcam_Grabber INITIALIZED: " << (_isConnected ? "TRUE" : "FALSE");
	//if (!_isConnected) {
	//	ofLogError(__FUNCTION__) << "CAN'T INITIALIZE webcam_Grabber!";
	//	ofLogError(__FUNCTION__) << "CAMERA DISABLED";
	//	bWebCam_Enable = false;
	//}
}

//--------------------------------------------------------------
void ofApp::Changed_WebCam(ofAbstractParameter & e) {
	string name = e.getName();

	// exclude debugs
	if (name != scaleMode_Name.getName()
		//	&& name != "exclude"
	) {
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	if (0) {
	}

	//----

	else if (name == bWebCam_Enable.getName()) {
		if (bWebCam_Enable.get())
			doRestart_WebCam();
		else
			webcam_Grabber.close();
	}

	else if (name == webcam_Name.getName()) {
		//TODO:
		// 1. get index for a name
		// 2. setup cam for index
	}

	else if (name == webcam_Index_Device.getName() && bWebCam_Enable.get()) {
		setup_WebCam(webcam_Index_Device.get());
	}

	else if (name == bWebCam_Next.getName() && bWebCam_Next.get()) {
		bWebCam_Next = false;
		webcam_Next();
	}

	else if (name == bWebCam_Restart.getName() && bWebCam_Restart) {
		bWebCam_Restart = false;

		bWebCam_Enable = false;
		// must close before reopen
		webcam_Grabber.close();
		//doRestart_WebCam();
		setup_WebCam(webcam_Index_Device);
		bWebCam_Enable = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw_WebCam() {
	if (!bWebCam_Enable) return;
	if (!bWebCam_Draw) return;

	if (bWebCam_DrawMini)
		draw_WebCam_MiniPreview(true);
	else
		draw_WebCam_Full();
}

//--------------------------------------------------------------
void ofApp::draw_WebCam_Full() {
	if (!bWebCam_Enable) return;
	if (!bWebCam_Draw) return;

	//TODO:
	// making fit view ports

	ofPushStyle();
	ofSetColor(255, 255);

	ofScaleMode scaleMode;
	switch (scaleMode_Index) {
	case 0:
		scaleMode = OF_SCALEMODE_FIT;
		scaleMode_Name = "FIT";
		break;
	case 1:
		scaleMode = OF_SCALEMODE_FILL;
		scaleMode_Name = "FILL";
		break;
	case 2:
		scaleMode = OF_SCALEMODE_CENTER;
		scaleMode_Name = "CENTER";
		break;
	case 3:
		scaleMode = OF_SCALEMODE_STRETCH_TO_FILL;
		scaleMode_Name = "STRETCH_TO_FILL";
		break;
	}

	ofRectangle rOut = ofGetWindowRect();
	//ofRectangle rOut(0, 0, NDI_OUT_Sender.GetWidth(), NDI_OUT_Sender.GetHeight());

	ofRectangle rSrc(0, 0, webcam_Grabber.getWidth(), webcam_Grabber.getHeight());

	rOut.scaleTo(rSrc, scaleMode);
	//rSrc.scaleTo(rOut, scaleMode);

	webcam_Grabber.draw(rOut.x, rOut.y, rOut.width, rOut.height);
	//webcam_Grabber.draw(rSrc.x, rSrc.y, rSrc.width, rSrc.height);

	ofPopStyle();
}