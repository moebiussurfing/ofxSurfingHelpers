
#pragma once

#include "ofMain.h"

#include "ofxSurfingHelpers.h"
#include "ofxInteractiveRect.h"
#include "DoubleClicker.h"
//#include "TextBoxWidget.h"

/*

	NOTE
	This class is deprecated and moved to continue into ofxSurfingBox.
	No removed bc legacy with old projects.

	TODO:

	+ use ctrl modifier bc three clicks interferes with double..
	+ add store layout settings
	+ move layout to drag rect class
	+ sum padding from 0, not expanded from center..

*/


/*

	This class draws a draggable text box with colored background.
	Auto stores and recall the box position between the app sessions.
	DoubleClick the box to allow move the position.

	Usage Example Snippet:

	// * HelpBox *

	// .h
	#include "TextBoxWidget.h"
	TextBoxWidget textBoxWidget;

	ofParameter<bool> bHelp{ "Help", true };

	// Setup
	{
		//textBoxWidget.setPath(path_GLOBAL + "HelpBox/");//customize path before call setup
		textBoxWidget.setup();

		std::string helpInfo = "";
		helpInfo += "HELP NDI MANAGER \n";
		helpInfo += "KEY COMMANDS \n\n";
		helpInfo += "H          HELP \n";
		helpInfo += "E          EDIT \n";
		helpInfo += "D          DEBUG \n";

		textBoxWidget.setText(helpInfo);
	}

	// Draw
	{
		if (bHelp) textBoxWidget.draw();
	}

*/

//#define LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE

class TextBoxWidget : public ofBaseApp
{

public:

	//--------------------------------------------------------------
	TextBoxWidget()
	{
		// Default

		size_TTF = 9;
		name_TTF = "JetBrainsMono-Bold.ttf";

		//size_TTF = 10;
		//name_TTF = "telegrama_render.otf";

		//size_TTF = 11;
		//name_TTF = "telegrama_render.otf";

		//size_TTF = 10;
		//name_TTF = "overpass-mono-bold.otf";
	}

	//--------------------------------------------------------------
	~TextBoxWidget()
	{
		// All app settings
		ofxSurfingHelpers::CheckFolder(path_Global + "/");
		rect_HelpTextBox.saveSettings(path_RectHelpBox, path_Global + "/", false);
		//rect_HelpTextBox.saveSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
	}

	//-

public:

	enum BOX_LAYOUT
	{
		FREE_LAYOUT = 0,
		CENTER,
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT,
		//LOCKED,

		NUM_LAYOUTS
	};

private:

	DoubleClicker doubleClicker;

	BOX_LAYOUT modeLayout = FREE_LAYOUT;
	string str_modeLayout = "";
	bool bLocked = false;

public:

	//--------------------------------------------------------------
	string getEditingString() {
		if (rect_HelpTextBox.isEditing()) return "Editing"; else return "Not Editing";
	}

	//--------------------------------------------------------------
	string getTheme() {
		if (bThemeDarkOrLight) return "Dark"; else return "Light";
	}

	//--------------------------------------------------------------
	string getTextMode() {
		if (bNoText) return "No Text"; else return "Text";
	}
	
	//--------------------------------------------------------------
	bool isEditing() {
		return rect_HelpTextBox.isEditing();
	}

	//--------------------------------------------------------------
	string getMode() {
		str_modeLayout = "UNKNOWN";
		switch (modeLayout)
		{
		case 0: str_modeLayout = "FREE_LAYOUT"; break;
		case 1: str_modeLayout = "BOTTOM_CENTER"; break;
		case 2: str_modeLayout = "BOTTOM_LEFT"; break;
		case 3: str_modeLayout = "BOTTOM_RIGHT"; break;
		case 4: str_modeLayout = "TOP_CENTER"; break;
		case 5: str_modeLayout = "TOP_LEFT"; break;
		case 6: str_modeLayout = "TOP_RIGHT"; break;
		case 7: str_modeLayout = "CENTER"; break;
			//case 8: str_modeLayout = "LOCKED"; break;
		default: str_modeLayout = "UNKNOWN LAYOUT"; break;
		}
		return str_modeLayout;
	}

private:

