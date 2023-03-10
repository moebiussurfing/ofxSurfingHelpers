#include "ofApp.h"

////--------------------------------------------------------------
//void ofApp::setup() {
//}
//
////--------------------------------------------------------------
//void ofApp::update() {
//}

//--------------------------------------------------------------
void ofApp::draw()
{
	int i = 1;
	int x = 20;
	int y = 20;
	string s;

	s = "[" + ofToString(m.isEnabledAutoHide() ? "x" : " ") + ofToString("]  [RETURN]  Enable auto hide mouse pointer.");
	ofDrawBitmapStringHighlight(s, 20, y + 20 * i++);
	i++;

	s = "[" + ofToString(m.isEnabledRuler() ? "x" : " ") + ofToString("]  [SPACE]   Enable mouse ruler.");
	ofDrawBitmapStringHighlight(s, 20, y + 20 * i++);
	if (m.isEnabledRuler()) {
		ofDrawBitmapStringHighlight("Move the mouse around to display position", 20, y + 20 * i++);
		ofDrawBitmapStringHighlight("Drag the mouse to display distances", 20, y + 20 * i++);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') { m.setToggleVisibleRuler(); }
	if (key == OF_KEY_RETURN) { m.setToogleAutoHide(); }
}

////--------------------------------------------------------------
//void ofApp::keyReleased(int key) {
//}
//
////--------------------------------------------------------------
//void ofApp::windowResized(int w, int h) {
//}
//
////--------------------------------------------------------------
//void ofApp::gotMessage(ofMessage msg) {
//
//}
//
////--------------------------------------------------------------
//void ofApp::dragEvent(ofDragInfo dragInfo) {
//
//}


////--------------------------------------------------------------
//void ofApp::mouseMoved(ofMouseEventArgs& args) {
//}
//
////--------------------------------------------------------------
//void ofApp::mouseDragged(ofMouseEventArgs& args) {
//}
//
////--------------------------------------------------------------
//void ofApp::mousePressed(ofMouseEventArgs& args) {
//}
//
////--------------------------------------------------------------
//void ofApp::mouseReleased(ofMouseEventArgs& args) {
//}