#pragma once

#include "ofMain.h"
#include "SurfingTimers.h"

#define FONT_FILES_PATH "assets/fonts/"
#define FONT_FILE_BIG "JetBrainsMonoNL-ExtraBold.ttf"
#define FONT_FILE_SMALL "JetBrainsMono-Bold.ttf"
#define FONT_SIZE_BIG 16
#define FONT_SIZE_SMALL 10

class CircleBeat
{
private:

	float dt_Bpm = 0.f;
	bool bBpmMode = false;
	bool bToggleMode = false;
	bool bState = false;

public:

	void toggle(bool b)
	{
		bState = b;
		if (bState)animCounter = 0.0f;//anim from 0.0 to 1.0
		else animCounter = 1.0f;
	}

	void setToggleMode(bool b) {
		bToggleMode = b;
		toggle(false);
	}

	ofParameterGroup params{ "CircleBeat" };
	ofParameter<float> radiusMax{ "Radius", 100, 10, 1920 };
	ofParameter<float> bpm{ "Bpm", -1, 40.f, 400.f };
	ofParameter<float> speed{ "Speed", 0.5f, 0.01f, 1 };
	ofParameter<int> div{ "Bpm Div", 2, 1, 4 };
	ofParameter<glm::vec2> position{ "Position", glm::vec2(100,100), glm::vec2(0,0), glm::vec2(1920,1080) };
	ofParameter<bool> bGui{ "bGui", true };
	ofParameter<bool> bLock{ "Lock Move", false };

	void setLocked(bool b) {
		bLock = b;
	}

	void setDiv(int _div)
	{
		div = _div;
		setBpm(bpm);
	}

	void setBpm(float _bpm, float _fps = 60.f)
	{
		bBpmMode = true;
		dt = 1.0f / _fps;

		if (bpm != _bpm)
			bpm = _bpm;

		int barDur = 60000 / bpm;// one bar duration in ms
		dt_Bpm = ((barDur * div) / 1000.f) * dt;

		//animCounter goes from 0 to 1
		//if (animRunning) animCounter += speedRatio * speed * dt;
	}

public:

	void setFps(float _fps)
	{
		dt = 1.0f / _fps;
		speedRatio = _fps / 10.0f;
	}
	ofColor getColor() const {
		return color;
	}
	void setColor(ofColor _color)
	{
		color.set(_color);
	}
	void setColorText(ofColor _color)
	{
		colorText.set(_color);
	}
	void setColorBackground(ofColor _color)
	{
		colorBg.set(_color);
	}
	void setAlpha(float _alpha)
	{
		int a = ofMap(_alpha, 0.0f, 1.0f, 0, 255);
		alphaMax = a;

		ofColor c = ofColor(colorBg);
		colorBg = ofColor(c.r, c.g, c.b, alphaMax);
	}
	void setSpeed(float _speed)
	{
		bBpmMode = false;
		ofClamp(_speed, 0.01f, 1.0f);
		speed = _speed;
	}
	void setRadius(float _size)
	{
		radiusMax = _size;
		radiusMin = radiusMax * 0.2f;
	}
	void setPosition(glm::vec2 _pos)
	{
		position = glm::vec2(_pos);

		x = position.get().x;
		y = position.get().y;
	}
	void setPosition(int _x, int _y)
	{
		x = _x;
		y = _y;

		position = glm::vec2(x, y);
	}
	void setEnableBorder(bool b)
	{
		bBorder = b;
	}
	float getValue()
	{
		float f = ofClamp(1.0f - animCounter, 0.f, 1.f);
		return f;
	}
	float getRadius()
	{
		return radiusMax;
	}
	glm::vec2 getPosition()
	{
		return position;
	}

	void setName(string n) { //call before setup
		bNamed = true;
		name = n;

		bGui.setName(name);
		params.setName(name);

		// font big
		{
			if (font.isLoaded()) return;
			//fontSize = sz;
			std::string _path = FONT_FILES_PATH; // assets folder
			string f = FONT_FILE_BIG;
			//string f = "JetBrainsMono-Bold.ttf";
			_path += f;
			bool b = font.load(_path, fontSize);
			if (!b) font.load(OF_TTF_MONO, fontSize);
		}

		// font small
		{
			if (font2.isLoaded()) return;
			//fontSize2 = sz;
			std::string _path = FONT_FILES_PATH; // assets folder
			string f = FONT_FILE_SMALL;
			_path += f;
			bool b = font2.load(_path, fontSize2);
			if (!b) font2.load(OF_TTF_MONO, fontSize2);
		}
	}

	void setSubLabel(string n) { //call before setup
		bNamed2 = true;
		name2 = n;

		if (font2.isLoaded()) return;

		//fontSize2 = sz;
		std::string _path = FONT_FILES_PATH; // assets folder
		string f = FONT_FILE_SMALL;
		_path += f;
		bool b = font2.load(_path, fontSize2);
		if (!b) font2.load(OF_TTF_MONO, fontSize2);
	}
	void setSubLabelBlinking(bool b) { bool blinking = b; }

private:

