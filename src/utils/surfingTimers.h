#pragma once

#include "ofMain.h"

/*

These functions can be used as simple signal generators, randomizers, fade blink or simple smoothing.

*/

#include <random>
#include <iostream>

using namespace std;

//namespace ofxImGuiSurfing
// -> collides between/if included ofxImGuiSurfing/ofxSurfingHelpers.h

namespace ofxSurfingHelpers
{
	//------------------------------------------------------------------------------
	// Animator functions taken from
	// https://github.com/tcoppex/ofxFontSampler/blob/main/example/src/ofApp.cpp

	namespace
	{
		/* Return a linear value in range [0,1] every delay (in seconds). */
		float Tick(float delay = 1.0f) {
			return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
		}

		/* Return a linear value in range [0,1] every delay (in seconds),
		 with value equal 0 at boundaries, and 1 at delay / 2. */
		float Bounce(float delay = 1.0f) {
			return 0.5f * (1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
		}

		/* Noise function used by the gradient scaling. */
		float Noise(const ofPoint& vertex = ofPoint(1, -1)) {
			//return /*24.0f **/ ofNoise(0.005f*vertex + 0.5f*ofGetElapsedTimeMillis()*0.0002f);

			return ofNoise(0.05f * vertex + 0.5f * ofGetElapsedTimeMillis() * 0.002f);
		}

		static constexpr int32_t kCharsetSize = 26;

	}  // namespace

	//--

	inline float NextGaussian(const float center, const float standard_dev)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::normal_distribution<float> distribution(center, standard_dev);
		return distribution(mt);
	}
	inline float NextReal(const float lower, const float upper)
	{
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> distribution(lower, upper);
		return distribution(mt);
	}

	//-

	// Get a blink faded to use as alpha on gui button when "active-listening-mode" enabled
	// ie: Blink when a new preset is editing
	//--------------------------------------------------------------
	inline float getFadeBlink(float min = 0.20, float max = 0.80, float freq = 0.15) {

		float a = ofMap(glm::sin(freq * ofGetFrameNum()), -1, 1, min, max);

		return a;
	}

	////----

	//// Simple Smooth
	///*
	//Example use:

	//*/

	////--------------------------------------------------------------
	//template <typename T>
	//inline void ofxKuValueSmooth(T& value, T target, float smooth) {
	//	value += (target - value) * (1 - smooth);
	//}

	////--------------------------------------------------------------
	//template <typename T>
	//void ofxKuValueSmoothDirected(T& value, T target, float smooth0, float smooth1) {
	//	float smooth = (target < value) ? smooth0 : smooth1;
	//	ofxKuValueSmooth(value, target, smooth);
	//}

	//----

	// Signal generators
	// useful for testing or just generating some noise

	class SurfGenerators
	{

#define NUM_SURF_GENERATORS 6

	public:

		SurfGenerators()
		{
			generators.resize(NUM_SURF_GENERATORS);
		}

		~SurfGenerators()
		{
		}

		void update() {

			if (ofGetFrameNum() % 30 == 0)
			{
				if (ofRandom(0, 1) > 0.5) bGenMode1 = !bGenMode1;
			}

			if (ofGetFrameNum() % 120 == 0)
			{
				if (ofRandom(0, 1) > ofRandom(0.5, 1)) bGenMode2 = !bGenMode2;
			}

			// NOTICE 
			// that fails when ignoring original params ranges!

			for (int g = 0; g < NUM_SURF_GENERATORS; g++)
			{
				switch (g) {

					// square 
				case 0: generators[g] = bGenMode1 ? 0.0 : 0.8;//simple
					//case 0: generators[g] = (bGenMode1 ? 0.f : (bGenMode2 ? 0.8 : 0.4));
					break;

					// saw
				case 1: generators[g] =  ofMap(ofxSurfingHelpers::Tick((bGenMode2 ? 1 : 2)), 0, 1, 0.0, 0.8);//simple
					//case 1: generators[g] = (bGenMode2 ? 1.f : 0.25f) * ofMap(ofxSurfingHelpers::Tick((bGenMode2 ? 1 : 2)), 0, 1, 0.1, 0.6);
					break;

					// noise slow1
				case 2: generators[g] = (bGenMode2 ? 1.f : 0.25f) * ofMap(ofxSurfingHelpers::Noise(ofPoint((!bGenMode2 ? 1 : 0.001), 1)), 0, 1, 0, 1);
					break;

					// noise slow2
				case 3: generators[g] = bGenMode1 ? 0.25 : ofxSurfingHelpers::Noise(ofPoint((!bGenMode2 ? 1 : 0.00001), (!bGenMode2 ? 0.3 : 0.03)));
					break;

					// noise fast1
				case 4: generators[g] = (ofxSurfingHelpers::Noise()) *
					(!bGenMode1 ? 0.25 : ofClamp(ofxSurfingHelpers::NextGaussian(0.25, 1), 0, 0.65));
					break;

					// noise fast2
				case 5: generators[g] = ofxSurfingHelpers::Noise() * ofMap(ofxSurfingHelpers::NextReal(0, (bGenMode2 ? 1 : 0.1)), 0, 1, 0.1, 0.7);
					break;

				}
			}
		}

		int size() {
			return (int)NUM_SURF_GENERATORS;
		}

		float get(int index) {
			if (index > NUM_SURF_GENERATORS) {
				ofLogError("SurfGenerators") << (__FUNCTION__) << "Index out of range! #" << index;

				return 0;
			}

			return generators[index];
		}

	private:

		vector<float> generators;

		bool bGenMode1 = false;
		bool bGenMode2 = false;
	};
};

