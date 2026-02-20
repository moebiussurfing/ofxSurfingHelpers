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
		SurfSceneGrids()
		{
			ofAddListener(ofEvents().update, this, &SurfSceneGrids::update);

#ifdef SURFING_IMGUI__CREATE_EXIT_LISTENER
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "Constructor()";

			// Fix some exit exceptions.
			int minValue = std::numeric_limits<int>::min();
			ofAddListener(ofEvents().exit, this, &SurfSceneGrids::exit, minValue);
#endif
		}

		~SurfSceneGrids()
		{
#ifdef SURFING_IMGUI__CREATE_EXIT_LISTENER
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "Destructor()";

			ofRemoveListener(ofEvents().exit, this, &SurfSceneGrids::exit);
#endif
		}

	private:
#ifdef SURFING_IMGUI__CREATE_EXIT_LISTENER
		void exit(ofEventArgs& args)
		{
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "exit(ofEventArgs& args)";
			exit();
		}
#endif

	private:
		void update(ofEventArgs& args) // subscribed after app
		{
			update();
		}
		void update()
		{
#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
			if (bFlagSaveSettings) {
				bFlagSaveSettings = false;
				doSave(true);
			}
#endif	
		}

	private:
		void exit()
		{
			ofRemoveListener(params.parameterChangedE(), this, &SurfSceneGrids::Changed); // exit()
			ofRemoveListener(ofEvents().update, this, &SurfSceneGrids::update);

#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_EXIT
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "exit()";
			doSave();
#endif
		}

		//--

	private:
#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
		bool bFlagSaveSettings = 0;
	public:
		ofParameter<bool> bAutoSaveSettings{ "Auto Save", true };

	private:
		//--------------------------------------------------------------
		void saveSettingsFlag()
		{
			if (bAutoSaveSettings) bFlagSaveSettings = true;
			else
			{
				ofLogWarning("ofxSurfingHelpers:SurfSceneGrids") << "saveSettingsFlag()";
				ofLogWarning("ofxSurfingHelpers:SurfSceneGrids") << "Skipped saveSettingsFlag() bc bAutoSaveSettings was disabled!";
			}
		}
