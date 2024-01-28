#pragma once

#include "ofMain.h"

/*

 WIDGETS

*/

//--

// A simple and animated scene using an image:

//TODO:
//// Draw image
//ofRectangle r(0, 0, image.getWidth(), image.getHeight());
//r.scaleTo(ofGetWindowRect());
//image.draw(r.x, r.y, r.width, r.height);

//--------------------------------------------------------------
class ImageAnimated : public ofBaseApp {
private:
	ofImage img;

	int xOffset;
	int vOffset;

public:
	ofParameter<bool> bAnimate { "Animate", true };

	//--------------------------------------------------------------
	void beginAnimate() {
		ofPushMatrix();

		const float noiseAmnt = 0.07f;
		float scale = ofMap(ofxSurfingHelpers::Bounce(), 0, 1, 1, 1.08f);
		float noise = ofMap(ofxSurfingHelpers::Noise(), -1, 1, -noiseAmnt, noiseAmnt);
		xOffset = noise * 500;
		vOffset = noise * 300;

		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofScale(scale + noise);
	}
	//--------------------------------------------------------------
	void endAnimate() {
		ofPopMatrix();
	}

	//--------------------------------------------------------------
	ImageAnimated() {
	}

	//--------------------------------------------------------------
	~ImageAnimated() {
	}

	//--------------------------------------------------------------
	void setup(string path) {
		img.load(path);
	}
	//--------------------------------------------------------------
	void load(string path) {
		img.load(path);
	}

	//--------------------------------------------------------------
	void draw() {
		if (!img.isAllocated()) return;

		if (!bAnimate) {
			img.draw(0, 0, ofGetWidth(), ofGetHeight());
		} else {
			beginAnimate();

			img.draw(xOffset - ofGetWidth() / 2, vOffset - ofGetHeight() / 2, ofGetWidth(), ofGetHeight());

			endAnimate();
		}
	}

	//--------------------------------------------------------------
	void draw(int x, int y, int w, int h) {
		if (!img.isAllocated()) return;

		if (!bAnimate) {
			img.draw(x, y, w, h);
		} else {
			beginAnimate();

			img.draw(xOffset - w / 2, vOffset - h / 2, w, h);

			endAnimate();
		}
	}
};

//----

