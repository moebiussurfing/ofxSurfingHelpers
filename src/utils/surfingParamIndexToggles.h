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

		params_Toggles.clear();
		params_Toggles.setName(_i.getName());
		doBuildToggles();

		//--

		// callback

		listenerIndex = index.newListener([this](int i) {
			//listenerIndex = index.newListener([this](int i) {

		//	//TODO:
		//	// filter a bit to ignore if not really changed.
		//	// retrigger mode should be considerated!
		//	static int index_PRE = -1;
		//if (index.get() != index_PRE) {
		//	index_PRE = index.get();
		//}
		//else {//not changed
		//	ofLogVerbose("surfingParamIndexToggles") << "SKIP " << index.getName();
		//	return;
		//}

			refreshTogglesByIndex();

			});
	}

public:

	ofParameterGroup params_Toggles{ "Toggles" };

protected:

	ofParameter<int> index; // Current selected preset index

private:

	vector<ofParameter<bool>> bToggles;

	//bool bDISABLE_CALLBACKS = false;

	//--------------------------------------------------------------
	void refreshTogglesByIndex()
	{
		//if (bDISABLE_CALLBACKS) return;

		ofLogVerbose("surfingParamIndexToggles") << "::" << index.getName() << " : " << index.get();

		////TODO:
		//// filter a bit to ignore if not really changed.
		//// re trigger mode should be considered!
		//static int index_PRE = -1;
		//if (index.get() != index_PRE) {
		//	index_PRE = index.get();
		//}
		//else {//not changed
		//	ofLogVerbose("surfingParamIndexToggles") << "SKIP " << index.getName();
		//	return;
		//}

		//--

		// Exclusive
		// Sets to true the respective toggle 
		// for the current changed index 
		// and then set to false for all the others.

		for (int i = 0; i <= index.getMax() && i < bToggles.size(); i++)
		{
			if (bToggles[i].get()) bToggles[i].set(false);
		}
		if (index <= index.getMax() && index < bToggles.size())
		{
			if (!bToggles[index].get()) bToggles[index].set(true);
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
					if (i != index) index = i;

					ofLogNotice("surfingParamIndexToggles") << " " << name << " : TRUE";
					bFoundTrue = true;

					continue;
				}
				else // is false
				{
					// Avoid that all are disabled!

					for (int j = 0; j <= index.getMax() && j < bToggles.size(); j++)
					{
						if (bToggles[j].get()) {//one is already true

							bAttending = false;
							return;
							// if at least one is true, we return!
						}
					}

					// If arrived here: none is true. all are false
					{
						// restore to who changed back to true

						// TODO:
						//avoid crash
						if (bToggles.size() <= 1) {
							bAttending = false;
							return;
						}

						//bToggles[index.get()] = true;
						if (bToggles[index.get() == false]) bToggles[index.get()] = true;//crash?

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
				if (bToggles[i].get()) bToggles[i] = false;
			}
		}

		bAttending = false;
	}

};
