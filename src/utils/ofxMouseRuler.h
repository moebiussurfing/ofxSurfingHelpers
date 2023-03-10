#pragma once
#include "ofMain.h"

// Taken from https ://github.com/frauzufall/ofxMouseRuler
// modified by @moebiusSurfing

class ofxMouseRuler {

public:
	ofxMouseRuler();
	~ofxMouseRuler();

	void setup();
	void draw(ofEventArgs&);

	void hide();
	void show();
	void toggleVisibility();
	void setVisibility(bool b);

	bool mousePressed(ofMouseEventArgs& args);
	bool mouseReleased(ofMouseEventArgs& args);
	void mouseMoved(ofMouseEventArgs&){}
	void mouseDragged(ofMouseEventArgs&){}
	void mouseScrolled(ofMouseEventArgs&){}
	void mouseEntered(ofMouseEventArgs&){}
	void mouseExited(ofMouseEventArgs&){}

	void setOrig(int x, int y);

    void setAxisVisible(bool b)
    {
        bShowAvis = b;
    }

	ofParameter<bool> bEnabled{ "Mouse Ruler", true };
	bool isVisible() { return visible; }

private:
	bool setupDone;
	bool visible;
	ofPoint dragStart;
	bool dragging;
	ofBitmapFont font;

	// coordinate axis
	int orig_x = 0;
	int orig_y = 0;

	// pad text info
	int pad_x = 30;
	int pad_y = 30;

    bool bShowAvis = true;

	bool bTheme = false;

};

