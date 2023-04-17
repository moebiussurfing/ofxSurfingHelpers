#pragma once

/*

	TODO

	BUG
	autosave required to ctrl + copy

*/


//--

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
#include "imgui_stdlib.h"
//#include "surfingParamsRandom.h"

/*

	Simple Presets Manager

*/

/*

EXAMPLE

#ifdef USE_ofxSurfingPresetsLite

	#include "ofxSurfingPresetsLite.h"
	ofxSurfingPresetsLite presetsManager;

	// Setup
	presetsManager.setUiPtr(&ui);
	presetsManager.setPath(path_GLOBAL_Folder);
	presetsManager.AddGroup(params_Preset);

	// Draw
	// ImGui widgets not windowed
	ui.AddSpacingSeparated();
	presetsManager.drawImGui();

#endif

*/

//----

/*

	BUG:
	Seems that overwrites presets..

	TODO:
	improve first name.
		new auto set name to be faster
	add settings state for gui
	add auto save
	add rename preset
	add copy/swap?
	add save files
	search file listers from surfingPresets
*/

//----

class ofxSurfingPresetsLite
{
public:

	ofxSurfingPresetsLite::ofxSurfingPresetsLite()
	{
		ofAddListener(params.parameterChangedE(), this, &ofxSurfingPresetsLite::Changed);
		ofAddListener(ofEvents().update, this, &ofxSurfingPresetsLite::update);
		ofAddListener(ofEvents().exit, this, &ofxSurfingPresetsLite::exit);
		addKeysListeners();

		params.add(bGui);
		params.add(index);
		params.add(vScan);
		params.add(vDelete);
		params.add(vSave);
		params.add(vLoad);
		params.add(vNew);
		params.add(vRename);
		params.add(vReset);
		params.add(vRandom);
		params.add(bAutoSave);//edit
		params.add(bClicker);
		params.add(bExpand);
		params.add(amountButtonsPerRowClicker);
	};

	ofxSurfingPresetsLite::~ofxSurfingPresetsLite()
	{
		//exit();

		ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingPresetsLite::Changed);
		ofRemoveListener(ofEvents().update, this, &ofxSurfingPresetsLite::update);
		ofRemoveListener(ofEvents().exit, this, &ofxSurfingPresetsLite::exit);
		removeKeysListeners();
	};

private:

	void exit(ofEventArgs& args) {
		exit();
	};

	void exit() {

		ofxSurfingHelpers::CheckFolder(path_Global);

		string s;
		if (path_Global == "") s = path_Settings;
		else s = path_Global + "/" + path_Settings;
		ofxSurfingHelpers::saveGroup(params, s);

		doSave();
	};

	ofxSurfingGui* ui;

public:

	void setUiPtr(ofxSurfingGui* _ui) {
		ui = _ui;
	}

	// Customize global name to avoid window name collide with other preset manager instances
	//--------------------------------------------------------------
	void setName(std::string s)
	{
		//if (s == "-1") s = "PRESETS EDITOR";
		//else
		//{
		//	n = name_Root;
		//	n += " EDITOR";
		//}

		//name_Root = s;

		bGui.setName(s);

		//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
		//		playerSurfer.setName(s);
		//#endif
	}

private:
	string path_Settings = "ofxSurfingPresetsLite_Settings.json";

	bool bKeyCtrl = false;
	bool bKeyAlt = false;

	//----

private:

	vector<char> keyCommandsChars;
	vector<ofColor> colors;
	bool bUseColorizedMatrices = false;

public:
	void setColorized(bool b) { bUseColorizedMatrices = b; }

	//----

