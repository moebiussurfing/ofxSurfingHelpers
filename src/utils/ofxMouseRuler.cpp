#include "ofxMouseRuler.h"

ofxMouseRuler::ofxMouseRuler() {

	setupDone = false;
	visible = false;
}

ofxMouseRuler::~ofxMouseRuler() {
	if (setupDone) {
		ofUnregisterMouseEvents(this);
		ofRemoveListener(ofEvents().draw, this, &ofxMouseRuler::draw, OF_EVENT_ORDER_AFTER_APP + 1);
	}
}

void ofxMouseRuler::setup() {
	visible = true;
	ofRegisterMouseEvents(this);
	ofAddListener(ofEvents().draw, this, &ofxMouseRuler::draw, std::numeric_limits<int>::max());
	setupDone = true;
}

void ofxMouseRuler::setOrig(int x, int y) {
	orig_x = x;
	orig_y = y;
}

void ofxMouseRuler::draw(ofEventArgs&) {

	if (visible) {

		ofPushStyle();

		//ofPoint mPos(ofGetMouseX(), ofGetMouseY());
		ofPoint mPos(ofGetMouseX() - orig_x, ofGetMouseY() - orig_y);

		//draw awis
		if (bShowAvis)
		{
			ofSetColor(ofColor(ofColor::white), 64);
			ofSetLineWidth(1);
			ofDrawLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
			ofDrawLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
		}

		std::string info;
		//if(!dragging)
		{
			info += "x:" + ofToString(mPos.x, 0) + " y:" + ofToString(mPos.y, 0);
		}
		
		// distance line
		if (dragging)
		{
			info += "\n";

			//blink
			int _fn = ofGetFrameNum();
			int _sp = 10;
			bool _b = (_fn % _sp) > _sp / 2;
			if (_b) ofSetColor(255, 0, 0, 255);
			else ofSetColor(255, 0, 0, 128);
			ofSetLineWidth(1);

			//ofDrawLine(mPos, dragStart);
			ofDrawLine(mPos + ofPoint(orig_x, orig_y), dragStart + ofPoint(orig_x, orig_y));

			info += ofToString((mPos - dragStart).length(), 0);
		}

		bTheme ? ofSetColor(255) : ofSetColor(0);
		ofFill();

		int _pad_y = pad_y;
		int _pad_x = pad_x;

		static ofBitmapFont f;
		auto r = f.getBoundingBox(info, 0, 0);

		if (mPos.x > ofGetWidth() * 0.8) _pad_x = -pad_x - r.getWidth();
		if (mPos.y > ofGetHeight() * 0.8) _pad_y = -pad_y + 8;


		ofRectangle rbb{ font.getBoundingBox(info, mPos.x + _pad_x, mPos.y + _pad_y) };
		rbb.scaleFromCenter(1.2);

		ofDrawRectangle(rbb);
		//ofDrawRectRounded(rbb, 3);

		bTheme ? ofSetColor(0) : ofSetColor(255);
		ofDrawBitmapString(info, mPos.x + _pad_x, mPos.y + _pad_y);

		ofPopStyle();
	}

}

void ofxMouseRuler::show() {
	visible = true;
}

void ofxMouseRuler::hide() {
	visible = false;
}

void ofxMouseRuler::toggleVisibility() {
	visible = !visible;
}

void ofxMouseRuler::setVisibility(bool b) {
	visible = b;
}


bool ofxMouseRuler::mousePressed(ofMouseEventArgs& args) {
	//	dragStart = ofPoint(args.x,args.y);
	dragStart = ofPoint(args.x - orig_x, args.y - orig_y);

	dragging = true;
	return false;
}

bool ofxMouseRuler::mouseReleased(ofMouseEventArgs& args) {
	dragging = false;
	return false;
}
