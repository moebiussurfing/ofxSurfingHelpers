#pragma once
#include "ofMain.h"

#include "ofxSurfingImGui.h"

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

*/

//--------------------------------------------------------------

class SurfingEasyCam : public ofEasyCam
{
public:
	SurfingEasyCam() {
	}

	~SurfingEasyCam() {
	}

	ofParameter<bool> bGui{ "SurfEasyCam", true };
	ofParameter<bool> bHelp{ "Help", false };
	ofParameter<bool> bExtra{ "Extra", true };

private:
	//ofEventListeners listenersEasyCamera;//TODO: group all here
	//ofParameterGroup params_EasyCamera;//TODO: group all here

	//TODO: listener and ctrl to toggle/enable workflow
	ofParameter<bool> bMouseCam{ "MouseCam", false };
	ofEventListener listenerMouseCam;

	ofParameter<bool> bInertia{ "Inertia", false };
	ofEventListener listenerIntertia;

	ofParameter<float> dragInertia{ "Drag", 0.7f, 0.2, 1.0 };
	ofEventListener listenerDrag;

	ofParameter<bool> bKeyMod{ "KeyMod", false };
	ofEventListener listenerKeyMod;

	ofParameter<bool> bKeyModMomentary{ "Momentary", false };
	ofEventListener listenerKeyModMomentary;
	// true: mouse cam enabled while key mod pressed.
	// false: mouse cam state switch when key mod press.

public:
	//------------------------------------------------------------------------------------------
	void setup() {
		ofLogNotice(__FUNCTION__);

		//--

		listenerMouseCam = bMouseCam.newListener([&](bool& b) {

			string s = "Mouse Camera " + ofToString(b ? "TRUE" : "FALSE");
			ofLogNotice("SurfingEasyCam") << s;

			//TODO:
			// fix (sometimes wrong state) with a workaround in update.
			if (b) this->enableMouseInput();
			else this->disableMouseInput();

			});

		//-

		listenerKeyMod = bKeyMod.newListener([&](bool& b) {
			static bool b_ = !bKeyMod.get();
			if (b != b_)
			{
				// changed
				b_ = b;

				if (!bKeyModMomentary) {
					if (b)
					{
						// MODE A: switch
						// toggle mouse cam if bKeyMod changed but to true
						bMouseCam = !bMouseCam.get();
					}
				}
				else {
					// MODE B: latch
					// mouse cam enabled while key mod pressed.
					bMouseCam = b;
				}

				string s = "ModKey " + ofToString(b ? "ENABLED" : "DISABLED");
				ofLogNotice("SurfingEasyCam") << s;
			}
			});

		//-

		listenerKeyModMomentary = bKeyModMomentary.newListener([&](bool& b) {
			static bool b_ = !bKeyModMomentary.get();
			if (b != b_)
			{
				// changed
				b_ = b;

				if (bKeyModMomentary) {
					bMouseCam = bKeyMod;
				}
				else {
				}
			}
			});

		//--

		listenerIntertia = bInertia.newListener([&](bool& b) {
			if (b) this->enableInertia();
			else this->disableInertia();

			this->setDrag(dragInertia);
			});

		listenerDrag = dragInertia.newListener([&](float& v) {
			this->setDrag(v);
			});

		//--

		//if (bInertia) this->enableInertia();
		//else this->disableInertia();
		//this->setDrag(dragInertia);//default is 0.9f

		//this->disableMouseInput();
		doReset();

		// refresh
		bInertia = bInertia;
		bMouseCam = bMouseCam;
	}

public:
	//------------------------------------------------------------------------------------------
	void drawImGui(SurfingGuiManager& ui)
	{
		if (bGui) IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_MINI;
		if (ui.BeginWindow(bGui))
		{
			//TODO: move inside Stage Manager?
			//TODO: add example of how to pass a f* to insert widgets from here (parent).

			//ui.AddLabelBig("EasyCamera");
			ui.Add(bMouseCam, OFX_IM_TOGGLE_BORDER_BLINK);
			if (ui.AddButton("Reset Camera")) {
				doReset();
			}

			if (ui.isMaximized()) {
				ui.AddSpacing();
				ui.Add(bExtra, OFX_IM_TOGGLE_ROUNDED_MINI);
			}

			if (ui.isMaximized() && bExtra) {
				ui.AddSpacingSeparated();

				ui.Add(bKeyModMomentary);
				ui.AddSpacing();

				ui.Add(bInertia);
				if (bInertia)
				{
					//ui.Add(dragInertia);
					//ui.Add(dragInertia, OFX_IM_STEPPER_NO_LABEL);
					ui.Add(dragInertia, OFX_IM_STEPPER);
					if (ui.AddButton("Reset Inertia")) {
						//bInertia = false;
						dragInertia = 0.7f;
					}
				}

				ui.Add(bHelp, OFX_IM_TOGGLE_ROUNDED_MINI);

				/*
				string s;
				//TODO: BUG: there's a bug on isMouseOverGui();
				// that fails when over some widgets!
				ui.AddSpacing();
				bool b = ui.isMouseOverGui();
				//s = "MouseOverGui " + string(b ? "TRUE" : "FALSE");
				s = b ? "MouseOverGui" : " ";
				ui.AddLabel(s);

				s = bKeyMod ? "KeyMod" : " ";
				ui.AddLabel(s);
				*/
			}

			ui.EndWindow();
		}
	}