#endif

		//--

	public:
		void setCameraPtr(ofCamera* _camera) { camera = _camera; };

	private:
		ofCamera* camera = nullptr;

		string pathGlobal = "";

		//string fileSuffix = "_Settings.json";
		string fileSuffix = ".json";

		bool bDoneSetup = false;

	public:
		void setPathGlobal(string path)
		{
			pathGlobal = path;

			if (!bDoneSetup) setup();
		}
		void setLogLevel(ofLogLevel level)
		{
			ofSetLogLevel("ofxSurfingHelpers", level);
			ofSetLogLevel("ofxSurfingHelpers:SurfSceneGrids", level);
		}

		// Name will be used for the file settings name.
		ofParameterGroup params{ "Floor" };

		ofParameterGroup paramsModes{ "Modes" };
		ofParameterGroup paramsColors{ "Colors" };
		ofParameterGroup paramsZones{ "Zones" };
		ofParameterGroup paramsBg{ "Background" };
		ofParameterGroup paramsElements{ "Elements" };

		ofParameter<bool> bGui{"FLOOR GRID", true};

		ofParameter<ofColor> cText{"C Text", ofColor(250), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cBig{"C Big", ofColor(ofColor::blue, 128), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cQuarter{"C 4th", ofColor(ofColor::orange, 128), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cSixteenth{"C 16th", ofColor(ofColor::yellow, 128), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cUnits{"C Units", ofColor(128, 128), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cBg1{"C Bg1", ofColor(128, 128), ofColor(0), ofColor(255, 255)};
		ofParameter<ofColor> cBg2{"C Bg2", ofColor(128, 128), ofColor(0), ofColor(255, 255)};

		ofParameter<bool> bBig{"Big", true};
		ofParameter<bool> bQuarter{"4th", true};
		ofParameter<bool> bSixteenth{"16th", true};
		ofParameter<bool> bUnits{"Units", true};
		ofParameter<bool> bAxis{"Axis", true};

		ofParameter<bool> bDraw{"Floor", true};

		ofParameter<bool> bDefaultColors{"Default Colors", false};
		ofParameter<bool> bForceBitmap{"Force ofBitmapFont", false};
		ofParameter<bool> bFlipBg{"Flip Bg", false};
		ofParameter<bool> bEnableBg{"Background", true};
		ofParameter<bool> bLabels{"Labels", true};
		ofParameter<bool> bFontSmall{"Small Font", false};
		ofParameter<bool> bOffsetLabels{"Offset Labels", false};
		ofParameter<int> gradientType{"Gradient Type", 2, 0, 3};
		vector<string> gradienTypeNames{"One Color", "Linear", "Circular", "Bar"};

	public:
		// Grid sizes
		//exposed to user
		ofParameter<float> stepSize{ "StepSize", 10, 0.1, 50 };
		ofParameter<int> numberOfSteps{"NumSteps", 10, 4, 50};
		float gridSize = 100;
		//ofParameter<int> gridSize { "Size", 100, 4, 500 };

		ofParameterGroup paramsGrid{ "Grid" };

		float scale = 1; //TODO: workaround trick to avoid transform errors..(only for ofScale, rotations fails!)

		//--

		void setup()
		{
			paramsElements.add(bGui);
			paramsElements.add(bLabels);
			paramsElements.add(bDraw);
			paramsElements.add(bAxis);
			params.add(paramsElements);

			paramsModes.add(bForceBitmap);
			paramsModes.add(bFontSmall);
			paramsModes.add(bOffsetLabels);
			paramsModes.add(bDefaultColors);
#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
			paramsModes.add(bAutoSaveSettings);
#endif
			params.add(paramsModes);

			paramsColors.add(cText);
			paramsColors.add(cBig);
			paramsColors.add(cQuarter);
			paramsColors.add(cSixteenth);
			paramsColors.add(cUnits);
			params.add(paramsColors);

			////TODO: allow disable some lines.
			// requires modifying ofxSurfingHelpers::ofxDrawFloor 
			paramsZones.add(bBig);
			paramsZones.add(bQuarter);
			paramsZones.add(bSixteenth);
			paramsZones.add(bUnits);
			params.add(paramsZones);

			paramsBg.add(bEnableBg);
			paramsBg.add(gradientType);
			paramsBg.add(cBg1);
			paramsBg.add(cBg2);
			paramsBg.add(bFlipBg);
			params.add(paramsBg);

			paramsGrid.add(stepSize);
			paramsGrid.add(numberOfSteps);
			params.add(paramsGrid);

			ofAddListener(params.parameterChangedE(), this, &SurfSceneGrids::Changed); // setup()

			//--

			// Font file
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

	private:
		void Changed(ofAbstractParameter& e)
		{
			string n = e.getName();
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "Changed: " << n << " : " << e;

#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
			saveSettingsFlag();
#endif
			//workflow
			if (n == bAutoSaveSettings.getName())
			{
				doSave();
				return;
			}
		}

	public:
		void drawBg()
		{
			if (!bDoneSetup) setup();

			if (!bDraw) return;
			if (!bEnableBg) return;

			// Bg one color
			if (gradientType == 0)
			{
				ofClear((bFlipBg ? cBg1 : cBg2));
			}
			else
			{
				// Bg gradient
				ofxSurfingHelpers::ofxDrawBgGradient((bFlipBg ? cBg1 : cBg2), (bFlipBg ? cBg2 : cBg1),
					ofGradientMode(gradientType.get() - 1));
			}
		}

		void drawOutsideCam() //draw labels
		{
			if (!bDoneSetup) setup();

			if (!bDraw) return;
			if (!bLabels) return;
			if (bForceBitmap) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Call floorGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Forced ofBitmapFont!";
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

				if (bDefaultColors)
					ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(
						stepSize, numberOfSteps, f, bOffsetLabels, camera, scale);
				else
					ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(
						stepSize, numberOfSteps, f, bOffsetLabels, camera, scale, cText);
			}
		}

		void drawInsideCam()
		{
			if (!bDoneSetup) setup();
			if (!bDraw) return;
			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Call floorGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers:SurfSceneGrids") << "Forced ofBitmapFont!";
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

					ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera,
						bUnits,
						(bDefaultColors) ? SURFING_RULES_COLOR_LINES_UNITS : cUnits,
						(bDefaultColors) ? SURFING_RULES_COLOR_TEXT : cText);
				}
				else
				{
					// bitmap font
					ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, 0, false, true, false, bUnits,
						(bDefaultColors) ? SURFING_RULES_COLOR_LINES_UNITS : cUnits,
						(bDefaultColors) ? SURFING_RULES_COLOR_TEXT : cText);
				}
			}

			// separated labels here
			if (bForceBitmap && bLabels)
				ofxSurfingHelpers::ofxDrawGridLabelsBitmapFonts(stepSize, numberOfSteps, false, true, false,
					(bDefaultColors) ? SURFING_RULES_COLOR_TEXT : cText);
		}

		//--

		string getPathSettings() const
		{
			string path;
			if (pathGlobal == "") path = params.getName() + fileSuffix;
			else path = pathGlobal + "/" + params.getName() + fileSuffix;
			return path;
		}

		void doSave(bool bSilent = false)
		{
			string path;
			if (pathGlobal == "") path = params.getName() + fileSuffix;
			else path = pathGlobal + "/" + params.getName() + fileSuffix;
			ofxSurfingHelpers::saveGroup(params, path, !bSilent);

			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doSave() Save settings: " << path;
		}

		void doLoad()
		{
			string path;
			if (pathGlobal == "") path = params.getName() + fileSuffix;
			else path = pathGlobal + "/" + params.getName() + fileSuffix;
			ofxSurfingHelpers::loadGroup(params, path);

			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doLoad() Load settings: " << path;
		}

		void doResetSettings()
		{
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doResetSettings()";

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

		void doResetGrid()
		{
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doResetGrid()";

			// 12x12 meters
#if 1 // units 1.0
			stepSize = 1;
			numberOfSteps = 6;
#else // units 0.5
			stepSize = 1.5;
			numberOfSteps = 12;
#endif

			// Based on RF 1.0: 
			//// 5x5 meters
			//stepSize = 0.5;
			//numberOfSteps = 10;
		}

		void doResetColors()
		{
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doResetColors()";

#if(DEBUG_COLORS__SCENE)
			cText = SURFING_RULES_COLOR_TEXT;
			cBig = SURFING_RULES_COLOR_LINES_BIG;
			cQuarter = SURFING_RULES_COLOR_LINES_QUARTER;
			cSixteenth = SURFING_RULES_COLOR_LINES_SIXTEENTH;
			cUnits = SURFING_RULES_COLOR_LINES_UNITS;
			cBg1 = SURFING_RULES_COLOR_BG_1;
			cBg2 = SURFING_RULES_COLOR_BG_2;
#else
			// Green and gray. dark
#if 0
			cBg1 = ofColor{ 40, 40, 40 };
			cBg2 = ofColor{ 0, 0, 0 };
			cText = ofColor{ 255, 255, 255, 200 };
			cBig = ofColor{ 0, 255, 175, 150 };
			cQuarter = ofColor{ 0, 255, 203, 75 };
			cSixteenth = ofColor{ 181, 181, 181, 48 };
			cUnits = ofColor{ 0, 0, 0, 24 };
#else
			// Gray. light
			cBg1 = SURFING_RULES_COLOR_BG_1;
			cBg2 = SURFING_RULES_COLOR_BG_2;
			cText = SURFING_RULES_COLOR_TEXT;
			cBig = SURFING_RULES_COLOR_LINES_BIG;
			cQuarter = SURFING_RULES_COLOR_LINES_QUARTER;
			cSixteenth = SURFING_RULES_COLOR_LINES_SIXTEENTH;
			cUnits = SURFING_RULES_COLOR_LINES_UNITS;
#endif

#endif
		}

		void doResetAll()
		{
			ofLogNotice("ofxSurfingHelpers:SurfSceneGrids") << "doResetColors()";

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
				if (ui.isMinimized()) // Minimized
				{
					string s = ofToString(2 * gridSize, 0) + "mts x " + ofToString(2 * gridSize, 0) + "mts";
					ui.AddLabelBig(s);
					ui.AddSpacing();

					ui.Add(stepSize, OFX_IM_STEPPER);
					ui.Add(numberOfSteps, OFX_IM_STEPPER);
					ui.AddSpacing();
					if (ui.AddButton("Reset##GRID"))
					{
						doResetGrid();
					}
					ui.AddSpacingBigSeparated();

					if (bLabels) ui.Add(bForceBitmap);
					ui.Add(bDefaultColors);
					ui.AddSpacingBigSeparated();

					if (bLabels)
					{
						if (!bForceBitmap)
						{
							ui.Add(cText);
							if (!bDefaultColors) ui.AddSpacingBig();
						}
					}

					if (!bDefaultColors)
					{
						SurfingGuiTypes t = OFX_IM_DEFAULT;
						//SurfingGuiTypes t = OFX_IM_COLOR_BOX_FULL_WIDTH_NO_ALPHA;
						if (bBig) ui.Add(cBig, t);
						if (bQuarter) ui.Add(cQuarter, t);
						if (bSixteenth) ui.Add(cSixteenth, t);
						if (bUnits) ui.Add(cUnits, t);

						//if (bBig) ui.Add(cBig);
						//if (bQuarter) ui.Add(cQuarter);
						//if (bSixteenth) ui.Add(cSixteenth);
						//if (bUnits) ui.Add(cUnits);
					}

					if (bEnableBg)
					{
						ui.AddSpacingBig();
						ui.Add(cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
						ui.Add(cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);
					}

					ui.AddSpacingBig();
					ui.Add(bAxis);

					ui.AddSpacingBigSeparated();

					int n_widgets = 2;
					float w = ui.getWidgetsWidth(n_widgets);
					float h = ui.getWidgetsHeightUnit() * 1.5f;
					ImVec2 sz1(ui.getWidgetsWidth(), h);
					ImVec2 sz2(w, h);
					if (ui.AddButton("Reset", sz1))
					{
						doResetAll();
					}
					if (ui.AddButton("Load", sz2))
					{
						doLoad();
					}
					ui.SameLine();
					if (ui.AddButton("Save", sz2))
					{
						doSave();
					}

#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
					ui.AddSpacing();
					ui.Add(bAutoSaveSettings, OFX_IM_TOGGLE_ROUNDED_MINI);
#endif
				} // Minimized
				else //if (ui.isMaximized()) // Maximized
				{
					ui.Add(bForceBitmap);
					ui.Add(bDefaultColors);
					ui.AddSpacingSeparated();

					string s;

					s = "Grid";
					s = ofToUpper(s);
					if (ui.BeginTree(s, true))
					{
						string s = ofToString(2 * gridSize, 0) + "mts x " + ofToString(2 * gridSize, 0) + "mts";
						ui.AddLabelBig(s);
						ui.AddSpacing();

						ui.Add(stepSize, OFX_IM_STEPPER);
						ui.Add(numberOfSteps, OFX_IM_STEPPER);
						ui.AddSpacing();

						if (ui.AddButton("Reset##GRID"))
						{
							doResetGrid();
						}

						ui.EndTree();
					}

					ui.AddSpacingSeparated();

					s = "Lines";
					s = ofToUpper(s);
					if (ui.BeginTree(s, true))
					{
						ui.AddSpacing();
						ui.Add(bBig, OFX_IM_TOGGLE_SMALL, 4);
						ui.SameLine();
						ui.Add(bQuarter, OFX_IM_TOGGLE_SMALL, 4);
						ui.SameLine();
						ui.Add(bSixteenth, OFX_IM_TOGGLE_SMALL, 4);
						ui.SameLine();
						ui.Add(bUnits, OFX_IM_TOGGLE_SMALL, 4);
						ui.AddSpacingBig();

						if (!bDefaultColors)
						{
							SurfingGuiTypes t = OFX_IM_DEFAULT;
							if (bBig) ui.Add(cBig, t);
							if (bQuarter) ui.Add(cQuarter, t);
							if (bSixteenth) ui.Add(cSixteenth, t);
							if (bUnits) ui.Add(cUnits, t);
						}

						ui.AddSpacingBig();
						ui.Add(bAxis);

						ui.EndTree();
					}

					if (bEnableBg)
					{
						ui.AddSpacingSeparated();

						s = "Background";
						s = ofToUpper(s);
						if (ui.BeginTree(s))
						{
							ui.AddCombo(gradientType, gradienTypeNames);
							//ui.Add(gradientType);
							ui.Add(cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);
							ui.Add(bFlipBg);

							ui.EndTree();
						}
					}

					if (bLabels)
					{
						if (!bForceBitmap)
						{
							ui.AddSpacingSeparated();
							s = "Labels";
							s = ofToUpper(s);
							if (ui.BeginTree(s))
							{
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

					int n_widgets = 2;
					float w = ui.getWidgetsWidth(n_widgets);
					float h = ui.getWidgetsHeightUnit() * 1.5f;
					ImVec2 sz2(w, h);
					ImVec2 sz1(ui.getWidgetsWidth(), h);

					if (ui.AddButton("Reset Settings", sz2))
					{
						doResetSettings();
					}
					ui.SameLine();
					if (ui.AddButton("Reset Colors", sz2))
					{
						doResetColors();
					}

					if (ui.AddButton("Reset", sz1))
					{
						doResetAll();
					}

					if (ui.AddButton("Load", sz2))
					{
						doLoad();
					}
					ui.SameLine();
					if (ui.AddButton("Save", sz2))
					{
						doSave();
					}

#ifdef SURFING_IMGUI__ENABLE_SAVE_ON_CHANGES
					ui.AddSpacing();
					ui.Add(bAutoSaveSettings, OFX_IM_TOGGLE_ROUNDED_MINI);
#endif
				} // Maximized

				ui.Unindent();
			}
		}

		void drawImGuiWindow(ofxSurfingGui& ui)
		{
			if (!bDoneSetup) setup();
			if (!bGui) return;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;//skip internal autoresize

			// autoresize makes weird color labels?
			if (ui.isMinimized())
			{
				//IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;
				float w = 220;
				ImVec2 size_min = ImVec2(w, -1);
				ImVec2 size_max = ImVec2(w, -1);
				ImGui::SetNextWindowSizeConstraints(size_min, size_max);
				window_flags += ImGuiWindowFlags_NoResize;
			}
			else
			{
				if (ui.bAutoResize) window_flags += ImGuiWindowFlags_AlwaysAutoResize;
				IMGUI_SUGAR__WINDOWS_CONSTRAINTSW;
				//IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;
			}

			if (ui.BeginWindow(bGui, window_flags))
			{
				drawImGuiWidgets(ui);

				ui.EndWindow();
		}
	}
#endif
};
}
