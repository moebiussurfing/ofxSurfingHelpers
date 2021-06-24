#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxInteractiveRect.h"
#include "DoubleClicker.h"

class TextBoxWidget : public ofBaseApp
{
public:

	enum BOX_LAYOUT
	{
		FREE_LAYOUT = 0,
		BOTTOM_CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_CENTER,
		TOP_LEFT,
		TOP_RIGHT,
		CENTER,
		NUM_LAYOUTS
	};

private:
	DoubleClicker doubleClicker;

	BOX_LAYOUT layout = FREE_LAYOUT;

	ofxInteractiveRect rect_HelpTextBox = { "Help_ofxPresetsManager" };
	std::string path_RectHelpBox = "_HelpBox";
	std::string path_Global = "TextBoxWidget/";// can be setted before setup
	std::string path_Name = "appSettings";// subfolder for app session settings

	// font to label clicker boxes
	ofTrueTypeFont myFont;
	std::string myTTF;// gui font for all gui theme
	int sizeTTF;

	std::string textInfo = "empty content";// info text to display shortcuts or path settings

	ofColor _colorText;// lines and text color
	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;
	bool bThemeDarkOrLight = true;

	bool bState1 = false;
	bool bState2 = false;

	float round = 5;
	int marginBorders = 50;

public:
	void setPath(string path) {
		path_Global = path;
	}

	//--------------------------------------------------------------
	TextBoxWidget() {
		std::string str;

		sizeTTF = 11;
		str = "telegrama_render.otf";
		//sizeTTF = 10;
		////str = "overpass-mono-bold.otf";
		//str = "telegrama_render.otf";

		myTTF = "assets/fonts/" + str;
		bool bLoaded = myFont.load(myTTF, sizeTTF, true, true);
		if (!bLoaded) bLoaded = myFont.load(OF_TTF_SANS, sizeTTF, true, true);

		_bUseShadow = true;

		setTheme(bThemeDarkOrLight);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight());//full screen
		doubleClicker.setDebug(false);
	}

	//--------------------------------------------------------------
	void setup() {

		ofxSurfingHelpers::CheckFolder(path_Global + "/" + path_Name + "/");
		//ofxSurfingHelpers::CheckFolder(path_Global + "/" + path_Name + "/");

		// load settings
		rect_HelpTextBox.loadSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
		rect_HelpTextBox.setLockResize(true);
	}

	//--------------------------------------------------------------
	void draw() {
		ofPushStyle();

		drawDoubleClickDebug();

		//-

		std::string ss = textInfo;
		float _w = ofGetWidth();
		float _h = ofGetHeight();

		int _padx = 10;
		int _pady = 10;

		int _xx;
		int _yy;

		float _ww = ofxSurfingHelpers::getWidthBBtextBoxed(myFont, ss);
		float _hh = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, ss);
		rect_HelpTextBox.setWidth(_ww);
		rect_HelpTextBox.setHeight(_hh);

		//-

		if (layout == FREE_LAYOUT) {

			_xx = rect_HelpTextBox.getX();
			_yy = rect_HelpTextBox.getY();
		}

		else if (layout == BOTTOM_CENTER) {

			_xx = _w / 2 - _ww / 2 - _padx;
			_yy = _h - _hh - _pady;
		}
		else if (layout == BOTTOM_LEFT) {

			_xx = _padx;
			_yy = _h - _hh - _pady;
		}
		else if (layout == BOTTOM_RIGHT) {

			_xx = _w - _ww - _padx;
			_yy = _h - _hh - _pady;
		}

		else if (layout == TOP_CENTER) {

			_xx = _w / 2 - _ww / 2 - _padx;
			_yy = _pady;
		}
		else if (layout == TOP_LEFT) {

			_xx = _padx;
			_yy = _pady;
		}
		else if (layout == TOP_RIGHT) {

			_xx = _w - _ww - _padx;
			_yy = _pady;
		}

		else if (layout == CENTER) {

			_xx = _w / 2 - _ww / 2 - _padx;
			_yy = _h / 2 - _hh / 2 - _pady;
		}

		//-

		ofColor colorBg;

		if (layout == FREE_LAYOUT) {

			if (rect_HelpTextBox.isEditing()) {
				float a = ofxSurfingHelpers::getFadeBlink();
				ofColor c = ofColor(_colorBg, 255 * a);
				rect_HelpTextBox.draw();

				colorBg = c;
			}
			else
			{
				colorBg = _colorBg;
			}
		}
		else
		{
			colorBg = _colorBg;
		}

		ofPopStyle();

		//-

		ofxSurfingHelpers::drawTextBoxed(myFont, ss, _xx, _yy, _colorText, colorBg, _bUseShadow, _colorButton, marginBorders, round);

		//-

		{
			// fit iniside window
			float _xmax = _w - _ww - _padx;
			float _ymax = _h - _hh - _pady;

			if (rect_HelpTextBox.getY() > _ymax)
			{
				rect_HelpTextBox.setY(_ymax);
			}
			else if (rect_HelpTextBox.getX() < _padx)
			{
				rect_HelpTextBox.setX(_padx);
			}
			else if (rect_HelpTextBox.getX() > _xmax)
			{
				rect_HelpTextBox.setX(_xmax);
			}
			else if (rect_HelpTextBox.getY() < _pady)
			{
				rect_HelpTextBox.setY(_pady);
			}
		}

		doubleClicker.set(_xx, _yy, _ww, _hh);
	}

