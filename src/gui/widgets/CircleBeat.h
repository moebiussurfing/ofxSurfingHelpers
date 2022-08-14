#pragma once

#include "ofMain.h"

class CircleBeat
{
private:

	float dtBpm = 0.f;
	bool bBpmMode = false;

public:

	ofParameterGroup params{ "CircleBeat" };
	ofParameter<float> radiusMax{ "Radius", 100, 10, 1920 };
	ofParameter<float> bpm{ "Bpm", -1, 60.f, 240.f };
	ofParameter<float> speed{ "Speed", 0.5f, 0.01f, 1 };
	ofParameter<int> div{ "Bpm Div", 2, 1, 4 };
	ofParameter<glm::vec2> position{ "Position", glm::vec2(100,100), glm::vec2(0,0), glm::vec2(1920,1080) };
	ofParameter<bool> bGui{ "bGui", true };
	ofParameter<bool> bLock{ "bLock", false };

	void setLocked(bool b) {
		bLock = b;
	}

	void setBpm(float _bpm, float _fps = 60)
	{
		bBpmMode = true;
		dt = 1.0f / _fps;

		if (bpm != _bpm)
			bpm = _bpm;

		int barDur = 60000 / bpm;// one bar duration in ms
		dtBpm = ((barDur * div) / 1000.f) * dt;

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
	void setRadius(float _size)
	{
		radiusMax = _size;
		radiusMin = radiusMax * 0.2;
	}
	void setPosition(glm::vec2 _pos)
	{
		position = glm::vec2(_pos);

		x = position.get().x;
		y = position.get().y;
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

	void setName(string n, int fontSize = 20) { //call before setup
		bNamed = true;
		name = n;
		bGui.setName(name);
		params.setName(name);

		std::string _path = "assets/fonts/"; // assets folder
		string f = "JetBrainsMono-Bold.ttf";
		_path += f;
		bool b = font.load(_path, fontSize);
		if (!b) font.load(OF_TTF_MONO, fontSize);
	}

private:

	ofTrueTypeFont font;

	string name = "";
	bool bNamed = false;

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

		color.set(255, 255, 255);
		colorBg.set(0, 0, 0, 200);
		alphaMax = 200;

		dt = 1.0f / 60.f;
		alpha = 0.0f;
		position = glm::vec2(200, 200);
		x = position.get().x;
		y = position.get().y;

		speed.set("Speed", 0.5f, 0.01f, 1.f);

		radiusMax = 100;
		radiusMin = radiusMax * 0.8;

		line = 2.0f;

		// params that can handle settings serialization from parent scope
		params.add(bGui);
		params.add(radiusMax);
		params.add(speed);
		params.add(div);
		params.add(bLock);
		params.add(position);

		ofAddListener(params.parameterChangedE(), this, &CircleBeat::Changed);
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
			radiusMin = radiusMax * 0.2;
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

	//private:

	void update()//not required if called draw!
	{
		if (!bGui) return;

		animRunning = animCounter <= 1.0f;//goes from 0 to 1 (finished)

		if (animRunning)
		{
			if (!bBpmMode) animCounter += speedRatio * speed * dt;
			else animCounter += dtBpm;
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

		// inner radium
		if (animRunning)
		{
			ofFill();
			alpha = ofMap(animCounter, 0, 1, alphaMax, 0, true);
			float _radius = ofMap(animCounter, 0, 1, radiusMax, radiusMin, true);

			ofSetColor(color.r, color.g, color.b, alpha);//faded alpha
			ofDrawCircle(position, _radius);
		}

		// outer radium
		if (bBorder)
		{
			ofNoFill();
			if (radiusMax > 20)	ofSetLineWidth(line);
			else ofSetLineWidth(1.0f);

			ofSetColor(color, alphaMax * 0.2f + alpha * 0.8f);
			//ofSetColor(color, alpha);

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


		if (bNamed) {
			ofSetColor(255, 200);
			ofRectangle r = font.getStringBoundingBox(name, 0, 0);
			int pad = r.getHeight();
			int _x, _y;
			//_x = position.get().x;
			//_y = position.get().y;

			//center
			_x = position.get().x - r.getWidth() / 2;
			_y = position.get().y + r.getHeight() / 2;

			//bottom
			//_y = position.get().y + radiusMax - 2 * pad;

			font.drawString(name, _x, _y);
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