public:

	// Presets

	void drawImGui(bool bWindowed = false, bool bShowMinimizer = false, bool bFolder = false, bool bOpen = true)
	{
		if (!bGui) return;

		//TODO:
		// make windowed
		bool bw = true;

		if (bWindowed)
		{
			bw = ui->BeginWindow(bGui.getName());
			//bw = ui->BeginWindow("PRESETS");
		}
		else if (bFolder)
		{
			bw = true;
		}

		if (bw)
		{
			////TODO:
			//static string _namePreset = "";
			////static bool bTyping = false;
			//static bool bOverInputText = false;

			//string s = "presetName";
			string s = filename;

			//bool bFolder = !bWindowed;
			//bool bFolder = true;

			bool b = true;

			string sn = "PRESETS";

			if (!bFolder)
			{
				b = true;
				if (!bWindowed) ui->AddLabelBig(sn, true, true);
			}
			else
			{
				b = ui->BeginTree(sn, bOpen);
				//b = ui->BeginTree(sn);

				if (b) ui->AddSpacing();
			}

			if (b)
			{
				if (bShowMinimizer) ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED);

				if (!ui->bMinimize)
				{
					ui->Add(bExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui->AddSpacing();
				}

				//ui->AddToggle("Expand", bExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
				if (bExpand)
				{
					if (!ui->bMinimize) { // maximized
						ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);

						//if (ui->Add(vSave, OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2))

						if (ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2))
						{
							bOverInputText = false;
							_namePreset = s;
						};

						//if (ui->Add(vSave,
						//	((bAutoSave || !bOverInputText) ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2))

						//	//SurfingGuiTypes s = (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_TOGGLE_SMALL_BORDER_BLINK);
						//	//bool b;
						//	//b = (ui->Add(vSave, s, 2));
						//	//if(b)

						//{
						//	bOverInputText = false;
						//	_namePreset = s;
						//};

						if (!bOverInputText)
						{
							if (ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2))
							{
								doNewPreset();
							}
						}
						else
						{
							if (ui->AddButton("Cancel", OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2))
							{
								bOverInputText = false;
								bDoingNew = false;
							}
						}
						ui->SameLine();
						ui->Add(vDelete, OFX_IM_BUTTON_SMALL, 2);

						if (ui->Add(vRename, OFX_IM_BUTTON_SMALL, 2, true)) {
							//delete
							vDelete.trigger();
							//create new
							if (!bOverInputText) bOverInputText = true;
							_namePreset = "";
							setFilename(_namePreset);
						};
						ui->Add(bAutoSave, OFX_IM_TOGGLE_SMALL_BORDER_BLINK, 2);

						ui->Add(vReset, OFX_IM_BUTTON_SMALL, 2, true);
						ui->Add(vRandom, OFX_IM_BUTTON_SMALL, 2);

						//--

						ui->refreshLayout();
						float _w2 = getWidgetsWidth(2);
						float _h = getWidgetsHeightUnit();

						if (ImGui::Button("Clear", ImVec2(_w2, _h)))
						{
							ImGui::OpenPopup("CLEAR KIT ?");
						}
						ui->AddTooltip("Remove all \nfile Presets!");

						if (ImGui::BeginPopupModal("CLEAR KIT ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{
							ui->AddLabelBig("Presets Kit \nwill be erased.", true, true);
							ui->AddSpacing();
							ui->AddLabelBig("This operation \ncannot be undone!", true, true);
							ui->AddSpacingBig();

							static bool dont_ask_me_next_time = false;
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
							ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
							ImGui::PopStyleVar();

							ui->AddSpacing();

							if (!dont_ask_me_next_time) {
								if (ImGui::Button("OK", ImVec2(120, 2 * _h))) {
									ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__) << "CLEAR";
									doClearPresets();
									ImGui::CloseCurrentPopup();
								}
								ImGui::SetItemDefaultFocus();
								ImGui::SameLine();
								if (ImGui::Button("Cancel", ImVec2(120, 2 * _h))) {
									ImGui::CloseCurrentPopup();
								}
							}
							else {
								ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__) << "CLEAR";
								doClearPresets(false);
								ImGui::CloseCurrentPopup();

								if (bOverInputText) bOverInputText = false;
							}

							ImGui::EndPopup();
						}

						ui->SameLine();

						if (ImGui::Button("Populate", ImVec2(_w2, _h)))
						{
							if (bOverInputText) bOverInputText = false;

							doPopulatePresets();
						}
						ui->AddTooltip("Clear Kit and create \nNew Presets copying current");

						//--

						//ui->Add(vScan, OFX_IM_BUTTON_SMALL, 2);//should be automatic!
					}
					else // minimized
					{
						if (ui->Add(vSave, bOverInputText ? OFX_IM_BUTTON_SMALL_BORDER_BLINK : OFX_IM_BUTTON_SMALL, 2, true))
						{
							bOverInputText = false;
							_namePreset = s;
						};

						if (!bOverInputText) {
							if (ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2))
							{
								doNewPreset();

								//if (!bOverInputText) bOverInputText = true;

								//// default name
								//_namePreset = "";

								//// auto name
								//string _n = ofToString(dir.size());
								//bool bAvoidOverWrite = false;
								//for (int i = 0; i < dir.size(); i++)
								//{
								//	if (_n == dir.getName(i)) bAvoidOverWrite = true;
								//}
								//if (!bAvoidOverWrite) _namePreset = _n;

								//setFilename(_namePreset);
							}
						}
						else
						{
							if (ui->AddButton("Cancel",
								OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2)) {
								bOverInputText = false;
								bDoingNew = false;
							}
						}
					}

					//--

					if (bOverInputText)
					{
						if (!ui->bMinimize) ui->AddSpacing();

						int _w = ui->getWidgetsWidth() * 0.9f;
						ImGui::PushItemWidth(_w);
						{
							bool b = ImGui::InputText("##NAME", &s);
							if (b) {
								ofLogNotice("ofxSurfingPresetsLite") << "InputText:" << s.c_str();
								setFilename(s);
							}
							if (ImGui::IsItemDeactivated() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
								bOverInputText = false;
						}
						ImGui::PopItemWidth();
					}
				}

				if (ui->bMinimize && !bExpand)
				{
					ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_MEDIUM : OFX_IM_BUTTON_MEDIUM_BORDER_BLINK), 2, true);

					//if (ui->AddButton("Next", OFX_IM_BUTTON_MEDIUM, 2))
					//{
					//	doLoadNext();
					//};

					if (ui->AddButton("<", OFX_IM_BUTTON_MEDIUM, 4, true))
					{
						doLoadPrevious();
					};
					if (ui->AddButton(">", OFX_IM_BUTTON_MEDIUM, 4, false))
					{
						doLoadNext();
					};
				}

				//--

				if (!ui->bMinimize && dir.size() > 0) ui->AddSpacingSeparated();

				if (bExpand) // expanded
				{
					// Combo
					//ui->AddComboButtonDual(index, filenames, true);

					if (ui->bMinimize)
					{
						float p = ui->getWidgetsSpacingX();
						float w = ui->getWidgetsWidth() * 0.5 - p / 2;//?
						ImGui::PushItemWidth(w);
						ui->AddCombo(index, filenames, true);
						ImGui::PopItemWidth();

						ui->SameLine();
						if (ui->AddButton("Next", OFX_IM_BUTTON_SMALL, 2))
						{
							doLoadNext();
						}
					}
					else
					{
						float p = ui->getWidgetsSpacingX();
						float w = ui->getWidgetsWidth() * 1.f - p / 2;//?ImGui::PushItemWidth(w);
						ImGui::PushItemWidth(w);
						ui->AddCombo(index, filenames, true);
						ImGui::PopItemWidth();
					}
				}
				else // not expanded
				{
					if (!ui->bMinimize)
					{
						//ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_MEDIUM : OFX_IM_BUTTON_MEDIUM_BORDER_BLINK), 2, true);

						if (ui->AddButton("<", OFX_IM_BUTTON_MEDIUM, 2, true))
						{
							doLoadPrevious();
						};
						if (ui->AddButton(">", OFX_IM_BUTTON_MEDIUM, 2))
						{
							doLoadNext();
						};

						float p = ui->getWidgetsSpacingX();
						float w = ui->getWidgetsWidth() * 0.5 - p / 2;//?
						ImGui::PushItemWidth(w);
						ui->AddCombo(index, filenames, true);
						ImGui::PopItemWidth();

						ui->SameLine();
						ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2);
					}
				}

				//--

				if (!ui->bMinimize) ui->AddSpacingSeparated();
				//ui->AddSpacingSeparated();

				//--

				drawImGuiClicker();

				//--

				if (b && bFolder) ui->EndTree();
			}
		}

		if (bWindowed && bw)
		{
			ui->EndWindow();
		}
	}

	//----

	//TODO:
	// Files

	// 2. Presets Clicker

	void drawImGuiClicker(bool bWindowed = false, bool bMinimal = false)
	{
		bool bw = false;
		if (bWindowed) {
			bw = ui->BeginWindow(bGui);
			if (!bw) {
				ui->EndWindow();
				return;
			}
		}

		//--

		if (!bMinimal)
		{
			if (!ui->bMinimize) {
				ui->Add(bClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MINI);
				ui->AddSpacing();
			}
		}
		else
		{
			if (!bClicker) bClicker = true;
		}

		if (bClicker)
		{
			if (ui->bMinimize) ui->AddSpacingSeparated();

			float _h2 = ui->getWidgetsHeightUnit();
			_h2 *= 1.5f;

			bool bResponsiveButtonsClicker = true;

			//TODO: add public var
			bool bFlip = false;
			//bool bFlip = true;

			string toolTip = "";
			if (bKeyCtrl) toolTip = "Copy To";
			else if (bKeyAlt) toolTip = "Swap With";

			if (bUseColorizedMatrices)
			{
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, colors, bResponsiveButtonsClicker, amountButtonsPerRowClicker, true, _h2, toolTip, bFlip);
			}
			else
			{
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtonsClicker, amountButtonsPerRowClicker, true, _h2, toolTip, bFlip);
			}

			if (dir.size() > 0)
			{
				if (!bMinimal && !ui->bMinimize)
				{
					ui->AddSpacing();
					if (!ui->bMinimize) ui->Add(amountButtonsPerRowClicker, OFX_IM_STEPPER, 2);
				}
			}
		}

		//--

		if (bWindowed) {
			if (bw) {
				ui->EndWindow();
			}
		}
	}

	//--

