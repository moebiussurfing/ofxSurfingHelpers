/********  Test sample for ofxInteractiveObject									********/
/********  Make sure you open your console to see all the events being output	********/

// added double, and triple Click and some combination workflow by moebiusSurfing

/*

	TODO:
	BUG:
	onReleaseOutside is not called,
	then clicked left/right buttons hangs if release out of the box.

*/


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
	ofColor	RIGHT_COLOR;

	int alpha = 128;

	int timerMin = TIME_GAP_MIN;
	int timerMax = TIME_GAP_MAX;

	bool bDoubleClicked = false;
	bool bTripleClicked = false;
	uint64_t timerClick = 0;

	bool bDebug = false;

	bool bMouseLeftPressed = false;//it's maintained pressed
	bool bMouseRightPressed = false;//it's maintained pressed
	bool bMouseRightJustPressed = false;//to get when right click is trigged

public:

	// Exposed toggle to be used or linked in other parent scope GUIs!
	ofParameter<bool> bGui{ "_Show", true };

public:

	int clickCounter = 0;

	//--------------------------------------------------------------
	void setDebug(bool b) {
		bDebug = b;
	}
	//--------------------------------------------------------------
	bool isDebug() {
		return bDebug;
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

	// combinations and commands

	//--------------------------------------------------------------
	bool isMouseDoubleClick()
	{
		if (bDoubleClicked) {
			bDoubleClicked = false;
			ofLogNotice(__FUNCTION__);
			return true;
		}
		else return false;
	}
	//--------------------------------------------------------------
	bool isMouseTripleClick()
	{
		if (bTripleClicked) {
			bTripleClicked = false;
			ofLogNotice(__FUNCTION__);
			return true;
		}
		else return false;
	}
	//--------------------------------------------------------------
	bool isMouseRightClick()
	{
		if (bMouseRightJustPressed) {
			bMouseRightJustPressed = false;
			ofLogNotice(__FUNCTION__);

			//ignore if left pressed
			if (bMouseLeftPressed) return false;

			return true;
		}
		else return false;
	}

	//--------------------------------------------------------------
	void setup()
	{
		IDLE_COLOR = ofColor(ofColor::yellow, alpha * 0.1);
		OVER_COLOR = ofColor(ofColor::yellow, alpha * 0.5);
		DOWN_COLOR = ofColor(ofColor::yellow, alpha * 0.8);

		DOUBLE1_COLOR = ofColor(ofColor::yellow, alpha);
		DOUBLE2_COLOR = ofColor(ofColor::green, alpha);
		DOUBLE3_COLOR = ofColor(ofColor::blue, alpha);
		DOUBLE4_COLOR = ofColor(ofColor::black, alpha);
		RIGHT_COLOR = ofColor(ofColor::orange, alpha);

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
		auto t = ofGetElapsedTimeMillis();

		switch (clickCounter)
		{
		case 0: break;
		case 1: if (t - timerClick > timerMax) clickCounter = 0; break;
		case 2: if (t - timerClick > timerMax) clickCounter = 0; break;
		case 3: if (t - timerClick > timerMax) clickCounter = 4; break;
		case 4: clickCounter = 0; break;
		}

		//--

		//TODO:
		// fix
		// workaround
		// MSAInteractiveObject do not trigs onReleaseOutside..
		// so we set to off after some time,
		// the time we are using for double clicks waiting timers.
		{
			int timeFrame = ofGetLastFrameTime() * 1000;//ms
			int framesRelease = TIME_GAP_MAX / MAX(timeFrame, 1);

			static int c1 = 0;
			if (bMouseLeftPressed)
			{
				c1++;
				if (c1 >= framesRelease) {
					bMouseLeftPressed = false;
					c1 = 0;
				}
			}

			static int c2 = 0;
			if (bMouseRightPressed)
			{
				c2++;
				if (c2 >= framesRelease) {
					bMouseRightPressed = false;
					c2 = 0;
				}
			}
		}
	}

	void draw()
	{
		// debug

		if (bDebug)
		{
			ofPushStyle();

			if (isMousePressed()) ofSetColor(DOWN_COLOR);
			else if (isMouseOver()) ofSetColor(OVER_COLOR);

			ofLogNotice(__FUNCTION__);
			ofLogNotice(__FUNCTION__) << "bMouseLeftPressed:" << bMouseLeftPressed;
			ofLogNotice(__FUNCTION__) << "bMouseRightPressed:" << bMouseRightPressed;
			if (bMouseRightJustPressed) ofLogNotice(__FUNCTION__) << "bMouseRightJustPressed:" << bMouseRightJustPressed;

			//--

			switch (clickCounter)
			{
			case 0: ofSetColor(IDLE_COLOR); break;
			case 1: ofSetColor(DOUBLE1_COLOR); break;
			case 2: ofSetColor(DOUBLE2_COLOR); break;
			case 3: ofSetColor(DOUBLE3_COLOR); break;
			case 4: ofSetColor(DOUBLE4_COLOR); break;
			}

			if (bMouseRightPressed) ofSetColor(RIGHT_COLOR);

			//--

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

		//--

		// left click
		if (button == 0)
		{
			auto t = ofGetElapsedTimeMillis();
			timerClick = t;

			// combinations and extra workflow
			// left click
			bMouseLeftPressed = true;

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

			//// this function could be moved to DoubleClick class
			//bool bIsOver = this->inside(x, y);
			//if (!bIsOver) return; // discard and return if not inside box!
		}

		// right click
		else if (button == 2)
		{
			bMouseRightPressed = true;
			bMouseRightJustPressed = true;//to get when right click is trigged

			// combination to hide
			// left click + right click
			if (bMouseLeftPressed) bGui = false;
		}
	}

	virtual void onRelease(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;

		if (button == 0) bMouseLeftPressed = false;
		if (button == 2) bMouseRightPressed = false;
	}

	//TODO: BUG:
	// called when mouse releases outside of object after being pressed on object
	virtual void onReleaseOutside(int x, int y, int button) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << x << y << button;

		//TODO:
		// fix
		// workaround on update()
		// MSAInteractiveObject do not trigs onReleaseOutside..
		// so we set to off after some time,
		// the time we are using for double clicks waiting timers.
		bMouseLeftPressed = false;
		bMouseRightPressed = false;
		//if (button == 0) bMouseLeftPressed = false;
		//if (button == 2) bMouseRightPressed = false;
	}

	virtual void keyPressed(int key) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << key;
	}

	virtual void keyReleased(int key) {
		if (bDebug) ofLogVerbose(__FUNCTION__) << key;
	}

};