	ofxInteractiveRect rect_HelpTextBox = { "Help_ofxPresetsManager" };//main object

	std::string path_RectHelpBox = "_HelpBox";
	std::string path_Global = "TextBoxWidget/";// can be setted before setup
	//std::string path_Name = "appSettings";// subfolder for app session settings

	// font to label clicker boxes
	ofTrueTypeFont myFont;
	std::string path_TTF;
	std::string name_TTF;
	int size_TTF;

	std::string textInfo = "TextBoxWidget\nempty content";// info text to display shortcuts or path settings

	ofColor _colorText;// lines and text color
	ofColor _colorButton;// bg selected button
	ofColor _colorBg;// background color
	ofColor _colorShadow;
	bool _bUseShadow;

	bool bCenter = true;
	bool bLeftPosition = false;
	bool bThemeDarkOrLight = true;

	bool bStateEdit = false;
	bool bState2 = false;

	float round = 5;
	int marginBorders = 25;
	//int marginBorders = 50;

	bool bNoText = false;

	// Fit Marks
	int xcenter;
	int ycenter;
	int xleft;
	int xright;
	int ytop;
	int ybottom;

	//--------------------------------------------------------------
	void doForceFitOnWindow()
	{
		if (rect_HelpTextBox.getY() > ybottom) // bottom
		{
			rect_HelpTextBox.setY(ybottom);
		}
		else if (rect_HelpTextBox.getX() < xleft) // left
		{
			rect_HelpTextBox.setX(xleft);
		}
		else if (rect_HelpTextBox.getX() > xright) // right
		{
			rect_HelpTextBox.setX(xright);
		}
		else if (rect_HelpTextBox.getY() < ytop) // top
		{
			rect_HelpTextBox.setY(ytop);
		}
	};

	//float round = 5;
	//int padding = 50;

public:

	//ofParameter<glm::vec2> shape{ "Shape", glm::vec2(1920 / 2, 1080 / 2), glm::vec2(0,0), glm::vec2(19200,1080) };
	//--------------------------------------------------------------
	void setTextMode(bool b) {
		bNoText = !b;
		rect_HelpTextBox.setLockResize(b);
	}
	//--------------------------------------------------------------
	void setNoTextMode(bool b) {
		bNoText = b;
		rect_HelpTextBox.setLockResize(!b);
	}
	//--------------------------------------------------------------
	void setToggleNoTextMode() {
		bNoText = !bNoText;
		rect_HelpTextBox.setLockResize(!bNoText);
	}
	//--------------------------------------------------------------
	void setShape(ofRectangle shape) {
		rect_HelpTextBox.setRect(shape.x, shape.y, shape.getWidth(), shape.getHeight());
	}

public:

	//--------------------------------------------------------------
	void setPath(string path) {//call before setup. Will set path to save settings into.
		path_Global = path;
	}

	//--------------------------------------------------------------
	void setFontSize(int size = 10) { // Call before setup. Default it's 10
		size_TTF = size;
	}
	//--------------------------------------------------------------
	void setFontName(string name = "JetBrainsMono-Bold.ttf") {//set the name only. path is /assets/fonts/*name*
		name_TTF = name;
	}

