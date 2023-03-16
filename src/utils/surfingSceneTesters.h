#pragma once

#include "ofMain.h"

namespace ofxSurfingHelpersT {//T to avoid conflicts

	/* Return a linear value in range [0,1] every delay (in seconds). */
	inline float Tick(float delay = 1.0f) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}
	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	inline float Bounce(float delay = 1.0f) {
		return 0.5f * (1.0 + glm::sin(ofxSurfingHelpersT::Tick(delay) * glm::two_pi<float>()));
	}
	/* Noise function used by the gradient scaling. */
	inline float Noise(const ofPoint& vertex = ofPoint(1, -1)) {
		return ofNoise(0.05f * vertex + 0.5f * ofGetElapsedTimeMillis() * 0.002f);
	}
}

namespace ofxSurfingHelpers {

	inline string getTextRandom() {
		string s = "";
		static int ilast = -1;
		float r = ofRandom(1.f);
		if (r < 0.25f && ilast != 0) {
			s = "Hello people, how are you today?";
			if (ofRandom(1) < 0.5) s += " And current frame is " + ofToString(ofGetFrameNum());
			ilast = 0;
		}
		else if (r < 0.5f && ilast != 1) {
			s = "Hello dude. Ready to wake up?";
			ilast = 1;
		}
		else if (r < 0.75f && ilast != 2) {
			s = "I go sleep now. Ready to wake up!";
			if (ofRandom(1) < 0.5) s += " Time is " + ofToString(ofGetTimestampString());
			ilast = 2;
		}
		else if (ilast != 3) {
			s = "Hey, hello! Im ready to go out";
			if (ofRandom(1) < 0.5) s += " Current time is " + ofToString(ofGetTimestampString());
			ilast = 3;
		}
		else {
			s = "How are you tonight? Bye bye people";
			ilast = 4;
		}

		return s;
	}
	inline string getRandomText() { return getTextRandom(); };//legacy

	// Helper to feed a Log or Notifier
	// get a random text message but bundled with a logLeve
	struct logData
	{
		ofLogLevel log;
		std::string text;
	};

	inline logData getRandomLogData() {
		logData d;
		string s = "";
		static int ilast = -1;
		float r = ofRandom(1.f);
		if (r < 0.25f && ilast != 0) {
			s = "Hello people, how are you today?";
			if (ofRandom(1) < 0.5) s += " And current frame is " + ofToString(ofGetFrameNum());
			ilast = 0;
			d.text = s;
			d.log = OF_LOG_VERBOSE;
		}
		else if (r < 0.5f && ilast != 1) {
			s = "Hello dude. Ready to wake up?";
			ilast = 1;
			d.text = s;
			d.log = OF_LOG_NOTICE;
		}
		else if (r < 0.75f && ilast != 2) {
			s = "I go sleep now. Ready to wake up!";
			if (ofRandom(1) < 0.5) s += " Time is " + ofToString(ofGetTimestampString());
			ilast = 2;
			d.text = s;
			d.log = OF_LOG_WARNING;
		}
		else if (ilast != 3) {
			s = "Hey, hello! Im ready to go out";
			if (ofRandom(1) < 0.5) s += " Current time is " + ofToString(ofGetTimestampString());
			ilast = 3;
			d.text = s;
			d.log = OF_LOG_ERROR;
		}
		else {
			s = "How are you tonight? Bye bye people";
			ilast = 4;
			d.text = s;
			d.log = OF_LOG_FATAL_ERROR;
		}

		return d;
	}
}

class surfingSceneTesters
{
public:

	ofParameter<int> amount{ "Amount", 10, 1, 100 };
	ofParameter<float> scale{ "Scale", 0, -1, 1 };
	ofParameter<float> speed{ "Speed", 0, 0, 1 };
	ofParameter<ofColor> c1{ "Color 1",ofColor::blue, ofColor(0,0), ofColor(255,255) };
	ofParameter<ofColor> c2{ "Color 2",ofColor::white, ofColor(0,0), ofColor(255,255) };
	ofParameter<bool> bNoise{ "Noise", false };
	ofParameter<bool> bUpdateable{ "EnableUpdate", true };
	ofParameterGroup params{ "Scene", amount, scale, speed, bNoise , c1, c2, bUpdateable };

	void setColor1(ofColor c) { c1 = c; };
	void setColor2(ofColor c) { c2 = c; };

	void draw()
	{
		ofPushStyle();
		ofFill();
		ofSetCircleResolution(200);

		float sc = 4;
		float s = ofMap(scale, -1, 1, 1.f / sc, sc);

		float sz = s * max(ofGetWidth(), ofGetHeight());

		float x = ofGetWidth() * 0.5f;
		float y = ofGetHeight() * 0.5f;

		float _speed = ofMap(speed, 0, 1, 2, 0.3, true);
		static float vBounce;

		if (bUpdateable)
			vBounce = ofxSurfingHelpersT::Bounce(_speed);

		float r0 = 0.75 + vBounce;

		float _d = 1;
		float d0 = 1.f / (float)amount;

		float _n = ofMap(ofxSurfingHelpersT::Noise(), 0, 1, 0.95, 1.05, true);
		if (bNoise) d0 = d0 * _n;

		for (int i = 0; i < amount; i++)
		{
			bool _b = (i % 2 == 0);//odd/even

			//int a = 255;
			int a = _b ? c1.get().a : c2.get().a;

			_d = _d - d0;
			float _r = r0 * sz * _d;

			ofSetColor(_b ? c1 : c2, a);
			ofDrawCircle(x, y, 0, _r);
		}

		ofPopStyle();
	}

};