public:

	//exposed to trig an external method
	ofParameter<void> vReset{ "Reset" };
	ofParameter<void> vRandom{ "Random" };

	ofParameter<int> index{ "Index", 0, 0, 0 };

	ofParameter<int> amountButtonsPerRowClicker{ "Amount", 1, 1, 4 };

	//--

	ofParameter<bool> bGui{ "PRESETS", true };

	void setToggleVisibleGui() { bGui = !bGui; }
	void setVisibleGui(bool b) { bGui = b; }
	bool getVisibleGui() { return bGui; }

	//--

private:

	int index_PRE = -1;//pre

	ofParameter<void> vPrevious{ "<" };
	ofParameter<void> vNext{ ">" };
	ofParameter<void> vRename{ "Rename" };
	ofParameter<void> vScan{ "Scan" };
	ofParameter<void> vDelete{ "Delete" };
	ofParameter<void> vSave{ "Save" };
	ofParameter<void> vLoad{ "Load" };
	ofParameter<void> vNew{ "New" };

	ofParameter<bool> bClicker{ "CLICKER", false };
	ofParameter<bool> bAutoSave{ "AutoSave", true };
	ofParameter<bool> bExpand{ "Expand", true };

	//public:
private:

	void setup()
	{
		ofLogNotice(__FUNCTION__);

		doRefreshFiles();//TODO:
	};

	void startup()
	{
		ofLogNotice(__FUNCTION__);

		doRefreshFiles();

		index_PRE = -1; // pre

		string s;
		if (path_Global == "") s = path_Settings;
		else s = path_Global + "/" + path_Settings;
		ofxSurfingHelpers::loadGroup(params, s);

		//index = index;

		//--

		//TODO:
		// matrix colors
		if (bUseColorizedMatrices) {
			colors.clear();
			keyCommandsChars.clear();
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
	};

	void update(ofEventArgs& args)
	{
		if (ofGetFrameNum() == 1)
			//if (ofGetFrameNum() == 0) 
		{
			startup();
		}
	};

	void draw()
	{
		//ImVec2 sz(100,30);
		//if (ofxSurfingGui::AddButton("Save", sz)) {
		//}
	};

	//--

	void doSave()
	{
		//save the previously settled name
		ofLogNotice(__FUNCTION__) << " : " << filename;

		if (bDoingNew) bDoingNew = false;

		ofxSurfingHelpers::CheckFolder(pathPresets);

		// Save Settings
		ofxSurfingHelpers::saveGroup(params_Preset, pathPresets + "/" + filename + ".json");
	};

	void doLoad()
	{
		ofLogNotice(__FUNCTION__) << " : " << filename;

		if (bDoingNew) bDoingNew = false;

		// Load Settings
		ofxSurfingHelpers::loadGroup(params_Preset, pathPresets + "/" + filename + ".json");
	};



	//void load(string path)
	//{
	//	ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);

	//	// Load Settings
	//	ofxSurfingHelpers::loadGroup(params_Preset, pathPresets + "/" + filename + ".json");
	//}


	//void doResetParams()
	//{
	//	ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);
	//	//TODO:
	//	//doResetParamsFull(RESET_PARAM_MIN);
	//	doResetParams();
	//}

	//-

public:

	void AddGroup(ofParameterGroup& group)
	{
		// make a pointer, bc maybe that = no works well..

		auto ptype = group.type();
		bool isGroup = ptype == typeid(ofParameterGroup).name();
		if (isGroup)
		{
			params_Preset = group;
		}

		string s = params_Preset.getName();
		s += ofToString("_Settings.json");
		path_Settings = s;

		setup();
	};

	void setPathGlobal(std::string path) {
		path_Global = path;
		ofxSurfingHelpers::CheckFolder(path_Global);
	};

private:
	std::string path_Global = "";//main folder where nested folder goes inside

public:
	void setPath(string p) {
		pathPresets = p + "/Presets";
		ofxSurfingHelpers::CheckFolder(pathPresets);
	};

	void setFilename(string p) {
		filename = p;
		ofLogNotice(__FUNCTION__) << " : " << filename;
	};

	void doLoadPrevious() {
		if (index > index.getMin()) index--;
		else if (index == index.getMin()) index = index.getMax();
	};

	void doLoadNext() {
		if (index < index.getMax()) index++;
		else if (index == index.getMax()) index = index.getMin();
	};

private:

	void Changed(ofAbstractParameter& e)
	{
		string name = e.getName();
		ofLogNotice(__FUNCTION__);
		ofLogNotice("ofxSurfingPresetsLite") << name << " " << e;

		if (0) {}

		//TODO: BUG: overwrites presets sometimes
		else if (name == index.getName())
		{
			if (filenames.size() == 0) return;

			//static int index_PRE = -1; // pre

			// clamp inside dir files amount limits
			index = ofClamp(index.get(), index.getMin(), index.getMax());

			// Changed?
			if (index.get() != index_PRE)
			{
				bIsChangedIndex = true;

				//TODO:
				//avoid error!
				//if (index.get() > filenames.size() - 1 || index_PRE > filenames.size() - 1) return;

				if (index_PRE != -1)
				{
					if (index_PRE < filenames.size() && index < filenames.size())
					{
						ofLogNotice("ofxSurfingPresetsLite") << "\n  Changed \n  Preset Index : "
							<< ofToString(index_PRE) << " > " << ofToString(index)
							<< "      \t(" <<
							ofToString(filenames[index_PRE]) << " > " <<
							ofToString(filenames[index]) << ")"
							<< "\n";
					}
				}

				//--

				// 1. Common Load but AutoSave

				if (!bKeyCtrl && !bKeyAlt) // Ctrl nor Alt not pressed
				{
					// Autosave

					if (bAutoSave)
					{
						if (dir.size() > 0 && index_PRE < dir.size() && index_PRE < filenames.size())
						{
							filename = filenames[index_PRE];
							doSave();
						}
						else { ofLogError("ofxSurfingPresetsLite") << "Preset Index points an out of range file!"; }
					}

					index_PRE = index;

					//--

					// Load

					ofLogNotice("ofxSurfingPresetsLite") << index.getName() + " : " << ofToString(index);

					if (dir.size() > 0 && index < dir.size() && index_PRE < filenames.size())
					{
						filename = filenames[index_PRE];
						doLoad();
					}
					else
					{
						ofLogError("ofxSurfingPresetsLite") << "File out of range";
					}
				}

				//--

				// 2. Save / Copy

				// Save to clicked preset index
				// Ctrl pressed. Alt not pressed

				else if (bKeyCtrl && !bKeyAlt)
				{
					if (index < filenames.size()) {
						filename = filenames[index];
						//filename = filenames[index_PRE];

						doSave();

						ofLogNotice("ofxSurfingPresetsLite") << "PRESET COPY!";

						index_PRE = index;
					}
				}

				//--

				//TODO:

				// 3. Swap

				// (from/to) pre/current index
				// Ctrl not pressed. Alt pressed

				else if (!bKeyCtrl && bKeyAlt)
				{
					if (dir.size() > 0 && index < filenames.size() && index_PRE < filenames.size())
					{
						// Rename target to source
						string _fFrom = filenames[index_PRE];
						string _fTo = filenames[index];
						ofFile f;

						_fTo = pathPresets + "/" + _fTo + ".json";
						_fFrom = pathPresets + "/" + _fFrom + ".json";

						bool bf = f.open(_fTo);
						bool bt = f.renameTo(_fFrom, true, true);

						// Save current to
						filename = _fTo;
						doSave();

						if (bf && bt) {
							ofLogNotice("ofxSurfingPresetsLite") << "PRESET SWAP!";
							ofLogNotice("ofxSurfingPresetsLite") << _fFrom << " <-> " << _fTo;
						}
						else {
							ofLogError("ofxSurfingPresetsLite") << "WRONG SWAP!";
						}

						index_PRE = index;
					}
				}
			}

			//----

			//TODO:
			// changed
			//if (index_PRE != index) {
			//	if (bAutoSave)
			//	{
			//		if (index_PRE < filenames.size() && index_PRE >= 0) {
			//			filename = filenames[index_PRE];
			//			doSave();
			//		}
			//	}
			//	index_PRE = index;//refresh
			//}

			//if (index < filenames.size() && index >= 0) {
			//	filename = filenames[index];
			//	doLoad();
			//}
		}

		else if (name == vLoad.getName())
		{
			doLoad();
		}

		else if (name == vReset.getName())
		{
			doResetParams();
		}

		else if (name == vRandom.getName())
		{
			doRandomizeParams();
		}

		else if (name == vSave.getName())
		{
			int num = getNumFiles();

			if (num == 0) filename = "0";

			// filename is already named
			doSave();

			// scan
			string filename_ = filename;
			doRefreshFiles();

			//if (filename_ != filename)
			if (num != getNumFiles())
			{
				ofLogNotice("ofxSurfingPresetsLite") << "Reorganize " << dir.size() << " files.";
				for (int i = 0; i < dir.size(); i++)
				{
					string n = dir.getName(i);
					if (n == filename_ + ".json")
					{
						index = i;
						break;
					}
				}
			}
		}

		else if (name == vScan.getName())
		{
			doRefreshFiles();
		}

		else if (name == vDelete.getName())
		{
			if (filenames.size() == 0) return;

			//filename = filenames[index];
			ofFile::removeFile(pathPresets + "/" + filename + ".json");
			doRefreshFiles();

			index = index;
			//doLoad();
		}

		else if (name == vRename.getName())
		{
			if (filenames.size() == 0) return;

			/*
			// remove
			//filename = filenames[index];
			ofFile::removeFile(pathPresets + "/" + filename + ".json");
			doRefreshFiles();

			// make new
			*/
		}
	};

	//--

private:

	ofParameterGroup params{ "ofxSurfingPresetsLite" };
	ofParameterGroup params_Preset{ "Preset" };

	string pathPresets = "ofxSurfingPresetsLite";

	// Files Browser
	ofDirectory dir;
	std::string fileName;
	std::string filePath;

public:

	string filename = "1";

	vector<std::string> filenames;

	//--

private:

	int getNumFiles() {
		return filenames.size();
	};

	bool doRefreshFiles()
	{
		// Load dragged images folder
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__) << "list files " << pathPresets;

		bool b = false;

		dir.listDir(pathPresets);
		dir.allowExt("JSON");
		dir.allowExt("json");
		dir.sort();

		// Log files on folder
		filenames.clear();
		for (int i = 0; i < dir.size(); i++)
		{
			ofLogNotice("ofxSurfingPresetsLite") << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

			std::string _name = "NoName"; // without ext
			auto _names = ofSplitString(dir.getName(i), ".");
			if (_names.size() > 0) {
				_name = _names[0];
			}
			filenames.push_back(_name);
		}

		index.setMin(0);

		if (dir.size() == 0) index.setMax(0);
		else
		{
			index.setMax(dir.size() - 1);
		}

		//index = index;

		//-

		b = (dir.size() > 0);
		// true if there's some file

		return b;
	};

	void doClearPresets(bool createOne = true)
	{
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);

		// Remove all files
		for (int i = 0; i < dir.size(); i++)
		{
			ofFile file = dir[i];
			file.remove();
		}
		doRefreshFiles();

		if (createOne) doNewPreset();
		if (bOverInputText) bOverInputText = false;

		index = 0;
	};

	//TODO:
	string _namePreset = "";
	bool bOverInputText = false;
	bool bDoingNew = false;
	void doNewPreset()
	{
		//only sets a name, 
		//correlated to the amount of files

		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);
		if (!bOverInputText) bOverInputText = true;

		bDoingNew = true;

		//default name
		_namePreset = "";

		//auto name counting the amount of files
		string _n = ofToString(dir.size());
		bool bAvoidOverWrite = false;

		for (int i = 0; i < dir.size(); i++)
		{
			if (_n == dir.getName(i)) bAvoidOverWrite = true;
		}

		if (!bAvoidOverWrite) _namePreset = _n;
		//TODO: should rename if file is already there!

		setFilename(_namePreset);
	};

	void doPopulatePresets(int amount = 9)
	{
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);

		doClearPresets(false);

		int _max;
		if (amount == -1)
		{
			_max = dir.size();
			//amount files on folder
		}
		else _max = amount;

		for (int i = 0; i < _max; i++)
		{
			//index = i;
			doNewPreset();
			doSave();
			doRefreshFiles();
		}

		//// Workflow
		//amountButtonsPerRowClicker.setMax(_max);
		//amountButtonsPerRowClickerMini.setMax(_max);
		//amountButtonsPerRowClicker.set(_max / 3);
		//amountButtonsPerRowClickerMini.set(_max / 3);

		// Workflow
		index = 0;
	};

	//----

	// Resets Engine
	// 
	//TODO:

