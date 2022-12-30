#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//ofSetWindowPosition(-1900, 50);

	pIt.setup(index);

	//--

	// Init matrix colors and char/labels
	{
		colors.clear();
		for (size_t i = 0; i < 9; i++)
		{
			ofColor c;
			if (i < 3) c = ofColor::green;
			else if (i < 6) c = ofColor::yellow;
			else if (i < 10) c = ofColor::red;

			colors.push_back(c);
		}
		keyCommandsChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ui.Begin();
	{
		if (ui.BeginWindow("ofApp")) {

			ui.Add(ui.bMinimize, OFX_IM_TOGGLE_ROUNDED_SMALL);
			if (!ui.bMinimize) ui.Add(ui.bAutoResize, OFX_IM_TOGGLE_ROUNDED_SMALL);
			ui.AddSpacingSeparated();

			if (!ui.bMinimize) ui.AddLabelBig("The Master Param Index");
			ui.AddSpacing();

			// index
			ui.Add(index, OFX_IM_HSLIDER);
			ui.AddSpacingSeparated();

			//--

			if (!ui.bMinimize) {
				ui.AddLabel("surfingParamIndexToggles.h");
				ui.AddSpacing();
				ui.AddLabelBig("Param Index \nto Param Toggles");
			}

			// toggles
			//ui.AddGroup(pIt.params_Toggles);
			auto& g = pIt.getParamToggles();

			ui.AddGroup(g, ui.bMinimize ? SurfingGuiGroupStyle_NoHeader : SurfingGuiGroupStyle_None);

			// styles
			{
				static bool bDone = false;
				if (!bDone) {
					bDone = true;
					//ui.ClearStyles();
					ui.AddStyleGroupForBools(g, OFX_IM_TOGGLE_MEDIUM_BORDER_BLINK);
				}
			}

			ui.AddSpacingSeparated();

			//pIt.draw();

			//--

			if (!ui.bMinimize) ui.AddLabelBig("Index to \ntoggle Matrix");

			// tester

			string toolTip = "Each toggles will \ncontrol the main index " + ofToString(index.get());
			static bool bFlip = true;
			static bool bResponsiveButtonsClicker = false;
			static bool bUseColorizedMatrices = true;
			static int amountButtonsPerRowClicker = 3;
			static float h = 2 * ui.getWidgetsHeightUnit();

			if (!ui.bMinimize) {
				ImGui::SliderFloat("Height", &h, 1 * ui.getWidgetsHeightUnit(), 10 * ui.getWidgetsHeightUnit());
				ui.AddToggle("Colorized", bUseColorizedMatrices, OFX_IM_TOGGLE_ROUNDED_MINI);
				if (bUseColorizedMatrices) ui.AddToggle("Responsive", bResponsiveButtonsClicker, OFX_IM_TOGGLE_ROUNDED_MINI);
				if (!bResponsiveButtonsClicker && bUseColorizedMatrices) {
					ui.AddToggle("Flip", bFlip, OFX_IM_TOGGLE_ROUNDED_MINI);
					ImGui::SliderInt("Amount", &amountButtonsPerRowClicker, 1, index.getMax() + 1);
				}
			}

			//--

			if (!bUseColorizedMatrices)
				ofxImGuiSurfing::AddMatrixClicker(index, h);
			else
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, colors, !bResponsiveButtonsClicker, amountButtonsPerRowClicker, true, h, toolTip, bFlip);

			//--

			ui.EndWindow();
		};
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

