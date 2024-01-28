#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	size_TTF = 12;
	string p = FONT_FILES_PATH + name_TTF;
	bool bLoaded = myFont.load(p, size_TTF, true, true);
	if (bLoaded) ofLogNotice(__FUNCTION__) << "Loaded " << p;
	if (!bLoaded) {
		p = FONT_FILES_PATH + ofToString(FONT_FILE_BIG);
		bLoaded = myFont.load(p, size_TTF, true, true);
		if (bLoaded)
			ofLogNotice(__FUNCTION__) << "Loaded " << p;
		else
			ofLogError(__FUNCTION__) << "Error loading " << p;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

#if 1
	string ss = R"(
	Lorem Ipsum is simply dummy text of the printing and typesetting industry.
	Lorem Ipsum has been the industry's standard dummy text ever since the 1500s,
	when an unknown printer took a galley of type and scrambled it
	to make a type specimen book. It has survived not only five centuries,
	but also the leap into electronic typesetting,
	remaining essentially unchanged. It was popularised
	in the 1960s with the release of Letraset sheets
	containing Lorem Ipsum passages, and more recently
	with desktop publishing software like
	Aldus PageMaker including versions of Lorem Ipsum.)";
#else
	string ss = "";
	ss += "Lorem Ipsum is simply dummy text of the printing and typesetting industry. \n";
	ss += "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, \n";
	ss += "when an unknown printer took a galley of type and scrambled it \n";
	ss += "to make a type specimen book. It has survived not only five centuries, \n";
	ss += "but also the leap into electronic typesetting, \n";
	ss += "remaining essentially unchanged. It was popularised \n";
	ss += "in the 1960s with the release of Letraset sheets \n";
	ss += "containing Lorem Ipsum passages, and more recently \n";
	ss += "with desktop publishing software like \n";
	ss += "Aldus PageMaker including versions of Lorem Ipsum.";
#endif

	x = 1;
	y = 1;

	ofRectangle _r;
	_r = (myFont.getStringBoundingBox(ss, x, y));
	cout << _r.x << ", ";
	cout << _r.y << ", ";
	cout << _r.getWidth() << ", ";
	cout << _r.getHeight();
	cout << endl;

	ofPushStyle();
	ofSetColor(ofColor::red);
	ofNoFill();
	ofDrawRectangle(_r);
	ofPopStyle();

	myFont.drawString(ss, x, y);
}


	//ofxSurfingHelpers::drawTextBoxed(myFont, ss, x, y);
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}