#pragma once
#include "ofMain.h"

#include "SurfingEasyCam.h"
#include "ofxSurfingImGui.h"

//--------------------------------------------------------------

class SurfingEasyCamImGui : public SurfingEasyCam {

public:
	//------------------------------------------------------------------------------------------
	void drawImGui(SurfingGuiManager & ui) {
		if (bGui) IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;

		if (ui.BeginWindow(bGui)) {
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
				//ui.AddSpacingSeparated();
				ui.AddSpacing();

				ui.Add(bKeyModMomentary);
				//ui.AddSpacing();

				ui.Add(bInertia);
				if (bInertia) {
					//ui.Add(dragInertia);
					//ui.Add(dragInertia, OFX_IM_STEPPER_NO_LABEL);
					ui.AddLabel(dragInertia.getName());
					ui.Add(dragInertia, OFX_IM_STEPPER_NO_LABEL);
					if (ui.AddButton("Reset Inertia")) {
						//bInertia = false;
						dragInertia = 0.7f;
					}
				}

				ui.Add(bOrtho);
				ui.AddSpacing();
				ui.Add(bKeys, OFX_IM_TOGGLE_ROUNDED_MINI);
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
};