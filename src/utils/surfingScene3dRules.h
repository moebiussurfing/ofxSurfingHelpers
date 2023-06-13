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

		ofParameter<bool> bGui{ "FloorGrid", true };

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
		float stepSize = 10;//exposed to user
		float numberOfSteps = 10;//exposed to user
		float gridSize = 100;//for debug only

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
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
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
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
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
			cText = ofColor{ 255, 255, 255, 200 };
			cBig = ofColor{ 0, 255, 175, 150 };
			cQuarter = ofColor{ 0, 255, 203, 75 };
			cSixteenth = ofColor{ 181, 181, 181, 48 };
			cUnits = ofColor{ 0, 0, 0, 24 };
			cBg1 = ofColor{ 0, 0, 0 };
			cBg2 = ofColor{ 64, 64, 64 };

			//cBig = ofColor{ 96, 96, 96, 150 };
			//cQuarter = ofColor{ 64, 64, 64, 150 };
			//cSixteenth = ofColor{ 64, 64, 64, 150 };
			//cUnits = ofColor{ 96, 96, 96, 24 };
#endif

	}

		void doResetAll() {
			doResetSettings();
			doResetColors();
		}

	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;

		//--

#ifdef USE_IM_GUI__SCENE
	public:

		void drawImGuiWindow(ofxSurfingGui& ui)
		{
			if (!bDoneSetup) setup();
			if (!bGui) return;

			if (ui.isMinimized() && bGui) {
				IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;
			}
			else {
				IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;
			}

			if (ui.BeginWindow(bGui))
			{
				ui.AddMinimizerToggle();

				ui.AddSpacingSeparated();

				//--

				ui.AddLabelBig("FloorGrid", false, true);

				ui.Add(bDraw, OFX_IM_TOGGLE_BIG);

				if (bDraw)
				{
					//ui.Indent();

					ui.Add(bLabels, OFX_IM_TOGGLE);
					ui.Add(bEnableBg, OFX_IM_TOGGLE);
					ui.AddSpacing();

					if (ui.isMaximized())
					{
						ui.Add(bForceBitmap);
						ui.Add(bDefaultColors);
						ui.AddSpacingSeparated();
					}

					//ui.Unindent();
				}

				//--

				if (ui.isMaximized() && bDraw)
				{
					if (!bForceBitmap) {
						if (ui.BeginTree("Labels")) {
							ui.Add(bFontSmall);
							ui.Add(bOffsetLabels);
							if (!bForceBitmap) ui.Add(cText);

							ui.EndTree();
						}
					}
					ui.AddSpacing();
					if (ui.BeginTree("Lines")) {
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

						ui.Add(bAxis);
						//ui.Add(bAxis, OFX_IM_TOGGLE);

						ui.EndTree();
					}
					ui.AddSpacing();
					if (bEnableBg) {
						if (ui.BeginTree("Background")) {
							ui.AddCombo(gradientType, gradienTypeNames);
							/*
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
							*/
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

#if 1 
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
#endif
				}

				ui.EndWindow();
			}
		}
#endif

};
}
