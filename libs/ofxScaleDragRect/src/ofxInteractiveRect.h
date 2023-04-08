//
//  interactiveRect.h
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//  Updated by moebiusSurfing 2020, 2021
//


/*

	TODO:

	+ blink on editing
	+ add border drawer, edit... on param group ready to add to a gui
	+ multiple positions left-rigth..etc
	+ double click to enable edit mode. Using MSAInteractive
	+ aspect ratio like camera mode,
	+ add helpers to rect fit, expand etc..
	+ fix rare from center scaling on text box widget
	+ add mouse scroll drag and scale ctrl / alt
	+ add mouse drag scale from center (get from ofxSCENE-SVG)
	+ lock x, y, w, h upper/bottom drag borders
	+ lock min/max size for the rect and avoid flipping..
	+ path setteable

*/


#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"

// Size of draggable and drawn zone
//#define BORDER_DRAG_SIZE 11 
#define BORDER_DRAG_SIZE 13 

//--

// Timing tools
namespace
{
	/* Return a linear value in range [0,1] every delay (in seconds). */
	//--------------------------------------------------------------
	float Tick(float delay = 1.0f) {
		return fmodf(ofGetElapsedTimeMillis() / 1000.0f, delay) / delay;
	}

	/* Return a linear value in range [0,1] every delay (in seconds),
	 with value equal 0 at boundaries, and 1 at delay / 2. */
	 //--------------------------------------------------------------
	float Bounce(float delay = 1.0f) {
		return 0.5f * (1.0 + glm::sin(Tick(delay) * glm::two_pi<float>()));
	}
} // namespace

//--

//--------------------------------------------------------------
class ofxInteractiveRect : public ofRectangle
{

private:

	bool bEnableMouseWheel = true;

public:

	void setEnableMouseWheel(bool b) { bEnableMouseWheel = b; }

private:

	ofColor colorBorderDraggable = { ofColor::yellow }; // Draggable borders
	ofColor colorEditingMoving{ ofColor(127, 127) }; // Borders. fill color when hover and moving the rectangle
	
	ofColor colorEditingHover{ ofColor(50, 20) }; // Rect fill when editing
	ofColor colorEditingPressedBorder{ ofColor(50, 200) }; // Borders
	
	// debug colors
	//ofColor colorEditingHover = { ofColor(ofColor::red, 200)}; // Draggable borders
	//ofColor colorEditingPressedBorder = { ofColor(ofColor::blue, 200)};
	
	//--

	bool bDrawBroder = true;

public:

	//--------------------------------------------------------------
	ofParameter<ofRectangle>& getParameterRectangle() {
		return rectParam;
	}

private:

	ofParameter<ofRectangle> rectParam{ "recetParam", ofRectangle(), ofRectangle(), ofRectangle() };
	void Changed_Rect(ofRectangle& r);

	// Rounded
	bool bRounded = false;
	float rounded = 0.0f;
	//float rounded = 5.0;

	float xpad = 0;
	float ypad = 0;

public:

	float diffx;
	float diffy;
	//TODO:
	float __diffx;
	float __diffy;

	//--------------------------------------------------------------
	void setPads(float x, float y) {
		xpad = x;
		ypad = y;
	}

	//--------------------------------------------------------------
	void setRounded(float r) {
		if (r > 0) bRounded = true;
		else bRounded = false;
		rounded = r;
	}

private:

	bool bLockX = false;
	bool bLockY = false;
	bool bLockW = false;
	bool bLockH = false;

	bool bTransparent = false;

public:

	void setLockX(bool b) { bLockX = b; };
	void setLockY(bool b) { bLockY = b; };
	void setLockW(bool b) { bLockW = b; };
	void setLockH(bool b) { bLockH = b; };

public:

	ofParameter<bool> bLockResize{ "Lock Resize", false };
	ofParameter<bool> bLock{ "Lock", false };

	//TODO:
	ofParameter<bool> bLockAspectRatio{ "Lock AspectRatio", false };
	//--------------------------------------------------------------
	void setLockAspectRatio(bool b) {
		bLockAspectRatio = b;
		if (bLockAspectRatio)
			aspectRatio = getWidth() / getHeight();
	}
	float aspectRatio = 16 / 9.f;

	//--------------------------------------------------------------
	void setLockResize(bool b) {
		bLockResize = b;
	}
	//--------------------------------------------------------------
	void setLock(bool b) {
		bLock = b;
	}
	//--------------------------------------------------------------
	void setTransparent() {
		bTransparent = true;
		setColorEditingHover(ofColor(0, 0));
		setColorEditingMoving(ofColor(0, 0));
	}

