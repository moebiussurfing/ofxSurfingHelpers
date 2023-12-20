#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 0
	ofSetWindowPosition(-1900, 50);
	ofSetEscapeQuitsApp(false); // disable ESC to allow text input workflow
#endif

	pm.setUiPtr(&ui);
	pm.setPath(path_GLOBAL);
	pm.AddGroup(sc.params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(sc.amount % 2 == 0 ? sc.c1 : sc.c2);

	sc.draw();

	ui.Begin();
	{
		if (ui.BeginWindow("ofApp")) {

			ui.Add(ui.bMinimize, OFX_IM_TOGGLE_ROUNDED_SMALL);
			ui.Add(pm.bFloating, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui.AddSpacingSeparated();

			ui.AddLabelBig("Preset \nParameters", true, true);
			ui.AddGroup(sc.params);

			// A.
			// Integrated inside another panel
			if (!pm.bFloating) {
				ui.AddSpacingBigSeparated();
				pm.drawImGui();
			}

			// B.
			// A minimal clicker only version
			//pm.drawImGuiClicker();

			ui.EndWindow();
		};

		// C.
		// Separated on another floating window
		if (pm.bFloating) pm.drawImGui(true);
	}
	ui.End();
}
