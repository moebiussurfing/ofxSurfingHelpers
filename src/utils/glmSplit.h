#pragma once

#include "ofMain.h"

/*
	This classes can split a glm:vec (2, 3 and 4 dims) ofParameter to the float elements ofParams.
	And the multidim glm params will be auto linked to the float parts.
	All is autoupdated and is public exposed to be populated into a gui,
	with the elements separated.
	Surely is a dirty approach, ald could be better done.
	by moebiusSurfing.

*/

/*

// Example
// ofApp.h
#pragma once

#include "ofMain.h"

#include "glmSplit.h"
#include "ofxGui.h"

//--

class ofApp : public ofBaseApp {

public:

	ofxPanel gui;
	ofxPanel guiSplitter;

	ofParameter<glm::vec2> v2{ "myVec2", glm::vec2(0.25), glm::vec2(0), glm::vec2(1) };
	ofParameter<glm::vec3> v3{ "myVec3", glm::vec3(0.25), glm::vec3(0), glm::vec3(1) };

	glmSplitter2 sp_v2;
	glmSplitter3 sp_v3;

	void setup() {
		sp_v2.setup(v2);
		sp_v3.setup(v3);

		gui.setup("glm:vec");
		gui.setPosition(10, 10);
		gui.add(v2);
		gui.add(v3);

		guiSplitter.setup("glmsplit:float");
		guiSplitter.setPosition(300, 10);
		guiSplitter.add(sp_v2.floats);
		guiSplitter.add(sp_v3.floats);
	};

	void draw() {
		gui.draw();
		guiSplitter.draw();
	};
};

*/

class glmSplitter4
{
public:
	glmSplitter4() {};
	~glmSplitter4() {};

private:
	ofEventListener listenerParams;

public:
	ofParameter<float> x{ "x", 0, 0, 1 };
	ofParameter<float> y{ "y", 0, 0, 1 };
	ofParameter<float> z{ "z", 0, 0, 1 };
	ofParameter<float> w{ "w", 0, 0, 1 };

private:
	ofParameterGroup params{ "params" };

public:
	ofParameterGroup floats{ "Floats" };

	ofParameter<glm::vec4> vecRef{ "-1",glm::vec4(0.5),glm::vec4(0),glm::vec4(1) };

public:
	void set(ofParameter<glm::vec4> _v) {
		x.set(_v.get().x);
		y.set(_v.get().y);
		z.set(_v.get().z);
		w.set(_v.get().w);

		// limits
		x.setMin(_v.getMin().x);
		y.setMin(_v.getMin().y);
		z.setMin(_v.getMin().z);
		w.setMin(_v.getMin().w);
		x.setMax(_v.getMax().x);
		y.setMax(_v.getMax().y);
		z.setMax(_v.getMax().z);
		w.setMax(_v.getMax().w);
	}

	void setup(ofParameter<glm::vec4> &_v) {
		vecRef.makeReferenceTo(_v);

		params.setName(_v.getName());
		params.add(vecRef);

		params.add(x);
		params.add(y);
		params.add(z);
		params.add(w);

		floats.setName(_v.getName());
		floats.add(x);
		floats.add(y);
		floats.add(z);
		floats.add(w);

		setup();
	}

private:
	void setup() {

		listenerParams = params.parameterChangedE().newListener([&](ofAbstractParameter&p) {
			string name = p.getName();

			static bool bdisable = false;

			if (name == "-1")
			{
				return;
			}

			if (name == vecRef.getName()) {
				x.setWithoutEventNotifications(vecRef.get().x);
				y.setWithoutEventNotifications(vecRef.get().y);
				z.setWithoutEventNotifications(vecRef.get().z);
				w.setWithoutEventNotifications(vecRef.get().w);

				x = x;
				y = y;
				z = z;
				w = w;
			}

			else {
				if (p.type() == typeid(ofParameter<float>).name())
				{
					ofParameter<float> pm = p.cast<float>();
					ofLogNotice() << "glmSplitter : " << name << " : " << pm.get();
				}
				else { return; }

				if (name == "x")
				{
					vecRef.set(glm::vec4(x, y, z, w));
				}
				else if (name == "y")
				{
					vecRef.set(glm::vec4(x, y, z, w));
				}
				else if (name == "z")
				{
					vecRef.set(glm::vec4(x, y, z, w));
				}
				else if (name == "w")
				{
					vecRef.set(glm::vec4(x, y, z, w));
				}
			}
		});

		ofLogNotice() << __FUNCTION__;
	};
};

//-