	//--------------------------------------------------------------
	void setColorBorderDraggable(const ofColor c) {
		colorBorderDraggable = c;
	}
	//--------------------------------------------------------------
	void setColorEditingHover(const ofColor c) {
		colorEditingHover = c;
	}
	//--------------------------------------------------------------
	void setColorEditingPressedBorder(const ofColor c) {
		colorEditingPressedBorder = c;
	}
	//--------------------------------------------------------------
	void setColorEditingMoving(const ofColor c) {
		colorEditingMoving = c;
	}

	//--------------------------------------------------------------
	bool isMouseOver() {
		bool b = inside(ofGetMouseX(), ofGetMouseY());

		return b;
	}

private:

	bool bAllScreenMouse = false;

public:

	//--------------------------------------------------------------
	void setAllScreenMouse(bool b) {
		bAllScreenMouse = b;
	}

	ofxInteractiveRect(string name, string path = "");

	virtual ~ofxInteractiveRect();

private:
	void exit(ofEventArgs& args);
	void exit();

public:
	void enableEdit(bool enable = true);
	void disableEdit();
	void toggleEdit();

	//--------------------------------------------------------------
	bool isEditing() 
	{ 
		if (bIsEditing) return true;
		else return bIsEditing; 
	}

	void draw();
	void drawBorder();

	bool loadSettings(string name = "", string path = "", bool bUseJson = false);//fix json
	void saveSettings(string name = "", string path = "", bool bUseJson = false);

private:

	bool bAutoSave = true;

public:

	//--------------------------------------------------------------
	void setAutoSave(bool b = true) {
		bAutoSave = b;

		if (b)
		{
			loadSettings();
		}
	}

public:

	void mouseMoved(ofMouseEventArgs& mouse);
	void mousePressed(ofMouseEventArgs& mouse);
	void mouseDragged(ofMouseEventArgs& mouse);
	void mouseReleased(ofMouseEventArgs& mouse);

	void mouseScrolled(ofMouseEventArgs& mouse);
	void mouseEntered(ofMouseEventArgs& mouse);
	void mouseExited(ofMouseEventArgs& mouse);

public:

	float getRectX() { return x; }
	float getRectY() { return y; }
	float getRectWidth() { return width; }
	float getRectHeight() { return height; }

private:

	string name;
	string path;

public:

	ofRectangle getRect();

	//--------------------------------------------------------------
	void setRect(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;

		if(!bConstrainedMin) this->width = width;
		else this->width = MAX(width, shapeConstraintMin.x);
		if(!bConstrainedMin) this->height = height;
		else this->height = MAX(height, shapeConstraintMin.y);

		doConstraints();
	}

	//--

private:

	glm::vec2 shapeConstraintMin;//min shape
	glm::vec2 shapeConstraintMax;//max shape//TODO:
	bool bConstrainedMin = false;
	bool bConstrainedMax = false;

	void doConstraints();
	
public:

	//--------------------------------------------------------------
	void setRectConstraintMin(glm::vec2 shape) {
		shapeConstraintMin = shape;
		bConstrainedMin = true;
	}
	//--------------------------------------------------------------
	void setRectConstraintMax(glm::vec2 shape) {
		shapeConstraintMax = shape;
		bConstrainedMax = true;
	}

	//--

private:

	ofJson saveToJson();
	void loadFromJson(const ofJson& j);

	ofXml saveToXml();
	bool loadFromXml(const ofXml& x);

	//--

public:

	ofParameter<bool> bEditMode{ "Edit", false };
	void Changed_EditMode(bool& b);

protected:

	bool bIsEditing;
	bool bMove;
	bool bIsOver;
	bool bLeft, bRight, bUp, bDown;
	bool bPressed;
	glm::vec2 mousePrev;

	std::string prefixName = "Rect";

	//--

	// Restore/reset position and shape

public:

	//--------------------------------------------------------------
	void setCentered()
	{
		setPosition(ofGetWidth() / 2.f - getWidth() / 2.f, ofGetHeight() / 2.f - getHeight() / 2.f);
	}
	//--------------------------------------------------------------
	void setResetSize(int sz = 800)
	{
		setWidth(sz);
		setHeight(sz / (16 / 9.f));
	}
	//--------------------------------------------------------------
	void setReset()
	{
		setResetSize();
		setCentered();
	}
};
