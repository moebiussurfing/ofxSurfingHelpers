#pragma once
#include "ofMain.h"

// -> Could be completely disabled or replaced by ofxGui. Just using the ofParams!
#define USE_IM_GUI__SCENE
#ifdef USE_IM_GUI__SCENE
#include "ofxSurfingImGui.h"
#endif

/*

	TODO:

*/

//--

#include "surfingDrawPlanes.h"

//----

// Helper class
// Easily add to an ofApp.

#define SCENE_GRID_SMALL_FONT_RATIO 0.6f

namespace ofxSurfingHelpers
{
	class SurfSceneGrids
	{
	public:
		SurfSceneGrids() {
			//setup();
		};

		~SurfSceneGrids() {
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Destructor() Save settings";
			doSave();
		};

		void setCameraPtr(ofCamera* _camera) { camera = _camera; };

	private:
		ofCamera* camera = nullptr;

		string pathGlobal = "";

		string fsuffix = "_Settings.json";
		//string fsuffix = ".json";

		bool bDoneSetup = false;

	public:
		void setPathGlobal(string path) {
			pathGlobal = path;

			if (!bDoneSetup) setup();
		}

		ofParameterGroup params{ "SceneGrids" };

		ofParameter<bool> bGui{ "FloorGrid", true };

		ofParameter<ofColor> cText{ "C Text", ofColor(250), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBig{ "C Big", ofColor(ofColor::blue, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cQuarter{ "C Quarter", ofColor(ofColor::orange, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cSixteenth{ "C Sixteenth", ofColor(ofColor::yellow, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cUnits{ "C Units", ofColor(128, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBg1{ "C Bg1", ofColor(128, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBg2{ "C Bg2", ofColor(128, 128), ofColor(0), ofColor(255,255) };

		ofParameter<bool> bBig{ "Big", true };
		ofParameter<bool> bQuarter{ "Quarter", true };
		ofParameter<bool> bSixteenth{ "Sixteenth", true };
		ofParameter<bool> bUnits{ "Units", true };
		ofParameter<bool> bAxis{ "Axis", true };

		ofParameter<bool> bEnable{ "Draw FloorGrid", true };

		ofParameter<bool> bDefaultColors{ "Default Colors", true };
		ofParameter<bool> bForceBitmap{ "Force ofBitmapFont", false };
		ofParameter<bool> bFlipBg{ "Flip Bg", false };
		ofParameter<bool> bEnableBg{ "EnableBg", true };
		ofParameter<bool> bLabels{ "Labels", true };
		ofParameter<bool> bFontSmall{ "Small Font", false };
		ofParameter<bool> bOffsetLabels{ "Offset Labels", false };
		ofParameter<int> gradientType{ "Gradient Type", 2, 0, 3 };

	public:

		// Grid sizes
		float stepSize = 10;//exposed to user
		float numberOfSteps = 10;//exposed to user
		float gridSize = 100;//for debug only

		float scale = 1;//workaround trick to avoid transform errors..

		//--

		void setup()
		{
			params.add(bGui);
			params.add(bEnable);
			params.add(bForceBitmap);
			params.add(bLabels);
			params.add(bFontSmall);
			params.add(bOffsetLabels);
			params.add(bDefaultColors);
			params.add(cText);
			params.add(cBig);
			params.add(cQuarter);
			params.add(cSixteenth);
			params.add(cUnits);

			////TODO: allow disable some lines.
			// requires modifying ofxSurfingHelpers::ofxDrawFloor 
			params.add(bBig);
			params.add(bQuarter);
			params.add(bSixteenth);
			params.add(bUnits);

			params.add(bAxis);

			params.add(bEnableBg);
			params.add(gradientType);
			params.add(cBg1);
			params.add(cBg2);
			params.add(bFlipBg);

			float sz = 10;
			string _FONT_FILES_PATH = "assets/fonts/";
			string path = _FONT_FILES_PATH + string("JetBrainsMonoNL-SemiBold.ttf");
			//string path = _FONT_FILES_PATH + string(FONT_FILE_SMALL);
			//string path = _FONT_FILES_PATH + string("JetBrainsMonoNL-ExtraBold.ttf");
			bool b = font.load(path, sz, true);
			if (!b) font.load(OF_TTF_MONO, sz, true);
			sz *= SCENE_GRID_SMALL_FONT_RATIO;
			b = fontSmall.load(path, sz, true);
			if (!b) fontSmall.load(OF_TTF_MONO, sz, true);

			doResetAll();

			doLoad();

			bDoneSetup = true;
		};

		void drawBg()
		{
			if (!bDoneSetup) setup();

			if (!bEnable) return;
			if (!bEnableBg) return;

			// Bg one color
			if (gradientType == 0) {
				ofClear((bFlipBg ? cBg1 : cBg2));
			}
			else {
				// Bg gradient
				ofxSurfingHelpers::ofxDrawBgGradient((bFlipBg ? cBg1 : cBg2), (bFlipBg ? cBg2 : cBg1), ofGradientMode(gradientType.get() - 1));
			}
		};

		void drawOutsideCam()//draw labels
		{
			if (!bDoneSetup) setup();

			if (!bEnable) return;
			if (!bLabels) return;
			if (bForceBitmap) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

			//workaround 
			// Draw labels, out of the camera
			// custom font
			//if (!bForceBitmap)
			{
				ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

				if (bDefaultColors) ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale);
				else ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale, cText);
			}
		};

		void drawInsideCam() {
			if (!bDoneSetup) setup();

			if (!bEnable) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

			gridSize = numberOfSteps * stepSize;

			// Axis
			if (bAxis)
			{
				float sza = gridSize / 20.f;
				ofDrawAxis(sza);
			}

			//--

			// floor lines: 

			// 4. Units 
			// (with labels when ofBitmapFont)
			if (bUnits)
			{
				if (!bForceBitmap)
				{
					ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

					// custom font
					if (bDefaultColors)
						ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera, bUnits);
					else
						ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera, bUnits, cUnits, cText);
				}
				else
				{
					// bitmap font
					if (bDefaultColors)
						ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, bLabels, false, true, false, bUnits);
					else
						ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, bLabels, false, true, false, bUnits, cUnits, cText);
				}
			}

			// 2 Quarters
			// 3 Sixteenth
			// (without labels)
			if (bQuarter || bSixteenth)
			{
				if (bDefaultColors)
					ofxSurfingHelpers::ofxDrawFloor(gridSize, bQuarter, bSixteenth);
				else
					ofxSurfingHelpers::ofxDrawFloor(gridSize, bQuarter, bSixteenth, cQuarter.get(), cSixteenth.get());
			}

			// 1. Big
			// (outer square)
			if (bBig) {
				ofSetColor(bDefaultColors ? SURFING_RULES_COLOR_LINES_BIG : cBig);
				ofxSurfingHelpers::ofxDrawFloorRectangle(gridSize);
			}
		};

		//--

		void doSave() {
			string path;
			if (pathGlobal == "") path = params.getName() + fsuffix;
			else path = pathGlobal + "/" + params.getName() + fsuffix;
			ofxSurfingHelpers::saveGroup(params, path);

			ofLogNotice("of xSurfingHelpers::SurfSceneGrids") << "Setup() Save settings: " << path;
		};

		void doLoad() {
			string path;
			if (pathGlobal == "") path = params.getName() + fsuffix;
			else path = pathGlobal + "/" + params.getName() + fsuffix;
			ofxSurfingHelpers::loadGroup(params, path);

			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Setup() Load settings: " << path;
		};

		void doResetSettings() {
			bEnable = true;
			bEnableBg = true;
			bFlipBg = false;
			bLabels = true;
			bFontSmall = true;
			bOffsetLabels = false;
		};

		void doResetColors() {
#if(DEBUG_COLORS__SCENE)
			cText = SURFING_RULES_COLOR_LABELS;
			cBig = SURFING_RULES_COLOR_LINES_BIG;
			cQuarter = SURFING_RULES_COLOR_LINES_QUARTER;
			cSixteenth = SURFING_RULES_COLOR_LINES_SIXTEENTH;
			cUnits = SURFING_RULES_COLOR_LINES_UNITS;
			cBg1 = SURFING_RULES_COLOR_BG_1;
			cBg2 = SURFING_RULES_COLOR_BG_2;
#else
			cText = ofColor{ 255, 255, 255, 200 };
			cBig = ofColor{ 96, 96, 96, 150 };
			cQuarter = ofColor{ 64, 64, 64, 150 };
			cSixteenth = ofColor{ 64, 64, 64, 150 };
			cUnits = ofColor{ 96, 96, 96, 24 };
			cBg1 = ofColor{ 40, 40, 40 };
			cBg2 = ofColor{ 0, 0, 0 };
#endif

		};

		void doResetAll() {
			doResetSettings();
			doResetColors();
		};

	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;

		//--

#ifdef USE_IM_GUI__SCENE
	public:

		void drawImGuiDebug(ofxSurfingGui& ui)
		{
			if (!bDoneSetup) setup();

			if (ui.isMinimized()) IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

			if (ui.BeginWindow(bGui))
			{
				ui.AddMinimizerToggle();
				ui.AddSpacingSeparated();

				//--

				ui.AddLabelBig("Floor", false, true);

				ui.Add(bEnable, OFX_IM_TOGGLE_BIG);

				if (bEnable)
				{
					ui.Indent();

					ui.Add(bLabels, OFX_IM_TOGGLE);
					ui.Add(bEnableBg, OFX_IM_TOGGLE);
					ui.AddSpacing();

					if (ui.isMaximized())
					{
						ui.Add(bDefaultColors);
						ui.Add(bForceBitmap);
						ui.AddSpacingSeparated();
					}

					ui.Unindent();
				}
				//ui.AddSpacing();

				//--

				if (ui.isMaximized() && bEnable)
				{
					if (!bForceBitmap) {
						if (ui.BeginTree("Labels")) {
							ui.Add(bFontSmall);
							ui.Add(bOffsetLabels);
							if (!bForceBitmap) ui.Add(cText);
							ui.EndTree();
						}
					}
					if (!bDefaultColors)
					{
						if (ui.BeginTree("Lines")) {

							ui.Add(bAxis);

							//ui.Add(cBig);
							//ui.Add(cQuarter);
							//ui.Add(cSixteenth);
							//ui.Add(cUnits);

							////TODO: allow disable some lines. 
							//requires modifying ofxSurfingHelpers::ofxDrawFloor() ! 
							ui.Add(bBig, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
							ui.Add(bQuarter, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
							ui.Add(bSixteenth, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
							ui.Add(bUnits, OFX_IM_TOGGLE_SMALL, 4);

							if (bBig) ui.Add(cBig);
							if (bQuarter) ui.Add(cQuarter);
							if (bSixteenth) ui.Add(cSixteenth);
							if (bUnits) ui.Add(cUnits);

							ui.EndTree();
						}
					}
					if (bEnableBg) {
						if (ui.BeginTree("Background")) {
							ui.Add(gradientType);
							string s;
							switch (gradientType)
							{
							case 0: s = "One Color"; break;
							case 1: s = "Linear"; break;
							case 2: s = "Circular"; break;
							case 3: s = "Bar"; break;
							}
							ui.AddLabel(s);
							ui.Add(cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(bFlipBg);

							ui.EndTree();
						}
					}
					bool b = !bEnableBg && bForceBitmap && bDefaultColors;
					if (!b) ui.AddSpacingSeparated();

					if (ui.AddButton("Reset", OFX_IM_BUTTON, 3, true)) {
						doResetSettings();
					}
					if (ui.AddButton("Reset Colors", OFX_IM_BUTTON, 3, true)) {
						doResetColors();
					}
					if (ui.AddButton("Reset All", OFX_IM_BUTTON, 3)) {
						doResetAll();
					}
					ui.AddSpacing();

					if (ui.AddButton("Save", OFX_IM_BUTTON, 2, true)) {
						doSave();
					}
					if (ui.AddButton("Load", OFX_IM_BUTTON, 2)) {
						doLoad();
					}
					ui.AddSpacingSeparated();

					static bool bDebug = false;
					ui.AddToggle("Debug", bDebug, OFX_IM_TOGGLE_ROUNDED_MINI);
					if (bDebug) {
						string s = "";
						s += "StepSize: " + ofToString(stepSize);
						s += "\nNumberOfSteps: " + ofToString(numberOfSteps);
						s += "\nSize: " + ofToString(gridSize);
						s += "\nScale: " + ofToString(scale);
						ui.AddLabel(s);
					}
				}

				ui.EndWindow();
			}
		}
#endif

	};
}
