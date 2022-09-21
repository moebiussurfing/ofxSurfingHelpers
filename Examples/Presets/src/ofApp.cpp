#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	bEnable.set("Bloom", false);
	scale.set("Scale", 0.1f, 0.1f, 16.f);
	thresh.set("Threshold", 0.1f, 0.1f, 2.f);
	brightness.set("Brightness", 2.5f, 0.f, 30.f);

	params.setName("Bloom");
	params.add(bEnable);
	params.add(scale);
	params.add(brightness);
	params.add(thresh);

	surfingPresets.setUiPtr(&ui);
	
	surfingPresets.setPath(pathGlobal);

	surfingPresets.AddGroup(params);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp")) {
			
			ui.Add(ui.bMinimize, OFX_IM_TOGGLE_ROUNDED_SMALL);
			ui.AddSpacingSeparated();

			ui.AddGroup(params);
			ui.EndWindow();
		};

		surfingPresets.drawImGui(true);
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
