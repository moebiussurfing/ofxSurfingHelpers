#pragma once
#include "ofMain.h"

#include "ofxMouseRuler.h"

/*

	- Timed hide mouse cursor

	- Ruler tool to measure distances
		taken from https://github.com/frauzufall/ofxMouseRuler


	TODO

	should disable listeners if not required, 
	like when they re disabled.

	add force hide stopping timer

*/


//--

class surfingMouseHandler
{
public:
	surfingMouseHandler()
	{
		ofRegisterMouseEvents(this);
		ofAddListener(ofEvents().update, this, &surfingMouseHandler::update, std::numeric_limits<int>::max());
		ofAddListener(ofEvents().draw, this, &surfingMouseHandler::draw, std::numeric_limits<int>::max());

		setup();
	};

	~surfingMouseHandler()
	{
		ofUnregisterMouseEvents(this);
		ofRemoveListener(ofEvents().update, this, &surfingMouseHandler::update, OF_EVENT_ORDER_AFTER_APP + 1);
		ofRemoveListener(ofEvents().draw, this, &surfingMouseHandler::draw, OF_EVENT_ORDER_AFTER_APP + 1);
	};

public:
	bool mousePressed(ofMouseEventArgs& args) {
		doRefreshCursor();
		return false;
	};
	bool mouseReleased(ofMouseEventArgs& args) {
		doRefreshCursor();
		return false;
	};

	void mouseMoved(ofMouseEventArgs&) { doRefreshCursor(); }
	void mouseDragged(ofMouseEventArgs&) { doRefreshCursor(); }

	void mouseScrolled(ofMouseEventArgs&) { /*doRefreshCursor();*/ }
	void mouseEntered(ofMouseEventArgs&) { /*doRefreshCursor();*/ }
	void mouseExited(ofMouseEventArgs&) { /*doRefreshCursor();*/ }

private:
	void setup()
	{
		// hidden mouse
		lastmove = ofGetElapsedTimeMillis();
		ofShowCursor();
		bHiddenMouse = false;

		// ruler
		mouseRuler.setup();

		//TODO:
		//ee = bEnabled.newListener([this] {
		//	{
		//		lastmove = ofGetElapsedTimeMillis();
		//		ofShowCursor();
		//		//if(bEnabled) ofShowCursor();
		//		//else ofShowCursor();
		//	}
		//	});

		//eh = bHiddenMouse.newListener([this] {
		//	{
		//		if(bHiddenMouse) mouseRuler.setVisibility(false);
		//		else mouseRuler.setVisibility(true);
		//	}
		//	});
	};

public:
	ofParameter<bool> bEnabled{ "Mouse AutoHide", true };
	void setEnableAutoHide(bool b) { bEnabled = b; }
	void setToogleAutoHide() { bEnabled = !bEnabled; }

	bool isEnabledRuler() { return mouseRuler.isVisible(); }
	bool isEnabledAutoHide() { return bEnabled.get(); }

private:
	//ofEventListener ee;

	ofxMouseRuler mouseRuler;

	// auto hide mouse
	uint64_t lastmove;
	int timerHideMouse = 5000;

	ofParameter<bool> bHiddenMouse{ "Mouse hidden", false };
	//ofEventListener eh;
	//bool bHiddenMouse = false;

	// easy callback
	bool bMustUptate = false;

public:
	void setTimerHidder(int t) {
		timerHideMouse = t;
	};

	bool isTimedOut()
	{
		if (bMustUptate) {
			bMustUptate = false;
			return true;
		}
		else return false;
	};

private:
	void draw(ofEventArgs&)
	{
		//cout << "draw:" <<  ofGetFrameNum() << endl;
	};

	void update(ofEventArgs&)
	{
		if (!bEnabled) return;

		// hidden mouse
		auto t = ofGetElapsedTimeMillis() - lastmove;
		//cout << "elapsed:" << t << endl;

		if (t > timerHideMouse)
		{
			bHiddenMouse = true;
			ofHideCursor();
		}
	};

	// hidden mouse
	void doRefreshCursor()
	{
		lastmove = ofGetElapsedTimeMillis();
		if (bHiddenMouse) {
			ofShowCursor();
			bHiddenMouse = false;
		}
	};

public:
	// ruler
	void setToggleVisibleRuler()
	{
		mouseRuler.toggleVisibility();
	};

	void setToggleVisibleRuler(bool b)
	{
		mouseRuler.toggleVisibility();
	};

};