private:
	//--------------------------------------------------------------
	void drawDoubleClickDebug() {

		// double click swap edit mode
		if (doubleClicker.isMouseDoubleClick())
		//if (doubleClicker.isMouseTripleClick()) 
		{
			bState1 = !bState1;

			setEdit(bState1);

			// workflow
			if (bState1) {
				if (layout != FREE_LAYOUT) layout = FREE_LAYOUT;
			}
			//layout = FREE_LAYOUT;
		}

		// triple clicks swap layout mode
		if (doubleClicker.isMouseTripleClick()) 
		//if (doubleClicker.isMouseDoubleClick())
		{
			bState2 = !bState2;

			int i = layout;
			i++;
			if (i >= NUM_LAYOUTS) { layout = FREE_LAYOUT; }
			else { layout = BOX_LAYOUT(i); }
		}

		//-

		////debug colors to bg
		////if (bState2) ofClear(bState1 ? ofColor::blue : ofColor::yellow);
		////else ofClear(bState1 ? ofColor::black : ofColor::white);

		//bool bDebug = false;
		//if (!bDebug) return;

		////-

		//stringstream s;
		//s << "DoubleClick changes color." << endl;
		//s << "TripleClick changes RGB/Black&White." << endl;
		//s << "number Clicks: " << doubleClicker.clickCounter << endl;
		////NOTE:can't check these easycallbakcs twice on same frame
		////s << "doubleClicked: " << "[" << (obj.isMouseDoubleClick() ? "x" : " ") << "]" << endl;
		////s << "tripleClicked: " << "[" << (obj.isMouseTripleClick() ? "x" : " ") << "]" << endl;
		//s << endl;
		//s << "isMouseOver: " << doubleClicker.isMouseOver() << endl;
		//s << "isMousePressed(0): " << doubleClicker.isMousePressed(0) << endl;
		//s << "isMousePressed(1): " << doubleClicker.isMousePressed(1) << endl;
		//s << "isMousePressed(2): " << doubleClicker.isMousePressed(2) << endl;
		//s << "getStateChangeMillis(): " << doubleClicker.getStateChangeMillis();
		//ofDrawBitmapStringHighlight(s.str(), 10, 30);
	}

public:
	//--------------------------------------------------------------
	void setEdit(bool bEdit)
	{
		if (bEdit)
		{
			rect_HelpTextBox.enableEdit();
		}
		else
		{
			rect_HelpTextBox.disableEdit();

			// all app settings
			//save_ControlSettings();
			rect_HelpTextBox.saveSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
		}
	}

	//--------------------------------------------------------------
	void setTheme(bool bTheme) {
		bThemeDarkOrLight = bTheme;

		// light theme
		if (!bThemeDarkOrLight)
		{
			_colorText = ofColor(0, 255);
			_colorButton = ofColor(255, 64);
			_colorBg = ofColor(225, 64);
		}
		// dark theme (white lines & black bg)
		else
		{
			_colorText = ofColor(255, 150);
			_colorButton = ofColor(16, 225);
			_colorBg = ofColor(0, 128);
		}
	}

public:
	//--------------------------------------------------------------
	void setText(string text) {
		textInfo = text;
	}

	//--
};

//--