	ofTrueTypeFont font;
	ofTrueTypeFont font2;

	int fontSize = FONT_SIZE_BIG;//big
	int fontSize2 = FONT_SIZE_SMALL;//small

	//name
	string name = "";
	bool bNamed = false;
	//sublabel

	string name2 = "";
	bool bNamed2 = false;
	bool bSubLabelBlinkind = false;

	ofColor colorText;
	ofColor color;
	ofColor colorBg;
	//float radiusMax;
	float radiusMin;

	//glm::vec2 position;
	float x, y;

	int alphaMax;
	bool bBorder = true;

	float dt;
	float animCounter;
	bool animRunning;
	float alpha;
	float line;

	float speedRatio = 6.0f;

	// draggable
	bool bDraggable = true;
	bool dragged, hovered, released;
	ofVec2f diffDrag;
	bool draggedPrev, mousePressedPrev;

public:

	void setDraggable(bool b) { bDraggable = b; }

	//-

public:

	CircleBeat()
	{
		ofAddListener(ofEvents().mouseScrolled, this, &CircleBeat::mouseScrolled);

		ofSetCircleResolution(100);

		colorText.set(225, 225, 225, 200);
		color.set(255, 255, 255, 255);
		colorBg.set(0, 0, 0, 200);
		alphaMax = 200;

		dt = 1.0f / 60.f;
		alpha = 0.0f;
		position = glm::vec2(200, 200);
		x = position.get().x;
		y = position.get().y;

		speed.set("Speed", 0.5f, 0.01f, 1.f);

		radiusMax = 100;
		radiusMin = radiusMax * 0.8f;

		line = 2.0f;

		// params that can handle settings serialization from parent scope
		params.add(bGui);
		params.add(radiusMax);
		params.add(speed);
		params.add(div);
		params.add(bLock);
		params.add(position);

		ofAddListener(params.parameterChangedE(), this, &CircleBeat::Changed);

		stop();
	};

	~CircleBeat() {
		ofRemoveListener(ofEvents().mouseScrolled, this, &CircleBeat::mouseScrolled);
		ofRemoveListener(params.parameterChangedE(), this, &CircleBeat::Changed);
	};

	//--------------------------------------------------------------
	void Changed(ofAbstractParameter& e)
	{
		string n = e.getName();

		if (n == radiusMax.getName())
		{
			radiusMin = radiusMax * 0.2f;
		}
		else if (n == position.getName())
		{
			x = position.get().x;
			y = position.get().y;
		}
		else if (n == speed.getName())
		{
			setSpeed(speed);
		}
		else if (n == bpm.getName())
		{
			setBpm(bpm);
		}
		else if (n == div.getName())
		{
			//setBpm(bpm);
			setDiv(div);
		}
	}

	void bang()
	{
		animCounter = 0.0f;//anim from 0.0 to 1.0
	}

	void stop()//force
	{
		animCounter = 1.0f;//anim from 0.0 to 1.0
	}

	//private:

	void update()//not required if called draw!
	{
		if (!bGui) return;

		if (bToggleMode)
		{
			animRunning = false;
			return;
		}

		animRunning = (animCounter <= 1.0f);//goes from 0 to 1 (finished)

		if (animRunning)
		{
			if (!bBpmMode) animCounter += (dt * speedRatio * speed);
			else animCounter += dt_Bpm;
		}
	}

public:

