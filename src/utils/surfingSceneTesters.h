#pragma once

#include "ofMain.h"

namespace ofxSurfingHelpersT {

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