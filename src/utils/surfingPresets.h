#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
#include "imgui_stdlib.h"
//#include "surfingParamsRandom.h"

/*

	Simple Presets Manager

*/

//----

/*
	TODO:

	add settings state for gui
	add auto save
	add rename preset
	add copy/swap?
	add save files
	search file listers from surfingPresets
*/

//----

class SurfingPresets
{

public:

	SurfingPresets::SurfingPresets()
	{
		ofAddListener(params.parameterChangedE(), this, &SurfingPresets::Changed);
		ofAddListener(ofEvents().update, this, &SurfingPresets::update);

		params.add(vScan);
		params.add(vDelete);
		params.add(vSave);
		params.add(vLoad);
		params.add(vNew);
		params.add(vRename);
		params.add(vReset);
		params.add(index);
		params.add(bAutoSave);
		params.add(bClicker);
		params.add(bExpand);
	};

	SurfingPresets::~SurfingPresets()
	{
		ofRemoveListener(params.parameterChangedE(), this, &SurfingPresets::Changed);
		ofRemoveListener(ofEvents().update, this, &SurfingPresets::update);

		ofxSurfingHelpers::CheckFolder(pathSettings);
		ofxSurfingHelpers::saveGroup(params);

		doSave();
	};


private:
	ofxSurfingGui* ui;

public:
	void setUiPtr(ofxSurfingGui* _ui) {
		ui = _ui;
	}

private:
	string pathSettings = "ofApp";

	//----

public:

	// Presets

	void drawImGui(bool bWindowed = false)
	{
		//TODO:
		// make windowed
		bool b = true;;

		if (bWindowed) {
			b = ui->BeginWindow("Presets");
		}

		if (b)
		{
			//TODO:
			static string _namePreset = "";
			//static bool bTyping = false;
			static bool bInputText = false;
			//string s = "presetName";
			string s = filename;

			static bool bFolder = true;

			bool b;
			if (!bFolder) ui->AddLabelBig("PRESETS", true, true);
			else b = ui->BeginTree("PRESETS");
			if (b) {
				ui->Add(bExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
				//ui->AddToggle("Expand", bExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
				//if (!ui->bMinimize)
				if (bExpand)
				{
					ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);

					if (ui->Add(vSave, OFX_IM_BUTTON_SMALL, 2))
					{
						bInputText = false;
						_namePreset = s;
					};

					if (ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2, true))
					{
						if (!bInputText) bInputText = true;
						_namePreset = "";
						setFilename(_namePreset);
					};

					ui->Add(vReset, OFX_IM_BUTTON_SMALL, 2);

					if (ui->Add(vRename, OFX_IM_BUTTON_SMALL, 2, true)) {
						//delete
						vDelete.trigger();
						//create new
						if (!bInputText) bInputText = true;
						_namePreset = "";
						setFilename(_namePreset);
					};

					ui->Add(vDelete, OFX_IM_BUTTON_SMALL, 2);
					ui->Add(vScan, OFX_IM_BUTTON_SMALL, 2, true);
					ui->Add(bAutoSave, OFX_IM_TOGGLE_SMALL, 2);

					//--

					ui->AddSpacing();

					if (bInputText)
					{
						int _w = ui->getWidgetsWidth() * 0.9f;
						ImGui::PushItemWidth(_w);
						{
							bool b = ImGui::InputText("##NAME", &s);
							if (b) {
								ofLogNotice("WaveformPlot") << "InputText:" << s.c_str();
								setFilename(s);
							}
						}

						ImGui::PopItemWidth();
					}
				}
				else {
					ui->Add(vSave, OFX_IM_BUTTON_SMALL);
				}

				//--

				ui->AddSpacingSeparated();

				// Combo
				ui->AddComboButtonDual(index, filenames, true);

				if (ui->AddButton("NEXT", OFX_IM_BUTTON_SMALL))
				{
					doLoadNext();
				};

				//ui->Add(bClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED);

				//--

				ui->AddSpacingSeparated();

				drawImGuiClicker();

				/*
				if (!ui->bMinimize)
				{
					// preset name
					if (_namePreset != "") ui->AddLabel(_namePreset.c_str());
				}
				*/

				//--

				if (b) ui->EndTree();
			}
		}

