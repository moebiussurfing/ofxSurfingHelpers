/********  Test sample for ofxInteractiveObject									********/
/********  Make sure you open your console to see all the events being output	********/

// added doubleClick by moebiusSurfing

#pragma once

#include "ofxMSAInteractiveObject.h"

// Time gaps
#define TIME_GAP_MIN 50 // Filter clicks. more faster are ignored.
#define TIME_GAP_MAX 250 // Max gap for double click. Lower is more precise but "difficult".

//--

class DoubleClicker : public ofxMSAInteractiveObject
{
private:

	// Debug colors
	
	ofColor	IDLE_COLOR;
	ofColor	OVER_COLOR;
	ofColor	DOWN_COLOR;

	ofColor	DOUBLE1_COLOR;
	ofColor	DOUBLE2_COLOR;
	ofColor	DOUBLE3_COLOR;
	ofColor	DOUBLE4_COLOR;

	int alpha = 128;

	int timerMin = TIME_GAP_MIN;
	int timerMax = TIME_GAP_MAX;

	bool bDoubleClicked = false;
	bool bTripleClicked = false;
	int timerClick = 0;

	bool bDebug = false;
	
	bool bMouseLeftPressed = false;
	bool bMouseRightPressed = false;

public:

	ofParameter<bool> bGui{ "_Show", true };//exposed toggle to be used or linked in other parent scope guis!

public:

	int clickCounter = 0;

	//--------------------------------------------------------------
	void setDebug(bool b) {
		bDebug = b;
	}

	//--------------------------------------------------------------
	void setAlpha(int a) {
		alpha = a;
	}
	//--------------------------------------------------------------
	void setColorIDLE_COLOR(ofColor c) {
		IDLE_COLOR = c;
	}
	//--------------------------------------------------------------
	void setColorOVER_COLOR(ofColor c) {
		OVER_COLOR = c;
	}
	//--------------------------------------------------------------
	void setColorDOWN_COLOR(ofColor c) {
		DOWN_COLOR = c;
	}
	//--------------------------------------------------------------
	void setColorDOUBLE1_COLOR(ofColor c) {
		DOUBLE1_COLOR = c;
	}
	//--------------------------------------------------------------
	void setColorDOUBLE2_COLOR(ofColor c) {
		DOUBLE2_COLOR = c;
	}
	//--------------------------------------------------------------
	void setColorDOUBLE3_COLOR(ofColor c) {
		DOUBLE3_COLOR = c;
	}

	//--

	//--------------------------------------------------------------
	bool isMouseDoubleClick()
	{
		if (bDoubleClicked) {
			bDoubleClicked = false;
			return true;
		}
		else return false;
	}
	//--------------------------------------------------------------
	bool isMouseTripleClick()
	{
		if (bTripleClicked) {
			bTripleClicked = false;
			return true;
		}
		else return false;
	}

	void setup()
	{
		IDLE_COLOR = ofColor(ofColor::yellow, alpha * 0.1);
		OVER_COLOR = ofColor(ofColor::yellow, alpha * 0.5);
		DOWN_COLOR = ofColor(ofColor::yellow, alpha * 0.8);

		DOUBLE1_COLOR = ofColor(ofColor::yellow, alpha);
		DOUBLE2_COLOR = ofColor(ofColor::green, alpha);
		DOUBLE3_COLOR = ofColor(ofColor::blue, alpha);
		DOUBLE4_COLOR = ofColor(ofColor::black, alpha);

		if (bDebug) ofLogVerbose(__FUNCTION__) << "hello!\n";

		enableMouseEvents();
		enableKeyEvents();
	}

	void exit()
	{
		if (bDebug) ofLogVerbose(__FUNCTION__) << "goodbye!\n";
	}

	void update()
	{
		//x = ofGetWidth()/2 + cos(ofGetElapsedTimef() * 0.2) * ofGetWidth()/4;
		//y = ofGetHeight()/2 + sin(ofGetElapsedTimef() * 0.2) * ofGetHeight()/4;
	}

	void draw()
	{
		if (bDebug)
		{
			ofPushStyle();
			if (isMousePressed()) ofSetColor(DOWN_COLOR);
			else if (isMouseOver()) ofSetColor(OVER_COLOR);
			//else ofSetColor(IDLE_COLOR);
		}

		auto t = ofGetElapsedTimeMillis();
		switch (clickCounter)
		{

		case 0: if (bDebug) ofSetColor(IDLE_COLOR); break;

		case 1:
		{
			if (bDebug) ofSetColor(DOUBLE1_COLOR);
			if (t - timerClick > timerMax) clickCounter = 0;
		}
		break;

		case 2:
		{
			if (bDebug) ofSetColor(DOUBLE2_COLOR);
			if (t - timerClick > timerMax) clickCounter = 0;
		}
		break;

		case 3:
		{
			if (bDebug) ofSetColor(DOUBLE3_COLOR);
			if (t - timerClick > timerMax) clickCounter = 4;
		}
		break;

		case 4://final wait
		{
			if (bDebug) ofSetColor(DOUBLE4_COLOR);
			clickCounter = 0;
		}
		break;

		}
		
		//--

		if (bDebug)
		{
			ofDrawRectangle(ofRectangle(x, y, width, height));
			ofPopStyle();
		}
	}

	virtual void onRollOver(int x, int y) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y;
	}

	virtual void onRollOut() {
		if (bDebug) ofLogVerbose(__FUNCTION__);
	}

	virtual void onMouseMove(int x, int y) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y;
	}

	virtual void onDragOver(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;
	}

	virtual void onDragOutside(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;
	}

	virtual void onPress(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;

		auto t = ofGetElapsedTimeMillis();
		timerClick = t;

		if (clickCounter == 0) {
			clickCounter++;
		}
		else if (clickCounter == 1) {
			if (t - timerClick > timerMin) clickCounter++;
			clickCounter++;
			bDoubleClicked = true;
		}
		else if (clickCounter == 2) {
			if (t - timerClick > timerMin) clickCounter++;
			clickCounter++;
			bTripleClicked = true;
		}
		else if (clickCounter == 3) {
			clickCounter++;
		}

		// this function could be moved to DoubleClick class
		bool bIsOver = this->inside(x, y);
		if (!bIsOver)return;

		if (button == 0) bMouseLeftPressed = true;
		if (button == 2) {
			bMouseRightPressed = true;

			if (bMouseLeftPressed) bGui = false;
		}
	}

	virtual void onRelease(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;

		if (button == 0) bMouseLeftPressed = false;
		if (button == 2) bMouseRightPressed = false;
	}

	virtual void onReleaseOutside(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;
	}

	virtual void keyPressed(int key) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << key;
	}

	virtual void keyReleased(int key) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << key;
	}

};