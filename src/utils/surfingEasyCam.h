#pragma once
#include "ofMain.h"

//#include "ofxSurfingImGui.h"

/*

	Extends ofEasyCam with:

	- Implements a mouse control workflow.
		(Ctrl key)
	- Momentary modes
		True: toggles mouse control state.
		False: enables mouse control state momentary.
	- Inertia and drag settings and config.
	- Help info and key commands.
	- ImGui required but it could be removed easily.

	TODO
	- fix orto mouse behavior
	- add near/far clip params
	- add fov etc
	- add presets or two cams mode, animated tweening

	// fix setControlArea
	void ofApp::windowResized(int w, int h){
//	ofLogNotice("ofApp :: windowResized : ") << w << " x " << h;
	mViewRect = ofRectangle( 0, 0, std::min(ofGetWidth()-320, 900), ofGetHeight());
	mTextRect = ofRectangle( mViewRect.getRight(), 0, ofGetWidth() - mViewRect.getWidth(), ofGetHeight() );
	
	cam.setControlArea(mViewRect);
*/

//--------------------------------------------------------------

class SurfingEasyCam : public ofEasyCam {
public:
	SurfingEasyCam() {
		ofAddListener(ofEvents().update, this, &SurfingEasyCam::update);
	}

	~SurfingEasyCam() {
		ofRemoveListener(ofEvents().update, this, &SurfingEasyCam::update);
	}

	ofParameter<bool> bGui { "Camera", true };
	ofParameter<bool> bHelp { "Help", false };
	ofParameter<bool> bExtra { "Extra", true };
	ofParameter<bool> bKeys { "Keys", true };

	string sHelp;

	//TODO: listener and ctrl to toggle/enable workflow
	ofParameter<bool> bMouseCam { "MouseCam", false };
	ofEventListener listenerMouseCam;

	ofParameter<bool> bInertia { "Inertia", false };
	ofEventListener listenerIntertia;

	ofParameter<bool> bOrtho { "Ortho", false };
	ofEventListener listenerOrtho;

	ofParameter<float> dragInertia { "Drag", 0.7f, 0.2, 1.0 };
	ofEventListener listenerDrag;

	//ofEventListeners listenersEasyCamera;//TODO: group all here
	//ofParameterGroup paramsEasyCamera;//TODO: group all here

private:
	bool bFlagBuildHelp = false;

protected:
	ofParameter<bool> bKeyMod { "KeyMod", false };
	ofEventListener listenerKeyMod;

	ofParameter<bool> bKeyModMomentary { "Momentary", false };
	ofEventListener listenerKeyModMomentary;
	// true: mouse cam enabled while key mod pressed.
	// false: mouse cam state switch when key mod press.

	bool bDoneSetup = false;

public:
	//------------------------------------------------------------------------------------------
	void setup() {
		ofLogNotice(__FUNCTION__);

		//--

		listenerMouseCam = bMouseCam.newListener([&](bool & b) {
			string s = "Mouse Camera " + ofToString(b ? "True" : "False");
			ofLogNotice("SurfingEasyCam") << s;

			//TODO:
			// fix (sometimes wrong state) with a workaround in update.
			if (b)
				this->enableMouseInput();
			else
				this->disableMouseInput();
			bFlagBuildHelp = true;
		});

		//-

		listenerKeyMod = bKeyMod.newListener([&](bool & b) {
			static bool b_ = !bKeyMod.get();
			if (b != b_) {
				// changed
				b_ = b;

				if (!bKeyModMomentary) {
					if (b) {
						// MODE A: switch
						// toggle mouse cam if bKeyMod changed but to true
						bMouseCam = !bMouseCam.get();
					}
				} else {
					// MODE B: latch
					// mouse cam enabled while key mod pressed.
					bMouseCam = b;
				}

				string s = "ModKey " + ofToString(b ? "ENABLED" : "DISABLED");
				ofLogNotice("SurfingEasyCam") << s;
				bFlagBuildHelp = true;
			}
		});

		//-

		listenerKeyModMomentary = bKeyModMomentary.newListener([&](bool & b) {
			static bool b_ = !bKeyModMomentary.get();
			if (b != b_) {
				// changed
				b_ = b;

				if (bKeyModMomentary) {
					bMouseCam = bKeyMod;
				} else {
				}
				bFlagBuildHelp = true;
			}
		});

		//--

		listenerIntertia = bInertia.newListener([&](bool & b) {
			if (b)
				this->enableInertia();
			else
				this->disableInertia();

			this->setDrag(dragInertia);
			bFlagBuildHelp = true;
		});

		listenerDrag = dragInertia.newListener([&](float & v) {
			this->setDrag(v);
			bFlagBuildHelp = true;
		});

		listenerOrtho = bOrtho.newListener([&](bool & b) {
			if (b)
				this->enableOrtho();
			else
				this->disableOrtho();
			bFlagBuildHelp = true;
		});

		//--

		//if (bInertia) this->enableInertia();
		//else this->disableInertia();
		//this->setDrag(dragInertia);//default is 0.9f

		//this->disableMouseInput();
		doReset();

		// refresh
		bInertia.set(bInertia.get());
		bMouseCam.set(bMouseCam.get());
		bOrtho.set(bOrtho.get());

		bFlagBuildHelp = true;

		bDoneSetup = true;
	}