class glmSplitter3
{
public:
	glmSplitter3() {};
	~glmSplitter3() {};

private:
	ofEventListener listenerParams;

public:
	ofParameter<float> x{ "x", 0, 0, 1 };
	ofParameter<float> y{ "y", 0, 0, 1 };
	ofParameter<float> z{ "z", 0, 0, 1 };

private:
	ofParameterGroup params{ "params" };

public:
	ofParameterGroup floats{ "Floats" };

	ofParameter<glm::vec3> vecRef{ "-1",glm::vec3(0.5),glm::vec3(0),glm::vec3(1) };

public:
	void set(ofParameter<glm::vec3> _v) {
		x.set(_v.get().x);
		y.set(_v.get().y);
		z.set(_v.get().z);

		// limits
		x.setMin(_v.getMin().x);
		y.setMin(_v.getMin().y);
		z.setMin(_v.getMin().z);
		x.setMax(_v.getMax().x);
		y.setMax(_v.getMax().y);
		z.setMax(_v.getMax().z);
	}

	void setup(ofParameter<glm::vec3> &_v) {
		vecRef.makeReferenceTo(_v);

		params.setName(_v.getName());
		params.add(vecRef);

		params.add(x);
		params.add(y);
		params.add(z);

		floats.setName(_v.getName());
		floats.add(x);
		floats.add(y);
		floats.add(z);

		setup();
	}

private:
	void setup() {

		listenerParams = params.parameterChangedE().newListener([&](ofAbstractParameter&p) {
			string name = p.getName();

			static bool bdisable = false;

			if (name == "-1")
			{
				return;
			}

			if (name == vecRef.getName()) {
				x.setWithoutEventNotifications(vecRef.get().x);
				y.setWithoutEventNotifications(vecRef.get().y);
				z.setWithoutEventNotifications(vecRef.get().z);

				x = x;
				y = y;
				z = z;
			}

			else {
				if (p.type() == typeid(ofParameter<float>).name())
				{
					ofParameter<float> pm = p.cast<float>();
					ofLogNotice() << "glmSplitter : " << name << " : " << pm.get();
				}
				else { return; }

				if (name == "x")
				{
					vecRef.set(glm::vec3(x, y, z));
				}
				else if (name == "y")
				{
					vecRef.set(glm::vec3(x, y, z));
				}
				else if (name == "z")
				{
					vecRef.set(glm::vec3(x, y, z));
				}
			}
		});

		ofLogNotice() << __FUNCTION__;
	};
};


//-


class glmSplitter2
{
public:
	glmSplitter2() {};
	~glmSplitter2() {};

private:
	ofEventListener listenerParams;

public:
	ofParameter<float> x{ "x", 0, 0, 1 };
	ofParameter<float> y{ "y", 0, 0, 1 };

private:
	ofParameterGroup params{ "params" };

public:
	ofParameterGroup floats{ "Floats" };
	
	ofParameter<glm::vec2> vecRef{ "-1",glm::vec2(0.5),glm::vec2(0),glm::vec2(1) };

public:
	void set(ofParameter<glm::vec2> _v) {
		x.set(_v.get().x);
		y.set(_v.get().y);

		// limits
		x.setMin(_v.getMin().x);
		y.setMin(_v.getMin().y);
		x.setMax(_v.getMax().x);
		y.setMax(_v.getMax().y);
	}

	void setup(ofParameter<glm::vec2> &_v) {
		vecRef.makeReferenceTo(_v);

		params.setName(_v.getName());
		params.add(vecRef);

		params.add(x);
		params.add(y);

		floats.setName(_v.getName());
		floats.add(x);
		floats.add(y);

		setup();
	}

private:
	void setup() {

		listenerParams = params.parameterChangedE().newListener([&](ofAbstractParameter&p) {
			string name = p.getName();

			static bool bdisable = false;

			if (name == "-1")
			{
				return;
			}

			if (name == vecRef.getName()) {
				x.setWithoutEventNotifications(vecRef.get().x);
				y.setWithoutEventNotifications(vecRef.get().y);

				x = x;
				y = y;
			}

			else {
				if (p.type() == typeid(ofParameter<float>).name())
				{
					ofParameter<float> pm = p.cast<float>();
					ofLogNotice() << "glmSplitter : " << name << " : " << pm.get();
				}
				else { return; }

				// get the values doing casting. we check the type first
				if (name == "x")
				{
					vecRef.set(glm::vec2(x, y));
				}
				else if (name == "y")
				{
					vecRef.set(glm::vec2(x, y));
				}
				else if (name == "z")
				{
					vecRef.set(glm::vec2(x, y));
				}
			}
		});

		ofLogNotice() << __FUNCTION__;
	};
};