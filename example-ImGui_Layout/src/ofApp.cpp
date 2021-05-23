#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	guiManager.setup(gui);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	guiManager.begin();
	{
		auto mainSettings = ofxImGui::Settings();

		ImGuiColorEditFlags _flagw = ImGuiWindowFlags_None;
		string name = "myWindow";
		if (ofxImGui::BeginWindow(name.c_str(), mainSettings, _flagw))
		{
			drawWidgets();
		}
		ofxImGui::EndWindow(mainSettings);
	}
	guiManager.end();
}

//--------------------------------------------------------------
void ofApp::drawWidgets() {
	//ImGui::Dummy(ImVec2(0.0f, 20.0f));

	auto mainSettings = ofxImGui::Settings();

	float _spcx;
	float _spcy;
	float _w100;
	float _h100;
	float _w99;
	float _w50;
	float _w33;
	float _w25;
	float _h;
	ofxSurfingHelpers::refreshImGui_WidgetsSizes(_spcx, _spcy, _w100, _h100, _w99, _w50, _w33, _w25, _h);

	//ofxImGui::AddGroup(params_RandomizersPowered, mainSettings);

	ImGui::Dummy(ImVec2(0.0f, 5.0f));

	if (ImGui::Button("RANDOMIZE!", ImVec2(_w100, _h)))
	{
		//doRamdomize();
	}
	if (ImGui::Button("RESET", ImVec2(_w100, _h)))
	{
		//doReset();
	}
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
