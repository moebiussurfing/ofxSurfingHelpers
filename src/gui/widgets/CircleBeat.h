#pragma once

#include "ofMain.h"

class CircleBeat
{
public:

	void setFps(float _fps)
	{
		dt = 1.0f / _fps;
		speedRatio = _fps / 10.0f;
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
		ofClamp(_speed, 0.01f, 1.0f);
		animSpeed = _speed;
	}
	void setRadius(float _size)
	{
		radiusMax = _size;
		radiusMin = radiusMax * 0.2;
	}
	void setPosition(glm::vec2 _pos)
	{
		position = glm::vec2(_pos);

		x = position.x;
		y = position.y;
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

private:

	ofParameter<float> animSpeed;
	ofColor color;
	ofColor colorBg;
	float radiusMax;
	float radiusMin;

	glm::vec2 position;
	float		x, y;

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
		x = position.x;
		y = position.y;

		animSpeed.set("animSpeed", 0.5f, 0.01f, 1.f);

		radiusMax = 100;
		radiusMin = radiusMax * 0.8;

		line = 2.0f;
	};

	~CircleBeat() {};

	void bang()
	{
		animCounter = 0.0f;//anim from 0.0 to 1.0
	}

private:

	void update()
	{
		animRunning = animCounter <= 1.0f;
		if (animRunning)
			animCounter += speedRatio * animSpeed * dt;
	}

public:

	void draw()
	{
		update();

		ofPushStyle();

		// background dark
		ofFill();
		ofSetColor(colorBg);
		ofDrawCircle(position, radiusMax);

		if (animRunning)
		{
			ofFill();
			alpha = ofMap(animCounter, 0, 1, alphaMax, 0);
			float _radius = ofMap(animCounter, 0, 1, radiusMax, radiusMin);

			ofSetColor(color.r, color.g, color.b, alpha);//faded alpha
			ofDrawCircle(position, _radius);
		}

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

		if (bDraggable)
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

		ofPopStyle();
	}

	void draw(glm::vec2 pos, float size) {
		position = pos;
		x = position.x;
		y = position.y;

		radiusMax = size;
		radiusMin = radiusMax * 0.8;

		draw();
	}

	void draw(glm::vec2 pos) {
		position = pos;
		x = position.x;
		y = position.y;

		draw();
	}

private:

	void mouseScrolled(ofMouseEventArgs& mouse) {
		ofLogNotice(__FUNCTION__) << mouse.scrollY;

		if (!bDraggable) return;
		if (!hovered) return;

		float d = 0.1f;
		float s = ofMap(mouse.scrollY, -2, 2, 1.f - d, 1.f + d);

		radiusMax *= s;
		radiusMin = radiusMax * 0.8;
	}
};

