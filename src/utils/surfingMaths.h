#pragma once

#include "ofMain.h"

/*

These functions can be used as simple math operators, like linear to logarithmic converters.

*/

namespace ofxSurfingHelpers
{
	// Logarithmic functions
	// Recommended for Audio gain 
	// or some frequency variables. 

	inline float reversedExponentialFunction(float y) {
		return log10(y);
	}

	inline float exponentialFunction(float x) {
		return pow(10, x);
	}

	// Wrong way->slider widget returns a value between 0 and 100, 
	// divide that by 100 and multiply every sample by that value
	// Better way->slider widget returns a value between 0 and 100, 
	// divide that by 100, then square it, and multiply every sample by that value
	inline float squaredFunction(float v) {//pass gain
		return (v * v);
	}

	//--
    
    /*
	 
	EXAMPLE

	ofEventListener listener;
	ofParameter<float> vIn;
	ofParameter<float> vOut1;
	ofParameter<float> vOut2;
	ofParameter<float> vOut3;

	vIn.set("vIn", 0.5f, 0.f, 1.f);
	vOut1.set("vOut1", 0.5f, 0.f, 1.f);
	vOut2.set("vOut2", 0.5f, 0.f, 1.f);
	vOut3.set("vOut3", 0.5f, 0.f, 1.f);

	// Callback for vIn
	// convert input linear to log
	listener = vIn.newListener([this](float& v) {

		vOut1 = ofxSurfingHelpers::reversedExponentialFunction(vIn * 10.f);
		vOut2 = ofxSurfingHelpers::exponentialFunction(vIn) / 10.f;
		vOut3 = ofxSurfingHelpers::squaredFunction(vIn);

		ofLogNotice() << v << " -> " << vOut1.get() << " : " << vOut2.get();
	});

    */

	//----

	// Simple Smooth
	/*
	
	Example use:
	ofxSurfingHelpers::ofxKuValueSmooth(out, int, smooth);

	*/

	//--------------------------------------------------------------
	template <typename T>
	inline void ofxKuValueSmooth(T& value, T target, float smooth) {
		value += (target - value) * (1 - smooth);
	}

	//--------------------------------------------------------------
	template <typename T>
	void ofxKuValueSmoothDirected(T& value, T target, float smooth0, float smooth1) {
		float smooth = (target < value) ? smooth0 : smooth1;
		ofxKuValueSmooth(value, target, smooth);
	}
};