private:

	/*
	enum ResetPramsType
	{
		RESET_PARAM_MIN = 0,
		RESET_PARAM_FULL_CENTER,
		RESET_PARAM_RANGE_CENTER,
		RESET_PARAM_MAX,
		RESET_RANGE_MIN,
		RESET_RANGE_MAX,
	};

	//--------------------------------------------------------------
	void doResetParamsFull(ResetPramsType MS_type) {
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);


		for (int i = 0; i < params_Preset.size(); i++)
			//for (auto p : editorEnablers)
		{
			auto p = true;//only reset this iterated param if it's enabled

			//-

			//std::string name = p.getName();//name
			std::string name = params_Preset[i].getName();//name
			//auto& g = params_Preset.getGroup(name);//ofParameterGroup
			//auto& g = params_Preset.getGroup(name);//ofParameterGroup
			auto& g = params_Preset;//ofParameterGroup
			auto& e = g.get(name);//ofAbstractParameter
			//auto& e = params_Preset.get(name);//ofAbstractParameter

			auto type = e.type();
			bool isFloat = type == typeid(ofParameter<float>).name();
			bool isInt = type == typeid(ofParameter<int>).name();

			bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
			bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
			bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

			if (isFloat)
			{
				auto pmin = g.getFloat("Min").get();
				auto pmax = g.getFloat("Max").get();
				ofParameter<float> p0 = e.cast<float>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
			}

			else if (isInt)
			{
				auto pmin = g.getInt("Min").get();
				auto pmax = g.getInt("Max").get();
				ofParameter<int> p0 = e.cast<int>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
			}

			else if (isVec2)
			{
				auto pmin = g.getVec2f("Min").get();
				auto pmax = g.getVec2f("Max").get();
				ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 2; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.getMin().x, p0.get().y));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.getMax().x - p0.getMin().x / 2, p0.get().y));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(pmax.x - pmin.x / 2, p0.get().y));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(pmin.x, p0.get().y));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(pmax.x, p0.get().y));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.get().x, p0.getMin().y));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.get().x,p0.getMax().y - p0.getMin().y / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(p0.get().x,pmax.y - pmin.y / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(p0.get().x, pmin.y));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(p0.get().x, pmax.y));//reset to range max
				//	}
				//}
			}
			else if (isVec3)
			{
				auto pmin = g.getVec3f("Min").get();
				auto pmax = g.getVec3f("Max").get();
				ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 3; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.getMin().x, p0.get().y, p0.get().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.getMax().x - p0.getMin().x / 2, p0.get().y, p0.get().z));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(pmax.x - pmin.x / 2, p0.get().y, p0.get().z));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(pmin.x, p0.get().y, p0.get().z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(pmax.x, p0.get().y, p0.get().z));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.getMin().y, p0.getMin().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.getMax().y - p0.getMin().y / 2, p0.getMin().z));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, pmax.y - pmin.y / 2, p0.getMin().z));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.getMin().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, pmin.y, p0.getMin().z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, pmax.y, p0.getMin().z));//reset to range max
				//	}
				//	else if (dim == 2) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMin().z));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMax().z - p0.getMin().z / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z - pmin.z / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, pmin.z));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z));//reset to range max
				//	}
				//}
			}
			else if (isVec4)
			{
				auto pmin = g.getVec4f("Min").get();
				auto pmax = g.getVec4f("Max").get();
				ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();

				if (0) {}
				else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
				else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
				else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
				else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
				else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
				else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

				//for (int dim = 0; dim < 4; dim++) {
				//	if (dim == 0) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.getMin().x, p0.get().y, p0.get().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4((p0.getMax().x - p0.getMin().x) / 2, p0.get().y, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4((pmax.x - pmin.x) / 2, p0.get().y, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(pmin.x, p0.get().y, p0.get().z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(pmax.x, p0.get().y, p0.get().z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 1) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.getMin().y, p0.get().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, (p0.getMax().y - p0.getMin().y) / 2, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, (pmax.y - pmin.y) / 2, p0.get().z, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, pmin.y, p0.get().z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, pmax.y, p0.get().z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 2) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.getMin().z, p0.get().w));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (p0.getMax().z - p0.getMin().z) / 2, p0.get().w));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (pmax.z - pmin.z) / 2, p0.get().w));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, pmin.z, p0.get().w));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, pmax.z, p0.get().w));//reset to range max
				//	}
				//	else if (dim == 3) {
				//		if (0) {}
				//		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMin()));//reset to param min
				//		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (p0.getMax().w - p0.getMin().w) / 2));
				//		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (pmax.w - pmin.w) / 2));
				//		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMax().w));//reset to param max
				//		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmin));//reset to range min
				//		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmax));//reset to range max
				//	}
				//}
			}
		}
	}
	*/

	// Random simple
	//--------------------------------------------------------------
	void doRandomizeParams(bool bSilent = false) {
		ofLogNotice(__FUNCTION__);

		//TODO:
		// this is not recursive inside the group content!
		// get solution from ImHelpers. AddGroup iterate groups

		for (int i = 0; i < params_Preset.size(); i++)
		{
			auto& p = params_Preset[i];

			if (p.type() == typeid(ofParameter<float>).name())
			{
				ofParameter<float> pr = p.cast<float>();
				float v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent) pr.setWithoutEventNotifications(v);
				else pr.set(v);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			else if (p.type() == typeid(ofParameter<int>).name())
			{
				ofParameter<int> pr = p.cast<int>();
				int v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent) pr.setWithoutEventNotifications(v);
				else pr.set(v);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name())
			{
				ofParameter<bool> pr = p.cast<bool>();
				bool b = (ofRandom(1.f) >= 0.5f);
				if (bSilent) pr.setWithoutEventNotifications(b);
				else pr.set(b);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
		}
	};

	// Reset Simple
	//--------------------------------------------------------------
	void doResetParams(bool bSilent = false) {
		ofLogNotice(__FUNCTION__);

		for (int i = 0; i < params_Preset.size(); i++)
		{
			auto& p = params_Preset[i];

			if (p.type() == typeid(ofParameter<float>).name())
			{
				ofParameter<float> pr = p.cast<float>();
				if (bSilent) pr.setWithoutEventNotifications(pr.getMin());
				else pr.set(pr.getMin());
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();

			}

			else if (p.type() == typeid(ofParameter<int>).name())
			{
				ofParameter<int> pr = p.cast<int>();
				if (bSilent) pr.setWithoutEventNotifications(pr.getMin());
				else pr.set(pr.getMin());
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name())
			{
				ofParameter<bool> pr = p.cast<bool>();
				bool b = false;
				if (bSilent) pr.setWithoutEventNotifications(b);
				else pr.set(b);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
		}

		//if (!bSilent) bIsRetrigged = true;
	};

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs& eventArgs)
	{
		const int& key = eventArgs.key;
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__) << (char)key << " [" << key << "]";

		// Modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (mod_CONTROL) bKeyCtrl = true;
		if (mod_ALT) bKeyAlt = true;
	};

	//--------------------------------------------------------------
	void keyReleased(ofKeyEventArgs& eventArgs)
	{
		const int& key = eventArgs.key;
		ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__) << (char)key << " [" << key << "]";

		// Modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (!mod_CONTROL) bKeyCtrl = false;
		if (!mod_ALT) bKeyAlt = false;

		//if (!bKeys) return;
	};
	//--------------------------------------------------------------
	void addKeysListeners()
	{
		ofAddListener(ofEvents().keyPressed, this, &ofxSurfingPresetsLite::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &ofxSurfingPresetsLite::keyReleased);
	};
	//--------------------------------------------------------------
	void removeKeysListeners()
	{
		ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresetsLite::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &ofxSurfingPresetsLite::keyReleased);
	};

	// easy callback to know when the preset index/selector changed
private:
	bool bIsChangedIndex = false;
public:
	bool isChangedIndex() {
		if (bIsChangedIndex) {
			bIsChangedIndex = false;
			return true;
		}
		else return false;
	};
};

