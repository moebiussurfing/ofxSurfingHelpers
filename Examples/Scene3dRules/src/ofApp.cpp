#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::SurfSetMyMonitor(1);
	sceneGrid.setCameraPtr(&camera);

	doResetCamera();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	sceneGrid.drawBg();

	//--

	camera.begin();
	{
		ofScale(scale.get());

		sceneGrid.scale = scale.get();//trick
		sceneGrid.drawInsideCam();
	}
	camera.end();

	sceneGrid.drawOutsideCam();

	//--

	ui.Begin();
	{
		if (ui.BeginWindow(bGui))
		{
			ui.AddLabelHuge("Scene\n3dRules");
			ui.AddSpacing();

			ui.AddGroup(sceneGrid.params);
			if (ui.AddButton("Reset Colors")) {
				sceneGrid.doResetColors();
			}
			ui.AddSpacingBigSeparated();
			if (ui.AddButton("Reset Camera")) {
				doResetCamera();
			}
			ui.Add(scale);
			ui.Add(scale, OFX_IM_STEPPER);
			ui.AddSpacingSeparated();

			bool b = camera.getMouseInputEnabled();
			if (ui.AddButton("MouseCam")) {
				if (b) camera.disableMouseInput();
				else camera.enableMouseInput();
			}
			ui.AddLabel(b ? "Enabled" : "Disabled");

			ui.EndWindow();
		}
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') bGui = !bGui;
	if (key == OF_KEY_RETURN) sceneGrid.bForceBitmap = !sceneGrid.bForceBitmap;
}