#pragma once

#include "ofMain.h"

namespace ofxSurfingHelpers {

	/* Return a linear value in range [0,1] every delay (in seconds). */
	inline float Tick(float delay = 1.0f) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}
	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	inline float Bounce(float delay = 1.0f) {
		return 0.5f * (1.0 + glm::sin(ofxSurfingHelpers::Tick(delay) * glm::two_pi<float>()));
	}
	/* Noise function used by the gradient scaling. */
	inline float Noise(const ofPoint& vertex = ofPoint(1, -1)) {
		return ofNoise(0.05f * vertex + 0.5f * ofGetElapsedTimeMillis() * 0.002f);
	}
}

class surfingSceneTesters
{
private:

	ofColor c1 = ofColor::blue;
	ofColor c2 = ofColor::white;

public:

	void setColor1(ofColor c) { c1 = c; };
	void setColor2(ofColor c) { c2 = c; };

	//--------------------------------------------------------------
	void drawScene()
	{
		float sz = max(ofGetWidth(), ofGetHeight());
		float x = ofGetWidth() * 0.5f;
		float y = ofGetHeight() * 0.5f;
		int a = 255;

		// a.
		//const int d = 30;
		//int f = ofGetFrameNum() % d;
		//float r0 = ofMap(f, 0, d, 0, 1, true);

		// b.
		float r0 = 0.75 + ofxSurfingHelpers::Bounce();
		//float r0 = ofxSurfingHelpers::Noise() * 0.75 + Bounce();
		//float r0 = ofxSurfingHelpers::Noise();

		/*
		{
			ofPushStyle();
			ofFill();

			float r1 = r0 * sz;
			float r2 = r0 * sz * 0.2;
			float r3 = r0 * sz * 0.025;
			float r4 = r0 * sz * 0.005;

			ofSetColor(c2, a);
			ofDrawCircle(x, y, 0, r1);
			ofSetColor(c1, a);
			ofDrawCircle(x, y, 0, r2);
			ofSetColor(c2, a);
			ofDrawCircle(x, y, 0, r3);
			ofSetColor(c1, a);
			ofDrawCircle(x, y, 0, r4);

			ofPopStyle();
		}
		*/

		///*
		{
			ofPushStyle();
			ofFill();
			ofSetCircleResolution(200);

			//int amt = 100 * Noise();
			int amt = 10;
			float _d = 1;
			float d0 = 1.f / (float)amt;
			//float d0 = 1.f / ((float)amt * Noise());

			for (int i = 0; i < amt; i++)
			{
				_d = _d - d0;
				float _r = r0 * sz * _d;

				bool _b = (i % 2 == 0);//odd/even
				ofSetColor(_b ? c1 : c2, a);
				ofDrawCircle(x, y, 0, _r);
			}

			ofPopStyle();
		}
		//*/
	}

};