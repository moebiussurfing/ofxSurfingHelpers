// This is a modified version by mobiusSurfing
// bc some errors on static definitions...

//
//     _____    ___     
//    /    /   /  /     ofxNotify
//   /  __/ * /  /__    (c) ponies & light, 2013, 2015. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxNotify.h
//  Created by tim on 23/01/2013.
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef GUARD__ofxNotify__
#define GUARD__ofxNotify__

#include "ofMain.h"

class ofxNotify
{

public:

	//--------------------------------------------------------------
	void addNotification(string msg) {
		messages[ofGetElapsedTimeMicros()] = msg;
	}

private:

	std::map<unsigned long long, std::string> messages;
	int messageLifeTimeInMilliseconds; // defaults to 2000ms
	std::ostringstream message;
	bool bPrinted = false;

	ofTrueTypeFont font;
	float fontSize;
	int pad;
	int pad2;
	float rounded;

	//--------------------------------------------------------------
	void refreshPads()
	{
		pad = 3 * fontSize;
		pad2 = fontSize * 0.5f;
	}

public:

	//--------------------------------------------------------------
	ofxNotify()
	: bPrinted(false) {

		messageLifeTimeInMilliseconds = 2000;

		// draw help font
		fontSize = 15;
		font.load(OF_TTF_MONO, fontSize);

		refreshPads();
		rounded = 10.f;
	};

	//--------------------------------------------------------------
	void setup(string pathFont, int sizeFont, float round = 10.f) 
	{
			// draw help font
			fontSize = sizeFont;
			font.load(pathFont, sizeFont);

			rounded = round;
			refreshPads();
	}

	virtual ~ofxNotify();

	// draw the notifier by calling ofxNotify::draw(); from within your app.
	void draw(bool shouldDraw_ = true);
	void setMessagesLifeTime(int messageLifeTimeInMilliseconds_);

	//--------------------------------------------------------------
	void drawTextBoxed(string text, int x, int y, int alpha = 255)
	{
		ofPushStyle();

		if (!font.isLoaded()) 
		{
			ofDrawBitmapStringHighlight(text, x, y);
		}
		else 
		{
			// bbox
			ofSetColor(0, alpha);
			ofFill();
			ofRectangle _r(font.getStringBoundingBox(text, x, y));
			_r.setWidth(_r.getWidth() + pad);
			_r.setHeight(_r.getHeight() + pad);
			_r.setX(_r.getPosition().x - pad / 2.);
			_r.setY(_r.getPosition().y - pad / 2.);
	
			//ofDrawRectangle(_r);
			ofDrawRectRounded(_r, rounded);

			// text
			ofSetColor(255, alpha);
			ofNoFill();
			font.drawString(text, x, y);
		}

		ofPopStyle();
	}

private:

	//--------------------------------------------------------------
	float getWidthBBtextBoxed(string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getWidth();
	}

	//--------------------------------------------------------------
	float getHeightBBtextBoxed(string text) {
		return (font.getStringBoundingBox(text, 0, 0)).getHeight();
	}

	//----

};


#endif /* defined(GUARD__ofxNotify__) */