	//------------------------------------------------------------------------------------------
	void doReset()
	{
		ofLogNotice(__FUNCTION__);

		this->setupPerspective();
		this->lookAt(glm::vec3(0));
		//this->setPosition(0, 0, 0);

		////this->disableMouseInput();

		//this->setPosition(-300, 200, -300);
		//this->lookAt(glm::vec3(0));

		//this->setFarClip(10000);
		//this->setNearClip(0);

		////this->enableMouseInput();
		////bMouseCam = true;
	}

	//--------------------------------------------------------------
	void keyPressed(int key) {
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
		ofSetColor(255, 255, 0);
		ofNoFill();
		glDepthMask(false);
		ofDrawCircle(x, y, r);
		glDepthMask(true);
		ofPopStyle();
	}

	//--------------------------------------------------------------
	void drawHelpText() {
		if (!bHelp) return;

		static ofBitmapFont f;

		stringstream ss;
		ss << "SurfingEasyCam: " << endl << endl;
		ss << "MOUSE INPUT: " << (this->getMouseInputEnabled() ? "ENABLED" : "DISABLED") << endl;
		ss << "MOMENTARY: " << (bKeyModMomentary ? "TRUE" : "FALSE") << endl;
		ss << "MODE: " << (this->getOrtho() ? "ORTHO" : "PERSPECTIVE") << endl;
		ss << "INERTIA: " << (this->getInertiaEnabled() ? "TRUE" : "FALSE") << endl;
		ss << "ROTATION RELATIVE Y AXIS: " << (this->getRelativeYAxis() ? "TRUE" : "FALSE") << endl;
		ss << endl;
		ss << "CTRL:  MOUSE INPUT:" << endl;
		ss << "SPACE: CAMERA PROJECTION MODE" << endl;
		ss << "       (ORTHO OR PERSPECTIVE)" << endl;
		ss << "I:     CAMERA INERTIA:" << endl;
		ss << "Y:     ROTATION RELATIVE Y AXIS:" << endl;
		ss << "H:     THIS HELP:" << endl;
		ss << endl;
		ss << "Camera x,y ROTATION:" << endl;
		ss << "    LEFT MOUSE BUTTON DRAG inside circle" << endl;
		ss << endl;
		ss << "Camera z rotation or roll" << endl;
		ss << "    LEFT MOUSE BUTTON DRAG outside circle" << endl;

		ss << endl;
		ss << "Move over x,y axis / truck and boom:" << endl;
		ss << "    LEFT MOUSE BUTTON DRAG + m" << endl;
		ss << "    MIDDLE MOUSE BUTTON PRESS" << endl;
		ss << endl;
		ss << "Move over z axis / dolly / zoom in or out:" << endl;
		ss << "    RIGHT MOUSE BUTTON DRAG" << endl;
		ss << "    VERTICAL SCROLL" << endl << endl;
		if (this->getOrtho()) {
			ss << "Notice that in ortho mode zoom \nwill be centered at the mouse position.";
		}

		//ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 25);

		auto bb = f.getBoundingBox(ss.str().c_str(), 0, 0);
		ofDrawBitmapStringHighlight(ss.str().c_str(), ofGetWidth() - bb.getWidth() - 15, ofGetHeight() - bb.getHeight() - 30);
	}
};