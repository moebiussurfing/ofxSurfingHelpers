#pragma once

#include "ofMain.h"

class RectBeat
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

	ofParameterGroup params{ "RectBeat" };
	ofParameter<float> sizeMax{ "Size", 100, 10, 1920 };
	ofParameter<float> bpm{ "Bpm", -1, 40.f, 240.f };
	ofParameter<float> speed{ "Speed", 0.5f, 0.01f, 1 };
	ofParameter<int> div{ "Bpm Div", 2, 1, 4 };
	ofParameter<glm::vec2> position{ "Position", glm::vec2(100,100), glm::vec2(0,0), glm::vec2(1920,1080) };
	ofParameter<bool> bGui{ "bGui", true };
	ofParameter<bool> bLock{ "bLock", false };

	void setLocked(bool b) {
		bLock = b;
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
	void setSize(float _size)
	{
		sizeMax = _size;
		sizeMin = sizeMax * 0.2f;
	}
	void setPosition(glm::vec2 _pos)
	{
		position = glm::vec2(_pos);

		x = position.get().x;
		y = position.get().y;
	}
	void setPosition(int x, int y)
	{
		x = position.get().x;
		y = position.get().y;

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
		return sizeMax;
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

		if (font.isLoaded()) return;
		//fontSize = sz;
		std::string _path = "assets/fonts/"; // assets folder
		string f = "JetBrainsMono-Bold.ttf";
		_path += f;
		bool b = font.load(_path, fontSize);
		if (!b) font.load(OF_TTF_MONO, fontSize);
	}

	void setSubLabel(string n) { //call before setup
		bNamed2 = true;
		name2 = n;

		if (font2.isLoaded()) return;

		//fontSize2 = sz;
		std::string _path = "assets/fonts/"; // assets folder
		string f = "JetBrainsMono-Bold.ttf";
		_path += f;
		bool b = font2.load(_path, fontSize2);
		if (!b) font2.load(OF_TTF_MONO, fontSize2);
	}
	void setSubLabelBlinking(bool b) { bool blinking = b; }

private:

	ofTrueTypeFont font;
	ofTrueTypeFont font2;

	int fontSize = 16;
	int fontSize2 = 9;

	//name
	string name = "";
	bool bNamed = false;
	//sublabel

	string name2 = "";
	bool bNamed2 = false;
	bool bSubLabelBlinkind = false;

	ofColor color;
	ofColor colorBg;
	//float sizeMax;
	float sizeMin;

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

	RectBeat()
	{
		ofAddListener(ofEvents().mouseScrolled, this, &RectBeat::mouseScrolled);

		ofSetCircleResolution(100);

		color.set(255, 255, 255, 255);
		colorBg.set(0, 0, 0, 200);
		alphaMax = 255;
		//alphaMax = 200;

		dt = 1.0f / 60.f;
		alpha = 0.0f;
		position = glm::vec2(200, 200);
		x = position.get().x;
		y = position.get().y;

		speed.set("Speed", 0.5f, 0.01f, 1.f);

		sizeMax = 100;
		sizeMin = sizeMax * 0.8f;

		line = 2.0f;

		// params that can handle settings serialization from parent scope
		params.add(bGui);
		params.add(sizeMax);
		params.add(speed);
		params.add(div);
		params.add(bLock);
		params.add(position);

		ofAddListener(params.parameterChangedE(), this, &RectBeat::Changed);

		stop();
	};

	~RectBeat() {
		ofRemoveListener(ofEvents().mouseScrolled, this, &RectBeat::mouseScrolled);
		ofRemoveListener(params.parameterChangedE(), this, &RectBeat::Changed);
	};

	//--------------------------------------------------------------
	void Changed(ofAbstractParameter& e)
	{
		string n = e.getName();

		if (n == sizeMax.getName())
		{
			sizeMin = sizeMax * 0.2f;
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
			setBpm(bpm);
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
		ofPushMatrix();
		ofTranslate(-sizeMax, -sizeMax);
		{
			// background dark
			ofFill();
			ofSetColor(colorBg);

			float _w = 2 * sizeMax;
			ofDrawRectangle(position, _w, _w);

			// fading bg box
			{
				ofFill();
				alpha = ofMap(animCounter, 0, 1, alphaMax, 0, true);
				int _a = ofMap(alpha, 0, 255, 0, color.a, true);
				if (/*animRunning || */bToggleMode)
				{
					ofSetColor(color, _a);
				}
				else
				{
					ofSetColor(color, MAX(_a, 32));//transparent box
				}
				ofDrawRectangle(position, _w, _w);
			}

			//--

			// border
			// outer radium
			if (bBorder)
			{
				ofNoFill();
				ofSetLineWidth(line);

				if (bBorder) ofSetColor(color, MAX(64, alphaMax * 0.1f + alpha * 0.9f));
				else ofSetColor(color, alphaMax * 0.1f + alpha * 0.9f);

				float _w = 2 * sizeMax;
				ofDrawRectangle(position, _w, _w);
			}

			//--

			// draggable

			if (bDraggable && !bLock)
			{
				position = glm::vec2(x, y);

				// check if hovered
				hovered = (ofGetMouseX() - x) * (ofGetMouseX() - x) + (ofGetMouseY() - y) * (ofGetMouseY() - y) < sizeMax * sizeMax;

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
					float offsetSz = ((hovered || dragged) ? 0.01 : 0.0) * sizeMax;

					float _w = 2 * sizeMax + offsetSz;
					ofDrawRectangle(x, y, _w, _w);

				}

				// update states
				draggedPrev = dragged;
				mousePressedPrev = ofGetMousePressed();
			}

			//--

			// text

			// name
			if (bNamed) {
				ofSetColor(255, 200);
				ofRectangle r = font.getStringBoundingBox(name, 0, 0);
				int pad = r.getHeight();
				int _x, _y;
				//_x = position.get().x;
				//_y = position.get().y;

				// center
				_x = position.get().x - r.getWidth() / 2;
				_y = position.get().y + r.getHeight() / 2;

				// bottom
				//_y = position.get().y + sizeMax - 2 * pad;

				font.drawString(name, _x, _y);
			}

			// sub label
			if (bNamed2) {
				if (bSubLabelBlinkind) {
					ofSetColor(255, 200 * ofxSurfingHelpers::Bounce());
				}
				else
					ofSetColor(255, 200);

				ofRectangle r = font2.getStringBoundingBox(name2, 0, 0);
				int pad = r.getHeight();
				int _x, _y;
				//_x = position.get().x;
				//_y = position.get().y;

				// center
				_x = position.get().x - r.getWidth() / 2;
				_y = position.get().y + r.getHeight() / 2;

				// bottom
				//_y = position.get().y + sizeMax - 2 * pad;

				_y += fontSize; // space for name

				font2.drawString(name2, _x, _y);
			}
		}
		ofPopMatrix();
		ofPopStyle();
	}

	void draw(glm::vec2 pos, float size) {
		position = pos;
		x = position.get().x;
		y = position.get().y;

		sizeMax = size;
		sizeMin = sizeMax * 0.8;

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

		ofLogNotice("RectBeat") << mouse.scrollY;

		float d = 0.1f;
		float s = ofMap(mouse.scrollY, -2, 2, 1.f - d, 1.f + d);

		sizeMax *= s;
		sizeMin = sizeMax * 0.8;
	}
};

