//
//     _____    ___     
//    /    /   /  /     ofxNotify
//   /  __/ * /  /__    (c) ponies & light, 2013. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  ofxNotify.cpp
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

#include "ofxNotify.h"

// ----------------------------------------------------------------------
// Static class variables
// ----------------------------------------------------------------------

//map<unsigned long long, string>				ofxNotify::messages;
//
//ofMutex										ofxNotify::msgMutex;
//int											ofxNotify::messageLifeTimeInMilliseconds = 2000;


// ----------------------------------------------------------------------
// Class Methods
// ----------------------------------------------------------------------

void ofxNotify::setMessagesLifeTime(int messageLifeTimeInMilliseconds_) {
	messageLifeTimeInMilliseconds = messageLifeTimeInMilliseconds_;
}

// ----------------------------------------------------------------------

void ofxNotify::draw(bool shouldDraw_) {

	if (messages.empty()) return;

	// -------- Invariant: there are messages to display.

	unsigned long long elapsedTimeNow = ofGetElapsedTimeMicros();

	map<unsigned long long, string>::iterator it = messages.begin();

	if (shouldDraw_)
	{
		ofPushStyle();

		int i = 0;

		float y = pad + 2 * pad2;
		float x;
		float w;
		float h;

		int alpha;

		while (it != messages.end())
		{
			string str = it->second;
			alpha = ofMap(elapsedTimeNow - it->first, 0, messageLifeTimeInMilliseconds * 1000 * 1.f, 255, 0);
			//alpha = ofMap(elapsedTimeNow - it->first, 0, messageLifeTimeInMilliseconds * 1000 * 1.75f, 255, 100);

			if (!font.isLoaded()) {
				w = (it->second.size() * 8 + pad);
				h = 20;
			}
			else {
				w = getWidthBBtextBoxed(str) + pad;
				h = getHeightBBtextBoxed(str) + pad + pad2;
			}
			x = ofGetWidth() - w;

			if (!font.isLoaded())
			{
				ofDrawBitmapStringHighlight(str, x, y, (i == messages.size() - 1) ? (ofColor::red) : (ofColor::black));
			}
			else
			{
				//ofSetColor((i == messages.size() - 1) ? (ofColor::red) : (ofColor::black));
				drawTextBoxed(str, x, y, alpha);
			}

			it++;
			i++;

			if (!font.isLoaded()) {
				y = 50 + i * h;
			}
			else {
				y = y + h;
			}
		}

		ofPopStyle();
	}

	it = messages.begin();
	bool hasFoundDeadMessage = false;

	while (it != messages.end()) {
		if (elapsedTimeNow - it->first > messageLifeTimeInMilliseconds * 1000) {
			it++;
			hasFoundDeadMessage = true;
		}
		else {
			break;
		}
	}

	if (hasFoundDeadMessage) {
		messages.erase(messages.begin(), it);
	}
}

// ----------------------------------------------------------------------
// Instance Methods
// ----------------------------------------------------------------------

ofxNotify::~ofxNotify() {
	if (!bPrinted) {
		if (!message.str().empty()) {
			messages[ofGetElapsedTimeMicros()] = message.str();
		}
	}
}

// ----------------------------------------------------------------------