	//--------------------------------------------------------------
	void setup() {
		
		//workflow
		rect_HelpTextBox.enableEdit();

		path_TTF = "assets/fonts/" + name_TTF;
		string path_TTF_LEGACY = "assets/fonts/telegrama_render.otf"; // some add-ons are settled with this font.
		bool bLoaded = myFont.load(path_TTF, size_TTF, true, true); // try before assign an oF bundled font!
		if (!bLoaded) bLoaded = myFont.load(path_TTF_LEGACY, size_TTF, true, true);
		if (!bLoaded) bLoaded = myFont.load(OF_TTF_MONO, size_TTF, true, true);

		_bUseShadow = true;

		setTheme(bThemeDarkOrLight);

		doubleClicker.set(0, 0, ofGetWidth(), ofGetHeight());// default full screen
		doubleClicker.setDebug(false);

		// Default position
		rect_HelpTextBox.setPosition(30, 30);
		//rect_HelpTextBox.setPosition(ofGetWidth() / 2, ofGetHeight() / 2);

		//----

		ofxSurfingHelpers::CheckFolder(path_Global + "/");
		//ofxSurfingHelpers::CheckFolder(path_Global + "/" + path_Name + "/");

		// Load settings
		rect_HelpTextBox.loadSettings(path_RectHelpBox, path_Global + "/", false);
		//rect_HelpTextBox.loadSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);

		// We dont need draggable borders and decoration.
		rect_HelpTextBox.setLockResize(true);
		//rect_HelpTextBox.setLockResize(!bNoText);
		rect_HelpTextBox.setTransparent();
	}
	//--------------------------------------------------------------
	void draw(string text) {
		setText(text);
		draw();
	}

	//--------------------------------------------------------------
	void draw() {
		ofPushStyle();

		updateDoubleClicker();

		//-

		std::string ss = textInfo;

		float _w = ofGetWidth();
		float _h = ofGetHeight();

		int _padx = 10;
		int _pady = 10;

		int _xx;
		int _yy;

		float _ww;
		float _hh;

		if (bNoText)
		{
			_ww = rect_HelpTextBox.getWidth();
			_hh = rect_HelpTextBox.getHeight();
		}
		else
		{
			_ww = ofxSurfingHelpers::getWidthBBtextBoxed(myFont, ss);
			_hh = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, ss);

			rect_HelpTextBox.setHeight(_hh);
			rect_HelpTextBox.setWidth(_ww);
		}

		//--

		// Fit Marks
		
		//TODO: must fix some offset!
		
		xleft = _padx + marginBorders / 2;
		xcenter = _w / 2 - _ww / 2 + marginBorders / 2 - _padx;
		xright = _w - _ww - marginBorders / 2 - _padx;
		
		ycenter = _h / 2 - _hh / 2 + marginBorders / 2;
		ytop = _pady + marginBorders / 2;
		ybottom = _h - _hh - _pady - marginBorders / 2;

		// Force fit box inside the window
		doForceFitOnWindow();

		//--

		// Free

		if (modeLayout == FREE_LAYOUT)
		{
			_xx = rect_HelpTextBox.getX();
			_yy = rect_HelpTextBox.getY();
		}

		//-

		// Top 

		else if (modeLayout == TOP_LEFT) {
			_xx = xleft;
			_yy = ytop;
		}
		else if (modeLayout == TOP_CENTER) {
			_xx = xcenter;
			_yy = ytop;
		}
		else if (modeLayout == TOP_RIGHT) {
			_xx = xright;
			_yy = ytop;
		}

		//-

		// Center 

		else if (modeLayout == CENTER) {

			_xx = xcenter;
			_yy = ycenter;
		}

		//-

		// Bottom

		else if (modeLayout == BOTTOM_LEFT) {
			_xx = xleft;
			_yy = ybottom;
		}
		else if (modeLayout == BOTTOM_CENTER) {
			_xx = xcenter;
			_yy = ybottom;
		}
		else if (modeLayout == BOTTOM_RIGHT) {
			_xx = xright;
			_yy = ybottom;
		}

		//-

		ofColor colorBg;