	void draw()
	{
		if (!bGui) return;

		update();

		ofPushStyle();

		// background dark
		ofFill();
		ofSetColor(colorBg);
		ofDrawCircle(position, radiusMax);

		bool bSmall = radiusMax < 60;

		// inner radium
		if (animRunning || bToggleMode)
		{
			//static const int gap = bSmall ? 0 : 3;
			static const int gap = 3;

			ofFill();
			alpha = ofMap(animCounter, 0, 1, alphaMax, 0, true);

			float _radius;
			if (!bToggleMode) _radius = ofMap(animCounter, 0, 1, radiusMax - gap, radiusMin, true);
			else {
				if (bState) _radius = radiusMax - gap;
				else _radius = radiusMin;
			}

			//ofSetColor(color.r, color.g, color.b, alpha); // faded alpha
			//int a = alphaMax * 0.2f + alpha * 0.8f;
			ofSetColor(color, ofMap(alpha, 0, 255, 0, color.a, true));

			ofDrawCircle(position, _radius);

			// shadowed border
			//if (/*!bSmall && */bBorder)
			if (bBorder)
			{
				ofNoFill();
				static const float thickness = 4.f;
				//static const float thickness = bSmall ? 1.f : 4.f;
				static const int a = 64;
				float r;
				//if (!bSmall) r = 1 + _radius - (thickness / 2.f);
				//else r = _radius;
				r = _radius;
				ofSetLineWidth(thickness);
				ofSetColor(colorBg, a);

				ofDrawCircle(position, r);
			}
		}

		// border
		// outer radium
		if (bBorder)
		{
			ofNoFill();
			if (radiusMax > 20)	ofSetLineWidth(line);
			else ofSetLineWidth(1.0f);

			//int a = alphaMax * 0.2f + alpha * 0.8f;
			//ofSetColor(color, ofMap(a, 0, 255, 0, color.a, true));
			//ofSetColor(color, alpha);
			//ofSetColor(color, alphaMax * 0.1f + alpha * 0.9f);

			ofSetColor(color, MAX(48, alphaMax * 0.1f + alpha * 0.9f));
			/*
			if (bBorder) ofSetColor(color, MAX(48, alphaMax * 0.1f + alpha * 0.9f));
			else ofSetColor(color, alphaMax * 0.1f + alpha * 0.9f);
			*/

			ofDrawCircle(position, radiusMax);
		}

		//-

		// draggable

		if (bDraggable && !bLock)
		{
			position = glm::vec2(x, y);

			// check if hovered
			hovered = (ofGetMouseX() - x) * (ofGetMouseX() - x) + (ofGetMouseY() - y) * (ofGetMouseY() - y) < radiusMax * radiusMax;

			// check if point is clicked
			if (hovered && ofGetMousePressed() && !mousePressedPrev) {
				dragged = true;
			}

			// this registers when we unclick
			dragged &= ofGetMousePressed();

			// store mouse posString and value when dragging starts
			if (dragged && !draggedPrev) {
				diffDrag = ofVec2f(x, y) - ofVec2f(ofGetMouseX(), ofGetMouseY());
			}

			// move point
			if (dragged) {
				x = ofGetMouseX() + diffDrag.x;
				y = ofGetMouseY() + diffDrag.y;
			}

			// update released
			released = !dragged && draggedPrev;

			// draw border
			if (hovered || dragged)
			{
				ofSetColor(color, alphaMax * 0.4f);
				//float offsetSz = 2;
				float offsetSz = ((hovered || dragged) ? 0.01 : 0.0) * radiusMax;
				ofDrawCircle(x, y, radiusMax + offsetSz);
			}

			// update states
			draggedPrev = dragged;
			mousePressedPrev = ofGetMousePressed();
		}


		//--

		// text

		// name
		if (bNamed) {
			ofSetColor(colorText);
			//ofSetColor(255, 200);
			ofRectangle r;
			if (bSmall) r = font2.getStringBoundingBox(name, 0, 0);
			else r = font.getStringBoundingBox(name, 0, 0);
			int pad = r.getHeight();
			int _x, _y;
			//_x = position.get().x;
			//_y = position.get().y;

			//center
			_x = position.get().x - r.getWidth() / 2;
			_y = position.get().y + r.getHeight() / 2;

			if (bNamed2) _y -= fontSize / 2;

			//bottom
			//_y = position.get().y + radiusMax - 2 * pad;
			if (bSmall) font2.drawString(name, _x, _y);
			else font.drawString(name, _x, _y);
		}

		// sub label
		if (bNamed2) {
			if (bSubLabelBlinkind) {
				ofSetColor(ofColor(colorText.r, colorText.g, colorText.b, 
					colorText.a * ofxSurfingHelpers::Bounce()));
				//ofSetColor(255, 200 * ofxSurfingHelpers::Bounce());
			}
			else
					ofSetColor(colorText);
					//ofSetColor(255, 200);

			ofRectangle r = font2.getStringBoundingBox(name2, 0, 0);
			int pad = r.getHeight();
			int _x, _y;
			//_x = position.get().x;
			//_y = position.get().y;

			//center
			_x = position.get().x - r.getWidth() / 2;
			_y = position.get().y + r.getHeight() / 2;

			_y -= fontSize / 2;

			//bottom
			//_y = position.get().y + radiusMax - 2 * pad;

			if (bSmall) _y += fontSize2 + 5;//space for name
			else _y += fontSize + 5;//space for name

			font2.drawString(name2, _x, _y);
		}

		ofPopStyle();
	}

	void draw(glm::vec2 pos, float size) {
		position = pos;
		x = position.get().x;
		y = position.get().y;

		radiusMax = size;
		radiusMin = radiusMax * 0.8;

		draw();
	}

	void draw(glm::vec2 pos) {
		position = pos;
		x = position.get().x;
		y = position.get().y;

		draw();
	}

private:

	void mouseScrolled(ofMouseEventArgs& mouse) {
		if (!bGui)return;
		if (bLock)return;

		if (!bDraggable) return;
		if (!hovered) return;

		ofLogNotice("CircleBeat") << mouse.scrollY;

		float d = 0.1f;
		float s = ofMap(mouse.scrollY, -2, 2, 1.f - d, 1.f + d);

		radiusMax *= s;
		radiusMin = radiusMax * 0.8;
	}
};

