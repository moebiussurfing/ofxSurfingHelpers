#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	size_TTF = 12;
	name_TTF = "overpass-mono-bold.otf";
	string p = FONT_FILES_PATH + name_TTF;
	bool bLoaded = myFont.load(p, size_TTF, true, true);
	if (bLoaded) ofLogNotice(__FUNCTION__) << "Loaded " << p;
	if (!bLoaded) {
		p = FONT_FILES_PATH + ofToString(FONT_FILE_BIG);//default font file 
		bLoaded = myFont.load(p, size_TTF, true, true);
		if (bLoaded)
			ofLogNotice(__FUNCTION__) << "Loaded " << p;
		else {
			ofLogError(__FUNCTION__) << "Error loading " << p;
			bLoaded = myFont.load(OF_TTF_SANS, size_TTF, true, true);//OF_TTF_SANS is broken in OF Windows...
			if (!bLoaded)
				ofLogError(__FUNCTION__) << "Error loading " << OF_TTF_SANS;
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

#if 0
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
	ss += "Lorem Ipsum is simply dummy text of the printing and typesetting industry.\n";
	ss += "Lorem Ipsum has been the industry's standard dummy text ever since the 1500s,\n";
	ss += "when an unknown printer took a galley of type and scrambled it\n";
	ss += "to make a type specimen book. It has survived not only five centuries,\n";
	ss += "but also the leap into electronic typesetting,\n";
	ss += "remaining essentially unchanged. It was popularised\n";
	ss += "in the 1960s with the release of Letraset sheets\n";
	ss += "containing Lorem Ipsum passages, and more recently\n";
	ss += "with desktop publishing software like\n";
	ss += "Aldus PageMaker including versions of Lorem Ipsum.";
#endif

	x = 2;
	y = 2;

	ofxSurfingHelpers::drawTextBoxed(myFont, ss, x, y);

	auto sz2 = ofxSurfingHelpers::getShapeBBtextBoxed(myFont, ss);
	ofxSurfingHelpers::drawTextBoxed(myFont, ss, ofGetWidth() - sz2.x - x, ofGetHeight() - sz2.y - y, ofColor::black, ofColor::orange);
}

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