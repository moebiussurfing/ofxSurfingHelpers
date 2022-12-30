#pragma once

#include "ofMain.h"

/*

	Converts an int ofParam to a vector of bool toggles.
	Also shows ofxSurfingImGui helpers
	to populate matrix buttons linked to that int.

*/


//--------------------------------------------------------------


class surfingParamIndexToggles
{
public:

	//--------------------------------------------------------------
	surfingParamIndexToggles() {
	};

	//--------------------------------------------------------------
	~surfingParamIndexToggles()
	{
		bToggles.clear();
		params_Toggles.clear();

		ofRemoveListener(params_Toggles.parameterChangedE(), this, &surfingParamIndexToggles::Changed_Toggles);
	};

private:

	ofEventListener listenerIndex;

public:

	//--------------------------------------------------------------
	void setup(ofParameter<int>& _i) // pass the master int param!
	{
		index.makeReferenceTo(_i); // link

		params_Toggles.setName(_i.getName());
		doBuildToggles();

		listenerIndex = index.newListener([this](int i) {
			refreshTogglesByIndex();
			});
	}

public:

	ofParameterGroup params_Toggles{ "Toggles" };

private:

	ofParameter<int> index; // Current selected preset index
	vector<ofParameter<bool>> bToggles;

	//bool bDISABLE_CALLBACKS = false;

	//--------------------------------------------------------------
	void refreshTogglesByIndex()
	{
		//if (bDISABLE_CALLBACKS) return;

		ofLogVerbose("surfingParamIndexToggles") << "::" << index.getName() << " : " << index.get();

		// Exclusive
		// Sets to true the respective toggle 
		// for current index and set to false for the others.

		for (int i = 0; i <= index.getMax() && i < bToggles.size(); i++)
		{
			if (i > bToggles.size() - 1) break;

			bToggles[i].set(false);
		}
		if (index <= index.getMax() && index < bToggles.size())
		{
			if (index <= bToggles.size() - 1) bToggles[index].set(true);
		}
	}

public:

	// To select index preset using bool toggle parameters triggers!
	//--------------------------------------------------------------
	ofParameterGroup& getParamToggles()
	{
		return params_Toggles;
	}

private:

	//--------------------------------------------------------------
	void doBuildToggles()
	{
		bToggles.clear();
		params_Toggles.clear();

		for (int i = 0; i <= index.getMax(); i++)
		{
			std::string n = index.getName();
			//std::string n = "P";
			//std::string n = "Preset ";

			//n += ofToString(i < 10 ? "0" : "");
			n += "_";
			n += ofToString(i);

			ofParameter<bool> b{ n, false };
			bToggles.push_back(b);
			params_Toggles.add(b);
		}

		ofRemoveListener(params_Toggles.parameterChangedE(), this, &surfingParamIndexToggles::Changed_Toggles);

		ofAddListener(params_Toggles.parameterChangedE(), this, &surfingParamIndexToggles::Changed_Toggles);
	}

	//--------------------------------------------------------------
	void Changed_Toggles(ofAbstractParameter& e)
	{
		//if (bDISABLE_CALLBACKS) return;

		std::string name = e.getName();

		bool bFoundTrue = false;

		// optimize a bit the callbacks
		static bool bAttending = false;
		if (bAttending) return;
		
		for (int i = 0; i <= index.getMax() && i < bToggles.size(); i++)
		{
			if (name == bToggles[i].getName())
			{
				bAttending = true;

				// if true
				if (bToggles[i].get())
				{
					index = i;
					ofLogNotice("surfingParamIndexToggles") << (__FUNCTION__) << " " << name << " : TRUE";
					bFoundTrue = true;

					continue;
				}
				else
				{
					// avoid that all are disabled!
					for (int j = 0; j <= index.getMax() && j < bToggles.size(); j++)
					{
						if (bToggles[j].get()) {

							bAttending = false;
							return;
							// if one is true return!
						}
					}

					// here none is true. all are false
					{
						// restore to who changed back to true
						bToggles[index.get()] = true;

						bAttending = false;
						return;
					}
				}
			}
		}
		if (!bFoundTrue) return; // Not any detected note true

		//--

		// Make exclusive. All others to false
		for (int i = 0; i <= index.getMax() && i < bToggles.size(); i++)
		{
			if (i != index && bToggles[i].get())
			{
				bToggles[i] = false;
			}
		}

		bAttending = false;
	}

};