		if (bWindowed && b) {
			ui->EndWindow();
		}
	}


	////--

	//TODO:
	// Files

	// 2. Presets
	void drawImGuiClicker(bool bWindowed = false)
	{
		ui->Add(bClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		if (bClicker)
		{
			float h = ui->getWidgetsHeightUnit();
			ofxImGuiSurfing::AddMatrixClickerLabelsStrings(index, filenames, true, 1, true, h);

			/*
			inline bool AddMatrixClickerLabelsStrings(ofParameter<int>&_index,
			const std::vector<string> labels, bool bResponsive = true, int amountBtRow = 3, const bool bDrawBorder = true, float __h = -1, bool bSpaced = true, string toolTip = "")
			*/

			/*
			ui->Indent();
			{
				// Paths
				{
					bool bOpen = false;
					ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
					//_flagt |= ImGuiTreeNodeFlags_Framed;

					if (ImGui::TreeNodeEx("Path", _flagt))
					{
						ImGui::TextWrapped(pathPresets.data()); // -> show path
						ImGui::TreePop();
					}
				}

				// Files
				// Buttons Selector for each file
				if (ofxImGuiSurfing::filesPicker(pathPresets, filename, index, { "json" }))
				{
					// Buttons Matrix

					//TODO:
					// Index back not working
					// this is a workaround
					// could fail on macOS/Linux -> requires fix paths slashes

					for (int i = 0; i < dir.size(); i++)
					{
						std::string si = ofToString(i);
						if (i < 10) si = "0" + si;
						std::string ss = name_Root + "_" + si;
						fileName = ss;

						auto s0 = ofSplitString(nameSelected, "\\", true);
						std::string s1 = s0[s0.size() - 1]; // filename
						auto s = ofSplitString(s1, ".json");

						std::string _nameSelected = s[0];

						if (_nameSelected == fileName)
						{
							index = i;
						}
					}

					ofLogNotice(__FUNCTION__) << "Picked file " << nameSelected << " > " << index;
				}
			}
			ui->Unindent();
			*/
		}
	}

	//--

public:

	ofParameter<void> vReset{ "Reset" };//exposed to trig an external method
	ofParameter<int> index{ "Index", 0, 0, 0 };

private:

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
		ofLogNotice("SurfingPresets") << (__FUNCTION__);

		doRefreshFiles();//TODO:
	}

	void startup()
	{
		ofLogNotice("SurfingPresets") << (__FUNCTION__);

		doRefreshFiles();

		ofxSurfingHelpers::loadGroup(params);

		//index = index;
	}

	void update(ofEventArgs& args)
	{
		if (ofGetFrameNum() == 0) {
			startup();
		}
	}

	void draw()
	{
		//ImVec2 sz(100,30);
		//if (ofxSurfingGui::AddButton("Save", sz)) {
		//}
	}

	//--

	void doSave()
	{
		ofLogNotice("SurfingPresets") << (__FUNCTION__);

		ofxSurfingHelpers::CheckFolder(pathPresets);
		ofxSurfingHelpers::saveGroup(paramsPreset, pathPresets + "/" + filename + ".json");
	}

	void doLoad()
	{
		ofLogNotice("SurfingPresets") << (__FUNCTION__);

		// Load Settings
		ofxSurfingHelpers::loadGroup(paramsPreset, pathPresets + "/" + filename + ".json");
	}

	void doReset()
	{
		ofLogNotice("SurfingPresets") << (__FUNCTION__);

		//TODO:
		//doResetParamsFull(RESET_PARAM_MIN);
	}

	//-

public:

	void AddGroup(ofParameterGroup& group)
	{
		// make a pointer, bc maybe that = no works well..

		auto ptype = group.type();
		bool isGroup = ptype == typeid(ofParameterGroup).name();
		if (isGroup)
		{
			paramsPreset = group;
		}

		setup();
	}

	void setPath(string p) {
		pathPresets = p + "/Presets";
	}

	void setFilename(string p) {
		filename = p;
	}

	void doLoadNext() {
		if (index < index.getMax()) index++;
		else if (index == index.getMax()) index = index.getMin();
	}

private:

	void Changed(ofAbstractParameter& e)
	{
		string name = e.getName();
		ofLogNotice("SurfingPresets") << (__FUNCTION__);
		ofLogNotice("SurfingPresets") << name << " " << e;

		if (0) {}

		else if (name == index.getName())
		{
			if (filenames.size() == 0) return;

			static int _index;
			if (_index != index) {//changed
				if (bAutoSave)
				{
					if (_index < filenames.size()) {
						filename = filenames[_index];
						doSave();
					}
				}
				_index = index;
			}

			if (_index < filenames.size()) {
				filename = filenames[index];
				doLoad();
			}
		}

		else if (name == vLoad.getName())
		{
			doLoad();
		}

		else if (name == vReset.getName())
		{
			doReset();
		}

		else if (name == vSave.getName())
		{
			int num = getNumFiles();

			doSave();

			// scan
			string filename_ = filename;
			doRefreshFiles();

			//if (filename_ != filename)
			if (num != getNumFiles())
			{
				ofLogNotice("SmoothChannel") << (__FUNCTION__) << "Reorganize " << dir.size() << " files.";
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

	ofParameterGroup params{ "SurfingPresets" };
	ofParameterGroup paramsPreset{ "Preset" };

	string pathPresets = "SurfingPresets";

	// Files Browser
	ofDirectory dir;
	std::string fileName;
	std::string filePath;

public:

	string filename = "presetName";

	vector<std::string> filenames;

	//--

private:

	int getNumFiles() {
		return filenames.size();
	}

	bool doRefreshFiles()
	{
		// Load dragged images folder
		ofLogNotice("SurfingPresets") << (__FUNCTION__) << "list files " << pathPresets;

		bool b = false;

		dir.listDir(pathPresets);
		dir.allowExt("JSON");
		dir.allowExt("json");
		dir.sort();

		// Log files on folder
		filenames.clear();
		for (int i = 0; i < dir.size(); i++)
		{
			ofLogNotice("SurfingPresets") << (__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

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
	}

	//--

	// Resets
	//TODO:
private:

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
		ofLogNotice(__FUNCTION__);


		for (int i = 0; i < paramsPreset.size(); i++)
			//for (auto p : editorEnablers)
		{
			auto p = true;//only reset this iterated param if it's enabled

			//-

			//std::string name = p.getName();//name
			std::string name = paramsPreset[i].getName();//name
			//auto& g = paramsPreset.getGroup(name);//ofParameterGroup
			//auto& g = paramsPreset.getGroup(name);//ofParameterGroup
			auto& g = paramsPreset;//ofParameterGroup
			auto& e = g.get(name);//ofAbstractParameter
			//auto& e = paramsPreset.get(name);//ofAbstractParameter

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
};