namespace ofxSurfingHelpers {

//--------------------------------------------------------------
// circular progress bar
//
// example:
//float val = ofMap(ofGetFrameNum() % 200, 0, 200, 0.f, 1.f, true);
//ofxSurfingHelpers::drawCircleProg(val);
//--------------------------------------------------------------
inline void drawCircleProg(float val) {
	ofPushMatrix();
	ofPushStyle();
	ofSetLineWidth(5);

	ofRotateXDeg(-90);

	float radius = 50;
	float ratio = 0.7;

	//float val = 0.5;
	//float progress = val / 100;

	ofPoint point1(150, 120);

	ofNoFill();

	ofDrawRectangle(point1.x, point1.y, 100, 100);
	//int startAngle = -90 * 16;
	//int spanAngle = val * 360 * 16;
	int startAngle = -90;
	int spanAngle = -90 + val * 360;
	//int spanAngle = progress * 360 * 16;

	//p.drawArc(rectangle, startAngle, spanAngle);
	//void ofPolyline_::arc(float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd, int circleResolution=20)
	//polyline1.arc(point1, 100, 100, 0, 360);

	ofPolyline polyline1;
	polyline1.lineTo(point1.x, point1.y + radius * ratio);
	polyline1.lineTo(point1.x, point1.y + radius);
	polyline1.arc(point1, radius, radius, startAngle, spanAngle);
	polyline1.arc(point1, radius * ratio, radius * ratio, startAngle, spanAngle);
	//polyline1.lineTo(0, point1.y + radius * ratio);
	//polyline1.lineTo(0, point1.y + radius);
	ofSetColor(ofColor::blue);
	polyline1.draw();

	string str = "prog" + ofToString(val * 100);
	//string str = "prog" + ofToString(progress * 100);
	ofDrawBitmapStringHighlight(str, point1.x, point1.y + 100);

	ofPopStyle();
	ofPopMatrix();
}

//--

//ofDrawBitmapStringHighlight("No empty dock space, nowhere to draw for oF !", posX, posY);
//ofBitmapFont f;
//ofRectangle strSize = f.getBoundingBox("No empty dock space, nowhere to draw for oF !", posX, posY);

//TODO:
//fix variations..

//--------------------------------------------------------------
// draws a transparent box with centered text
//--------------------------------------------------------------
//#define BOX_PADDING 50
inline void drawTextBoxed(ofTrueTypeFont & font, const string & text, int x = 0, int y = 0, ofColor fColor = 255, ofColor colorBackground = ofColor(0, 247), bool useShadow = false, ofColor colorShadow = 128, int _pad = 50, float _round = 5, int heighForced = -1, bool noPadding = false) {
	int BOX_PADDING = _pad;
	if (!noPadding) {
		x += 25;
		y += 33;
	}

	//int xpad = _pad/2;
	//int ypad = _pad/2;

	//int _pad = 50;
	//float _round = 5;

	ofPushStyle();
	{
		//float fontSize = font.getSize();

		//TODO:
		// force load a default font
		if (!font.isLoaded()) {
			font.load(OF_TTF_MONO, 10, true, true, true);
		}

		if (!font.isLoaded()) {
			ofDrawBitmapStringHighlight(text, x, y);
		} else //if (font.isLoaded())
		{
			//--

			// 1. Bg Box

			// bbox
			ofSetColor(colorBackground);
			ofFill();

			ofRectangle _r;
			_r = (font.getStringBoundingBox(text, x, y));
			cout << _r.getWidth() << endl;

#if 0
				_r.setWidth(_r.getWidth() + _pad);
				_r.setX(_r.getPosition().x - _pad / 2.);
				_r.setY(_r.getPosition().y - _pad / 2.);
#endif

			if (ofGetKeyPressed(' ')) {
				cout << _r.x << ", ";
				cout << _r.y << ", ";
				cout << _r.getWidth() << ", ";
				cout << _r.getHeight();
				cout << endl;
			}

#if 0
				if (heighForced == -1) _r.setHeight(_r.getHeight() + _pad);
				////TODO:
				//if (heighForced == -1) 
				//{
				//	float _h;
				//	bool b = !ofIsStringInString(text, "\n");//if only one line. 
				//	if(b) _h = font.getStringBoundingBox("I", 0, 0).getHeight();//hardcoded height
				//	else _h = _r.getHeight();
				//	_r.setHeight(_h + _pad);
				//}
				else _r.setHeight(heighForced + _pad);
#endif

			// Draw
			if (_round <= 0)
				ofDrawRectangle(_r);
			else
				ofDrawRectRounded(_r, _round);

			//--

			// 2. Text

			// Text shadow
			if (useShadow) {
				ofSetColor(colorShadow);
				font.drawString(text, x + 1, y + 1);
			}

			// Text
			ofSetColor(fColor);
			font.drawString(text, x, y);
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
// get box width
//--------------------------------------------------------------

inline float getWidthBBtextBoxed(ofTrueTypeFont & font, string text, int _pad = 50) {
	return (font.getStringBoundingBox(text, 0, 0)).getWidth() + _pad;
}

inline float getHeightBBtextBoxed(ofTrueTypeFont & font, string text, int _pad = 50) {
	return (font.getStringBoundingBox(text, 0, 0)).getHeight() + _pad;
}

inline glm::vec2 getShapeBBtextBoxed(ofTrueTypeFont & font, string text, int _pad = 50) {
	glm::vec2 sh(getWidthBBtextBoxed(font, text, _pad), getHeightBBtextBoxed(font, text, _pad));
	return sh;
}

//--

// A mini version without paddings
// bg box aligned to text letters

#define PAD_MINI 12
//#define PAD_MINI 8

inline void drawTextBoxedMini(ofTrueTypeFont & font, string text, int x = 0, int y = 0, ofColor fColor = 255, ofColor colorBackground = ofColor(0, 247), bool useShadow = false, ofColor colorShadow = 128) {
	float _round = 0;
	int heighForced = -1;
	bool noPadding = true;

	//fix
	x += PAD_MINI / 2;
	y -= ((PAD_MINI / 2) - 1);

	drawTextBoxed(font, text, x, y, fColor, colorBackground, useShadow, colorShadow, (int)PAD_MINI, _round, heighForced, noPadding);
}
inline float getWidthBBtextBoxedMini(ofTrueTypeFont & font, string text) {
	return (font.getStringBoundingBox(text, 0, 0)).getWidth() + (int)PAD_MINI;
}
inline float getHeightBBtextBoxedMini(ofTrueTypeFont & font, string text) {
	return (font.getStringBoundingBox(text, 0, 0)).getHeight() + (int)PAD_MINI;
}
inline glm::vec2 getShapeBBtextBoxedMini(ofTrueTypeFont & font, string text) {
	glm::vec2 sh(getWidthBBtextBoxed(font, text, (int)PAD_MINI), getHeightBBtextBoxed(font, text, (int)PAD_MINI));
	return sh;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// to debug/show mouse position and x,y coordinates to draw points into layouts
//--------------------------------------------------------------
inline void draw_Anchor(int x, int y) {
	ofPushStyle();
	ofFill();
	ofSetColor(ofColor::red);
	ofDrawCircle(x, y, 3);
	int pad;
	if (y < 15)
		pad = 20;
	else
		pad = -20;
	ofDrawBitmapStringHighlight(ofToString(x) + "," + ofToString(y), x, y + pad);
	ofPopStyle();
}
};
