#pragma once
#include "ofMain.h"

// -> Could be completely disabled or replaced by ofxGui. Just using the ofParams!
#define USE_IM_GUI__SCENE
#ifdef USE_IM_GUI__SCENE
#include "ofxSurfingImGui.h"
#endif

/*

	BUGS:

	fix transform problems for ttf (non bitmap) fonts bc cam scaling
		ttf fonts work bad when translations applied to the camera,
		like rotation.
		Scale is handled well.

	These are the main methods where text coords are converted to 2D:
		get3dGraphics().drawGridPlane(stepSize,numberOfSteps,labels);
		renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);

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
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Constructor()";

			// Fix some exit exceptions.
			int minValue = std::numeric_limits<int>::min();
			ofAddListener(ofEvents().exit, this, &SurfSceneGrids::exit, minValue);
		}

		~SurfSceneGrids() {
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Destructor()";

			ofRemoveListener(ofEvents().exit, this, &SurfSceneGrids::exit);
		}

	private:
		void exit(ofEventArgs& args)
		{
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "exit(ofEventArgs& args)";
			exit();
		}
		void exit() {
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "exit()";
			doSave();
		}

	public:
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

		ofParameter<bool> bGui{ "FLOOR GRID", true };
		//ofParameter<bool> bGui{ "FloorGrid", true };

		ofParameter<ofColor> cText{ "C Text", ofColor(250), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cBig{ "C Big", ofColor(ofColor::blue, 128), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cQuarter{ "C Quarter", ofColor(ofColor::orange, 128), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cSixteenth{ "C Sixteenth", ofColor(ofColor::yellow, 128), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cUnits{ "C Units", ofColor(128, 128), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cBg1{ "C Bg1", ofColor(128, 128), ofColor(0), ofColor(255, 255) };
		ofParameter<ofColor> cBg2{ "C Bg2", ofColor(128, 128), ofColor(0), ofColor(255, 255) };

		ofParameter<bool> bBig{ "Big", true };
		ofParameter<bool> bQuarter{ "Quarter", true };
		ofParameter<bool> bSixteenth{ "Sixteenth", true };
		ofParameter<bool> bUnits{ "Units", true };
		ofParameter<bool> bAxis{ "Axis", true };

		ofParameter<bool> bDraw{ "Floor", true };

		ofParameter<bool> bDefaultColors{ "Default Colors", true };
		ofParameter<bool> bForceBitmap{ "Force ofBitmapFont", false };
		ofParameter<bool> bFlipBg{ "Flip Bg", false };
		ofParameter<bool> bEnableBg{ "Background", true };
		ofParameter<bool> bLabels{ "Labels", true };
		ofParameter<bool> bFontSmall{ "Small Font", false };
		ofParameter<bool> bOffsetLabels{ "Offset Labels", false };
		ofParameter<int> gradientType{ "Gradient Type", 2, 0, 3 };
		vector<string> gradienTypeNames{ "One Color", "Linear", "Circular", "Bar"};

	public:

		// Grid sizes
		//exposed to user
		ofParameter<float> stepSize{ "StepSize", 10, 0.1, 50 };
		ofParameter<int> numberOfSteps { "NumSteps", 10, 4, 50 };
		float gridSize = 100;
		//ofParameter<int> gridSize { "Size", 100, 4, 500 };

		ofParameterGroup paramsGrid{ "Grid" };

		float scale = 1;//TODO: workaround trick to avoid transform errors..(only for ofScale, rotations fails!)

		//--

		void setup()
		{
			params.add(bGui);
			params.add(bDraw);
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

			paramsGrid.add(stepSize);
			paramsGrid.add(numberOfSteps);
			//paramsGrid.add(gridSize);
			params.add(paramsGrid);

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
		}

		void drawBg()
		{
			if (!bDoneSetup) setup();

			if (!bDraw) return;
			if (!bEnableBg) return;

			// Bg one color
			if (gradientType == 0) {
				ofClear((bFlipBg ? cBg1 : cBg2));
			}
			else {
				// Bg gradient
				ofxSurfingHelpers::ofxDrawBgGradient((bFlipBg ? cBg1 : cBg2), (bFlipBg ? cBg2 : cBg1), ofGradientMode(gradientType.get() - 1));
			}
		}

		void drawOutsideCam()//draw labels
		{
			if (!bDoneSetup) setup();

			if (!bDraw) return;
			if (!bLabels) return;
			if (bForceBitmap) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call floorGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

#ifdef USE_SURFING_DEBUG_ALPHA
			//TODO: fixing overlap transparencies..
			ofEnableAlphaBlending();
			ofEnableDepthTest();
#endif

			//workaround 
			// Draw labels, out of the camera
			// custom font
			//if (!bForceBitmap)
			{
				ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

				if (bDefaultColors) ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale);
				else ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale, cText);
			}
		}

		void drawInsideCam() {
			if (!bDoneSetup) setup();
			if (!bDraw) return;
			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call floorGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

#ifdef USE_SURFING_DEBUG_ALPHA
			//TODO: fixing overlap transparencies..
			ofEnableAlphaBlending();
			ofEnableDepthTest();
#endif

			gridSize = numberOfSteps * stepSize;

			// Axis
			//TODO: split in components and sizes..
			if (bAxis)
			{
				float sza = gridSize / 20.f;
				ofDrawAxis(sza);
			}

			//--

			// Floor lines: 

			// 1. Big
			// (outer square)
			if (bBig)
			{
				ofSetColor(bDefaultColors ? SURFING_RULES_COLOR_LINES_BIG : cBig);
				ofxSurfingHelpers::ofxDrawFloorRectangle(gridSize);
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

			// 4. Units 
			// (with labels when ofBitmapFont)
			if (bUnits)
			{
				if (!bForceBitmap)
				{
					// custom font
					ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

					ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera, bUnits,
						bDefaultColors ? SURFING_RULES_COLOR_LINES_UNITS : cUnits,
						bDefaultColors ? SURFING_RULES_COLOR_TEXT : cText);
				}
				else
				{
					// bitmap font
					ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, 0, false, true, false, bUnits,
						bDefaultColors ? SURFING_RULES_COLOR_LINES_UNITS : cUnits,
						bDefaultColors ? SURFING_RULES_COLOR_TEXT : cText);
				}
			}

			// separated labels here
			if (bForceBitmap && bLabels)
				ofxSurfingHelpers::ofxDrawGridLabelsBitmapFonts(stepSize, numberOfSteps, false, true, false,
					bDefaultColors ? SURFING_RULES_COLOR_TEXT : cText);
		}

		//--

		void doSave() {
			string path;
			if (pathGlobal == "") path = params.getName() + fsuffix;
			else path = pathGlobal + "/" + params.getName() + fsuffix;
			ofxSurfingHelpers::saveGroup(params, path);

			ofLogNotice("of xSurfingHelpers::SurfSceneGrids") << "Setup() Save settings: " << path;
		}

		void doLoad() {
			string path;
			if (pathGlobal == "") path = params.getName() + fsuffix;
			else path = pathGlobal + "/" + params.getName() + fsuffix;
			ofxSurfingHelpers::loadGroup(params, path);

			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Setup() Load settings: " << path;
		}

		void doResetSettings() {
			bDraw = true;
			bEnableBg = true;
			bFlipBg = false;
			bLabels = true;
			bFontSmall = true;
			bOffsetLabels = false;
			//bForceBitmap = 1;

			bBig = 1;
			bQuarter = 1;
			bSixteenth = 1;
			bUnits = 1;

			//doResetGrid();
		}

		void doResetGrid() {
#if 0
			stepSize = 10;
			numberOfSteps = 10;
#else
			// Based on RF 1.0: 
			//// 5x5 meters
			//stepSize = 0.5;
			//numberOfSteps = 10;

			// 12x12 meters
			stepSize = 0.5;
			numberOfSteps = 24;
#endif
		}

		void doResetColors() {
#if(DEBUG_COLORS__SCENE)
			cText = SURFING_RULES_COLOR_TEXT;
			cBig = SURFING_RULES_COLOR_LINES_BIG;
			cQuarter = SURFING_RULES_COLOR_LINES_QUARTER;
			cSixteenth = SURFING_RULES_COLOR_LINES_SIXTEENTH;
			cUnits = SURFING_RULES_COLOR_LINES_UNITS;
			cBg1 = SURFING_RULES_COLOR_BG_1;
			cBg2 = SURFING_RULES_COLOR_BG_2;
#else
			//green and grey
			cText = ofColor{ 255, 255, 255, 200 };
			cBig = ofColor{ 0, 255, 175, 150 };
			cQuarter = ofColor{ 0, 255, 203, 75 };
			cSixteenth = ofColor{ 181, 181, 181, 48 };
			cUnits = ofColor{ 0, 0, 0, 24 };
			cBg1 = ofColor{ 0, 0, 0 };
			cBg2 = ofColor{ 64, 64, 64 };

			//grey
			//cBig = ofColor{ 96, 96, 96, 150 };
			//cQuarter = ofColor{ 64, 64, 64, 150 };
			//cSixteenth = ofColor{ 64, 64, 64, 150 };
			//cUnits = ofColor{ 96, 96, 96, 24 };
#endif

		}

		void doResetAll() {
			doResetSettings();
			doResetColors();
			doResetGrid();
		}

	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;

		//--

#ifdef USE_IM_GUI__SCENE
	public:

		void drawImGuiWidgets(ofxSurfingGui& ui)
		{
			if (!bGui) return;

			//ui.AddMinimizerToggle();
			//ui.AddSpacingSeparated();
			//ui.AddSpacing();

			//--

			string s = bGui.getName();
			s = ofToUpper(s);
			ui.AddLabelBig(s, false, true);

			ui.Add(bDraw, OFX_IM_TOGGLE_BIG);

			if (bDraw)
			{
				ui.Add(bLabels, OFX_IM_TOGGLE);
				ui.Add(bEnableBg, OFX_IM_TOGGLE);
			}

			ui.AddSpacingBigSeparated();

			//--

			if (bDraw)
			{
				ui.Indent();

				// We have two different populations,
				// one for minimized and another when not-minimized (aka maximized)
				if (ui.isMinimized())
				{
					string s = ofToString(gridSize, 0) + "mts x " + ofToString(gridSize, 0) + "mts";
					ui.AddLabel(s);
					ui.AddSpacing();

					ui.Add(stepSize, OFX_IM_STEPPER);
					ui.Add(numberOfSteps, OFX_IM_STEPPER);
					ui.AddSpacing();
					if (ui.AddButton("Reset##GRID")) {
						doResetGrid();
					}
					ui.AddSpacingBigSeparated();

					ui.Add(bForceBitmap);
					ui.Add(bDefaultColors);
					ui.AddSpacingBigSeparated();

					if (!bForceBitmap) {
						if (bLabels) {
							ui.Add(cText);
							if (!bDefaultColors) ui.AddSpacingBig();
						}
					}

					if (!bDefaultColors)
					{
						if (bBig) ui.Add(cBig);
						if (bQuarter) ui.Add(cQuarter);
						if (bSixteenth) ui.Add(cSixteenth);
						if (bUnits) ui.Add(cUnits);
					}

					if (bEnableBg) {
						ui.AddSpacingBig();
						ui.Add(cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
						ui.Add(cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);
					}

					ui.AddSpacing();
					ui.Add(bAxis);

					ui.AddSpacingBigSeparated();

					if (ui.AddButton("Reset All", OFX_IM_BUTTON)) {
						doResetAll();
					}
					ui.AddSpacingBigSeparated();

					if (ui.AddButton("Save", OFX_IM_BUTTON, 2, true)) {
						doSave();
					}
					if (ui.AddButton("Load", OFX_IM_BUTTON, 2)) {
						doLoad();
					}
				}
				else //if (ui.isMaximized())
				{
					ui.Add(bForceBitmap);
					ui.Add(bDefaultColors);
					ui.AddSpacingSeparated();

					string s;

					s = "Grid";
					s = ofToUpper(s);
					if (ui.BeginTree(s)) {
						string s = ofToString(gridSize, 0) + "mts x " + ofToString(gridSize, 0) + "mts";
						ui.AddLabel(s);
						ui.AddSpacing();

						ui.Add(stepSize, OFX_IM_STEPPER);
						ui.Add(numberOfSteps, OFX_IM_STEPPER);
						ui.AddSpacing();

						if (ui.AddButton("Reset##GRID")) {
							doResetGrid();
						}

						ui.EndTree();
					}

					ui.AddSpacingSeparated();

					s = "Lines";
					s = ofToUpper(s);
					if (ui.BeginTree(s)) {
						ui.AddSpacing();
						ui.Add(bBig, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
						ui.Add(bQuarter, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
						ui.Add(bSixteenth, OFX_IM_TOGGLE_SMALL, 4); ui.SameLine();
						ui.Add(bUnits, OFX_IM_TOGGLE_SMALL, 4);

						if (!bDefaultColors)
						{
							if (bBig) ui.Add(cBig);
							if (bQuarter) ui.Add(cQuarter);
							if (bSixteenth) ui.Add(cSixteenth);
							if (bUnits) ui.Add(cUnits);
						}

						ui.AddSpacing();
						ui.Add(bAxis);

						ui.EndTree();
					}

					if (bEnableBg) {
						ui.AddSpacingSeparated();

						s = "Background";
						s = ofToUpper(s);
						if (ui.BeginTree(s)) {
							ui.AddCombo(gradientType, gradienTypeNames);
							//ui.Add(gradientType);
							ui.Add(cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(bFlipBg);

							ui.EndTree();
						}
					}

					if (bLabels) {
						if (!bForceBitmap) {
							ui.AddSpacingSeparated();
							s = "Labels";
							s = ofToUpper(s);
							if (ui.BeginTree(s)) {

								ui.Add(bFontSmall);
								ui.Add(bOffsetLabels);
								if (!bForceBitmap && bLabels) ui.Add(cText);

								ui.EndTree();
							}
						}
					}

					//ui.AddSpacingSeparated();

					//bool b = !bEnableBg && bForceBitmap && bDefaultColors;
					//if (!b) ui.AddSpacingSeparated();
					//if (!b) ui.AddSpacing();

					ui.AddSpacingBigSeparated();

					if (ui.AddButton("Reset", OFX_IM_BUTTON, 2, true)) {
						doResetSettings();
					}
					if (ui.AddButton("Reset Colors", OFX_IM_BUTTON, 2)) {
						doResetColors();
					}
					if (ui.AddButton("Reset All", OFX_IM_BUTTON)) {
						doResetAll();
					}

					ui.AddSpacingBigSeparated();

					if (ui.AddButton("Save", OFX_IM_BUTTON, 2, true)) {
						doSave();
					}
					if (ui.AddButton("Load", OFX_IM_BUTTON, 2)) {
						doLoad();
					}
				}

				ui.Unindent();
			}
		}

		void drawImGuiWindow(ofxSurfingGui& ui)
		{
			if (!bDoneSetup) setup();
			if (!bGui) return;

			if (ui.isMinimized() && bGui)
			{
				IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;
			}
			else
			{
				IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;
			}

			if (ui.BeginWindow(bGui))
			{
				drawImGuiWidgets(ui);

				ui.EndWindow();
			}
		}
#endif

	};
}