	void update(ofEventArgs & args) {
		if (!bDoneSetup) {
			setup();
		}

		if (bFlagBuildHelp) {
			bFlagBuildHelp = false;
			buildHelp();
		}
	}

public:
	//------------------------------------------------------------------------------------------
	void doReset() {
		ofLogNotice(__FUNCTION__);

		this->setupPerspective();
		this->lookAt(glm::vec3(0));

		//this->setFarClip(10000);
		//this->setNearClip(0);
	}

	//--------------------------------------------------------------
	void keyPressed(int key) {
		if (!bKeys) return;

		switch (key) {
		case OF_KEY_LEFT_CONTROL:
			bKeyMod = true;
			break;
		//case 'C':
		//case 'c':
		//	this->getMouseInputEnabled() ? this->disableMouseInput() : this->enableMouseInput();
		//	break;
		case ' ':
			this->getOrtho() ? this->disableOrtho() : this->enableOrtho();
			break;
		case 'H':
		case 'h':
			bHelp ^= true;
			break;
		case 'I':
		case 'i':
			this->getInertiaEnabled() ? this->disableInertia() : this->enableInertia();
			break;
		case 'Y':
		case 'y':
			this->setRelativeYAxis(!this->getRelativeYAxis());
			break;
		}
	}

	//--------------------------------------------------------------
	void keyReleased(int key) {
		if (!bKeys) return;
		switch (key) {
		case OF_KEY_LEFT_CONTROL:
			bKeyMod = false;
			break;
		}
	}

	//--------------------------------------------------------------
	void drawInteractionArea() {
		ofRectangle vp = ofGetCurrentViewport();
		float r = std::min<float>(vp.width, vp.height) * 0.5f;
		float x = vp.width * 0.5f;
		float y = vp.height * 0.5f;

		ofPushStyle();
		ofSetLineWidth(3);
		ofSetColor(0, 0, 0, 128);
		//ofSetColor(255, 255, 0);
		ofNoFill();
		glDepthMask(false);
		ofDrawCircle(x, y, r);
		glDepthMask(true);
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void buildHelp() {
		ofLogNotice(__FUNCTION__);

		stringstream ss;
		ss << "EasyCam" << endl
		   << endl;
		ss << "H:           Help" << endl;
		ss << "Mouse Input: " << (this->getMouseInputEnabled() ? "Enabled" : "Disabled") << endl;
		ss << "Momentary:   " << (bKeyModMomentary ? "True" : "False") << endl;
		ss << "Mode:        " << (this->getOrtho() ? "Ortho" : "Perspective") << endl;
		ss << "Inertia:     " << (this->getInertiaEnabled() ? "True" : "False") << endl;
		ss << "Rotation Relative y Axis: " << (this->getRelativeYAxis() ? "True" : "False") << endl;
		ss << endl;
		ss << "CTRL:  Mouse Input:" << endl;
		ss << "SPACE: Camera Projection Mode" << endl;
		ss << "       " << (bOrtho ? "Ortho" : "Perspective") << endl;
		ss << "I:     Camera Inertia:" << endl;
		ss << "Y:     Rotation Relative y Axis:" << endl;
		ss << endl;
		ss << "Camera x,y Rotation:" << endl;
		ss << "    Left Mouse Button Drag inside circle" << endl;
		ss << endl;
		ss << "Camera z rotation or roll" << endl;
		ss << "    Left Mouse Button Drag outside circle" << endl;
		ss << endl;
		ss << "Move over x,y axis / truck and boom:" << endl;
		ss << "    Left Mouse Button Drag + m" << endl;
		ss << "    Middle Mouse Button Press" << endl;
		ss << endl;
		ss << "Move over z axis / dolly / zoom in or out:" << endl;
		ss << "    Right Mouse Button Drag" << endl;
		ss << "    Vertical Scroll" << endl;
		if (bOrtho) {
			//ss << endl;
			ss << "Notice that in ortho mode zoom \nwill be centered at the mouse position.";
		}

		sHelp = ss.str();
	}

	//--------------------------------------------------------------
	void drawHelpText() {
		if (!bHelp) return;
#if 0
		ofDrawBitmapStringHighlight(sHelp, 20, 25);
#else
		static ofBitmapFont f;
		auto bb = f.getBoundingBox(sHelp, 0, 0);
		ofDrawBitmapStringHighlight(sHelp, ofGetWidth() - bb.getWidth() - 15, ofGetHeight() - bb.getHeight() - 30);
#endif
	}
};