		if (modeLayout == FREE_LAYOUT)
		{
			if (rect_HelpTextBox.isEditing())
			{
				float a = ofxSurfingHelpers::getFadeBlink(0.6f, 1.f);

				ofColor c = ofColor(_colorBg, _colorBg.a * a);
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

		//-

		if (!bNoText)
		{
			int h = (bFixedHeight ? hLocked : -1); // Unlocked to resize related to text size

			ofxSurfingHelpers::drawTextBoxed(myFont, ss, _xx, _yy, _colorText, colorBg, _bUseShadow, _colorButton, marginBorders, round, h);
		}

		ofPopStyle();

		// Force fit box inside the window
		doForceFitOnWindow();

		doubleClicker.set(_xx, _yy, _ww, _hh);
	}

	//-

private:

	//--------------------------------------------------------------
	void updateDoubleClicker()
	{
		//--

		// 1. Double click swap edit mode

		//-

		// Allow edit only on free layout mode:
#ifdef LOCK_EDIT_ON_NON_FREE_LAYOUT_MODE
		if (modeLayout == FREE_LAYOUT)
#endif
			//-

		{
			if (doubleClicker.isMouseDoubleClick())
				//if (doubleClicker.isMouseTripleClick()) 
			{
				bStateEdit = !bStateEdit;

				setEdit(bStateEdit);

				// workflow
				if (bStateEdit)
				{
					if (modeLayout != FREE_LAYOUT) modeLayout = FREE_LAYOUT;
				}
				//modeLayout = FREE_LAYOUT;
			}
		}

		//--

		// 2. Triple clicks swap modeLayout mode

		if (doubleClicker.isMouseTripleClick())
			//if (doubleClicker.isMouseDoubleClick())
		{
			bState2 = !bState2;

			int i = modeLayout;
			i++;
			if (i >= NUM_LAYOUTS) { modeLayout = FREE_LAYOUT; }
			else { modeLayout = BOX_LAYOUT(i); }
		}

		//--

		//// Debug colors to bg
		////if (bState2) ofClear(bStateEdit ? ofColor::blue : ofColor::yellow);
		////else ofClear(bStateEdit ? ofColor::black : ofColor::white);

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
	void setPosition(int x, int y)
	{
		rect_HelpTextBox.setPosition(glm::vec3(x, y,0));
	}
	
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

			// All app settings
			//save_ControlSettings();
			rect_HelpTextBox.saveSettings(path_RectHelpBox, path_Global + "/", false);
			//rect_HelpTextBox.saveSettings(path_RectHelpBox, path_Global + "/" + path_Name + "/", false);
		}
	}

	//--------------------------------------------------------------
	void setTheme(bool bTheme) {
		bThemeDarkOrLight = bTheme;

		// Light theme (false = light)
		if (!bThemeDarkOrLight)
		{
			_colorText = ofColor(0, 255);
			_colorButton = ofColor(255, 64);
			_colorBg = ofColor(225, 64);
		}
		// Dark theme (white lines & black bg) (true = dark)
		else
		{
			_colorText = ofColor(255, 150);
			_colorButton = ofColor(16, 225);
			_colorBg = ofColor(0, 200);
		}
	}

	//--------------------------------------------------------------
	void setToggleTheme() {
		bThemeDarkOrLight = !bThemeDarkOrLight;
	}

	//--------------------------------------------------------------
	void setToggleMode() {
		int i = BOX_LAYOUT(modeLayout);
		i++;
		i = i % NUM_LAYOUTS;
		modeLayout = BOX_LAYOUT(i);
	}

	//--------------------------------------------------------------
	void setMode(BOX_LAYOUT mode) {
		modeLayout = mode;
	}

	//--------------------------------------------------------------
	void setLocked(bool b) {
		bLocked = b;
		if (b) doubleClicker.disableAllEvents();
		else doubleClicker.enableAllEvents();
	}

public:

	//--------------------------------------------------------------
	void setText(string text) {
		textInfo = text;
	}

	//--

//	//TODO:
//	// Should be added to the ofxSurfingHelpers::drawTextBoxed, adding a bool flag to force height..
//	// A workaround to lock the box height to the amount of lines using a 'I' char, 
//	// then it will not depends to the chars of the text. 
//	// An using case could be an only oneline text and his box height will no being different depending on used chars..

private:
	bool bFixedHeight = false;
	float hLocked = -1;
	int numLines = -1;

public:
	//--------------------------------------------------------------
	void setFixedHeight(int _numLines = 1) { // an small tweak to fix box size to an amount of lines
		bFixedHeight = true;
		numLines = _numLines;

		std::string _ss = "";
		for (int i = 0; i < numLines; i++) { _ss += "I" + (i == 0 && i < numLines - 1) ? "" : "\n"; }
		//std::string _ss = "I";
		hLocked = ofxSurfingHelpers::getHeightBBtextBoxed(myFont, _ss);
	}

};

//--