#pragma once
#include "ofMain.h"

/*

	TODO:

	- add customizable font.

*/

//--

// WARNING!
// This is an attempt to draw 3d planes but not using ofBitmapFont for the labels,
// but using ofTrueTypeFonts instead!
// One drawback is that we need to handle a trick to handle scale transformation on the camera!
// https://forum.openframeworks.cc/t/how-to-draw-oftruetype-labels-inside-an-ofcamera-as-ofdrawbitmapstring-does/41542/4

#include "ofxSurfingHelpers.h"

namespace ofxSurfingHelpers
{
	// Colors
#if(1)
	static const ofColor SURFING_RULES_COLOR_LABELS = ofColor{ 255, 255, 255, 200 };
	static const ofColor SURFING_RULES_COLOR_LINES_1 = ofColor{ 96, 96, 96, 150 };
	static const ofColor SURFING_RULES_COLOR_LINES_2 = ofColor{ 64, 64, 64, 150 };
	static const ofColor SURFING_RULES_COLOR_LINES_3 = ofColor{ 64, 64, 64, 32 };
	static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{ 40, 40, 40 };
	static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{ 0, 0, 0 };
#else//debug colors
	static const ofColor SURFING_RULES_COLOR_LABELS = ofColor{ 0, 255, 255, 200 };
	static const ofColor SURFING_RULES_COLOR_LINES_1 = ofColor{ 0, 96, 0, 150 };
	static const ofColor SURFING_RULES_COLOR_LINES_2 = ofColor{ 0, 0, 64, 150 };
	static const ofColor SURFING_RULES_COLOR_LINES_3 = ofColor{ 255, 0, 0, 100 };
	static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{ 40, 40, 40 };
	static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{ 0, 0, 0 };
#endif
	//--

// Drawing methods. Mostly based on OF internals.

//--------------------------------------------------------------
	inline void ofxDrawString(string s, float x, float y, float z, ofTrueTypeFont* font, ofCamera* camera) {
		if (font == nullptr) return;
		if (camera == nullptr) return;

		//TODO: trying to do a function to draw true types inside a cam, as billboard like ofBitmapFont does..
		/*
		bool bWorldTo2D = false;
		if (bWorldTo2D)
		{
			//TODO: wrong position..is no centered ?
			// could be related to ofScale(100.f); on StageManager::draw() ?
			//glm::vec3 posScreen = camera->worldToScreen(glm::vec3(x / 100.f, y / 100.f, z / 100.f));

			ofRectangle r = ofGetCurrentViewport();
			glm::vec3 posScreen = camera->worldToScreen(glm::vec3(x, y, z), r);

			float _x = posScreen.x;
			float _y = posScreen.y;

			////debug
			//float _x = ofGetWidth() / 2;
			//float _y = ofGetHeight() / 2;

			font->drawString(s, _x, _y);
		}
		else
		{
			//TODO: not works

#ifdef USE_TTF_TO_3D
			ofxDrawStringTrueType(s, x, y, z, &font);
#else
			font->drawString(s, x, y); // draw fonts in 3d, not to viewer/screen perpendicular oriented..
#endif
		}
		*/

		font->drawString(s, x, y); // draw fonts in 3d, not to viewer/screen perpendicular oriented..
	};

	// From libs\openFrameworks\graphics\of3dGraphics.cpp
	// Allows passing a font and the camera to customize style instead of using bitmap font
	//--------------------------------------------------------------
	inline void ofxDrawGridPlane(float stepSize, size_t numberOfSteps, bool labels, ofTrueTypeFont* font, ofCamera* camera, ofColor c1 = SURFING_RULES_COLOR_LINES_2, ofColor c2 = SURFING_RULES_COLOR_LABELS)
	{
		ofPushStyle();

		auto renderer = ofGetCurrentRenderer();

		float scale = stepSize * numberOfSteps;
		float lineWidth = renderer->getStyle().lineWidth;

		// Draw all the lines

		ofSetColor(c1);
		for (int iDimension = 0; iDimension < 2; iDimension++)
		{
			for (size_t i = 0; i <= numberOfSteps; i++)
			{
				float yz = i * stepSize;

				if (i == numberOfSteps || i == 0)
					renderer->setLineWidth(2); // central axis or cap line
				else if (i % 2 == 0) {
					renderer->setLineWidth(1.5); // major
				}
				else {
					renderer->setLineWidth(1); // minor
				}

				if (iDimension == 0) {
					renderer->drawLine(0, yz, -scale, 0, yz, scale);
					if (yz != 0) renderer->drawLine(0, -yz, -scale, 0, -yz, scale);
				}
				else {
					renderer->drawLine(0, -scale, yz, 0, scale, yz);
					if (yz != 0) renderer->drawLine(0, -scale, -yz, 0, scale, -yz);
				}
			}
		}
		renderer->setLineWidth(lineWidth);

		// Draw all the labels

		if (labels) {
			//draw numbers on axes
			if (font == nullptr || camera == nullptr) // bitmap
			{
				ofColor prevColor = renderer->getStyle().color;
				ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;

				renderer->setColor(c2);
				renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);
				renderer->drawString(ofToString(0), 0, 0, 0);

				for (float i = 1; i <= numberOfSteps; i++)
				{
					float yz = i * stepSize;

					renderer->drawString(ofToString(yz), 0, yz, 0);
					renderer->drawString(ofToString(-yz), 0, -yz, 0);
					renderer->drawString(ofToString(yz), 0, 0, yz);
					renderer->drawString(ofToString(-yz), 0, 0, -yz);
				}

				renderer->setColor(prevColor);
				renderer->setBitmapTextMode(mode);
			}
			/*
			else // custom
			{
				//TODO:
				// skip labels. we will draw using the out-of-the-camera approach!
				ofPopStyle();
				return;


				//ofSetColor(c2_);

				//ofxDrawString(ofToString(0), 0, 0, 0, font, camera);

				//for (float i = 1; i <= numberOfSteps; i++)
				//{
				//	float yz = i * stepSize;
				//	ofxDrawString(ofToString(yz), 0, yz, 0, font, camera);
				//	ofxDrawString(ofToString(-yz), 0, -yz, 0, font, camera);
				//	ofxDrawString(ofToString(yz), 0, 0, yz, font, camera);
				//	ofxDrawString(ofToString(-yz), 0, 0, -yz, font, camera);
				//}

				ofPopStyle();
			}
			*/
		}

		ofPopStyle();
	};

	// Draws a grid of the three planes with labels. Can use ofTrueTypeFont for labels if passed font and camera.
	// Taken from OF of\libs\openFrameworks\graphics\of3dGraphics.cpp
	// void of3dGraphics::drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z);
	//--------------------------------------------------------------
	inline void ofxDrawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z, ofTrueTypeFont* font, ofCamera* camera, ofColor c1 = SURFING_RULES_COLOR_LINES_3, ofColor c2 = SURFING_RULES_COLOR_LABELS)
	{
		auto renderer = ofGetCurrentRenderer();

		//ofPushStyle();
		ofColor prevColor = ofGetStyle().color;

		ofSetColor(c1);

		// lines with each plane labels

		if (font == nullptr || camera == nullptr) { // bitmap
			if (x) {
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
			}
			if (y) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
				renderer->pushMatrix();
				renderer->multMatrix(m);
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
				renderer->popMatrix();
			}
			if (z) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
				renderer->pushMatrix();
				renderer->multMatrix(m);
				ofDrawGridPlane(stepSize, numberOfSteps, labels);
				renderer->popMatrix();
			}
		}
		else { // custom ttf

			if (x) {
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
			}
			if (y) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
				ofPushMatrix();
				ofMultMatrix(m);
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
				ofPopMatrix();
			}
			if (z) {
				glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
				ofPushMatrix();
				ofMultMatrix(m);
				ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
				ofPopMatrix();
			}
		}

		if (labels) {
			if (font == nullptr || camera == nullptr)
			{
				//ofPushStyle();
				ofSetColor(c2); //color is not internal

				// bitmap
				ofDrawBitmapMode mode = ofGetStyle().drawBitmapMode;
				float labelPos = stepSize * (numberOfSteps + 0.5);

				ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
				ofDrawBitmapString("x", labelPos, 0, 0);
				ofDrawBitmapString("y", 0, labelPos, 0);
				ofDrawBitmapString("z", 0, 0, labelPos);

				ofSetDrawBitmapMode(mode);
				
				//ofPopStyle();
			}
			/*
			else
			{
				// custom ttf
				//TODO:
				// skip labels. we will draw using the out-of-the-camera approach!

				//ofPopStyle();
				ofSetColor(prevColor);
				return;


				//float labelPos = stepSize * (numberOfSteps + 0.5);

				//ofxDrawString("x", labelPos, 0, 0, font, camera);
				//ofxDrawString("y", 0, labelPos, 0, font, camera);
				//ofxDrawString("z", 0, 0, labelPos, font, camera);
			}
			*/
		}

		//ofPopStyle();
		ofSetColor(prevColor);
	};


	//--------------------------------------------------------------
	inline void ofxDrawGridBitmapFont(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z, ofColor c1 = SURFING_RULES_COLOR_LINES_3, ofColor c2 = SURFING_RULES_COLOR_LABELS)
	{
		ofxDrawGrid(stepSize, numberOfSteps, labels, x, y, z, nullptr, nullptr, c1, c2);
	}

	//----

	//TODO: WIP
	//#define USE_TTF_TO_3D // -> Enable to go fixing..
#ifdef USE_TTF_TO_3D
	// Getting from of\libs\openFrameworks\gl\ofGLRenderer.cpp
	//void ofGLRenderer::drawString(string textString, float x, float y, float z)
	//----------------------------------------------------------
	void ofxDrawStringTrueType(string textString, float x, float y, float z, ofTrueTypeFont* font) const
	{
		//ofGLRenderer* mutThis = const_cast<ofGLRenderer*>(this);

		// ?

		auto g = ofGetGLRenderer();
		ofGLRenderer* mutThis = g;

		//auto g = ofGetCurrentRenderer();
		//'const_cast': cannot convert from 'std::shared_ptr<ofBaseRenderer>' to 'ofGLRenderer *' 
		//ofGLRenderer* mutThis = const_cast<ofGLRenderer*>(g);

		//-

		float sx = 0;
		float sy = 0;


		///////////////////////////
		// APPLY TRANSFORM / VIEW
		///////////////////////////
		//

		bool hasModelView = false;
		bool hasProjection = false;
		bool hasViewport = false;

		ofRectangle rViewport;

		//bool vflipped = isVFlipped();
		bool vflipped = false;

		/*
		//switch (currentStyle.drawBitmapMode) {

		//case OF_BITMAPMODE_SIMPLE:

		//	sx += x;
		//	sy += y;
		//	break;

		//case OF_BITMAPMODE_SCREEN:

		//	hasViewport = true;
		//	mutThis->pushView();

		//	rViewport = matrixStack.getFullSurfaceViewport();
		//	mutThis->viewport(rViewport);

		//	mutThis->matrixMode(OF_MATRIX_PROJECTION);
		//	mutThis->loadIdentityMatrix();
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->loadIdentityMatrix();

		//	mutThis->translate(-1, 1, 0);
		//	mutThis->scale(2 / rViewport.width, -2 / rViewport.height, 1);
		//	mutThis->translate(x, rViewport.height - y, 0);
		//	vflipped = false;
		//	break;

		//case OF_BITMAPMODE_VIEWPORT:

		//	rViewport = getCurrentViewport();

		//	hasProjection = true;
		//	mutThis->matrixMode(OF_MATRIX_PROJECTION);
		//	mutThis->pushMatrix();
		//	mutThis->loadIdentityMatrix();

		//	hasModelView = true;
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->pushMatrix();
		//	mutThis->loadIdentityMatrix();

		//	mutThis->translate(-1, 1, 0);
		//	mutThis->scale(2 / rViewport.width, -2 / rViewport.height, 1);
		//	mutThis->translate(x, rViewport.height - y, 0);
		//	vflipped = false;
		//	break;

		//case OF_BITMAPMODE_MODEL:

		//	hasModelView = true;
		//	mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		//	mutThis->pushMatrix();

		//	mutThis->translate(x, y, z);
		//	break;
		*/

		//case OF_BITMAPMODE_MODEL_BILLBOARD:
		{
			//our aim here is to draw to screen
			//at the viewport position related
			//to the world position x,y,z

			// tig: we want to get the signed normalised screen coordinates (-1,+1) of our point (x,y,z)
			// that's projection * modelview * point in GLSL multiplication order
			// then doing the good old (v + 1.0) / 2. to get unsigned normalized screen (0,1) coordinates.
			// we then multiply x by width and y by height to get window coordinates.

			// previous implementations used gluProject, which made it incompatible with GLES (and the future)
			// https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/gluProject.3.html
			//
			// this could probably be backported to the GL2 Renderer =)

			//rViewport = getCurrentViewport();
			rViewport = ofGetCurrentViewport();

			glm::mat4 modelview = glm::mat4(1.0), projection = glm::mat4(1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelview));
			glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));


			//glm::mat4 mat = matrixStack.getOrientationMatrixInverse() * projection * modelview;

			//glm::mat4 mat = matrixStack.getOrientationMatrixInverse() * projection * modelview;
			//ofMatrixStack matrixStack;??

			//TODO: not inverted//
			glm::mat4 _m = ofGetCurrentOrientationMatrix();

			glm::mat4 mat = _m * projection * modelview;;





			glm::vec4 dScreen4 = mat * glm::vec4(x, y, z, 1.0);
			glm::vec3 dScreen = glm::vec3(dScreen4) / dScreen4.w;
			dScreen += glm::vec3(1.0);
			dScreen *= 0.5;

			dScreen.x *= rViewport.width;
			dScreen.x += rViewport.x;

			dScreen.y *= rViewport.height;
			dScreen.y += rViewport.y;

			if (dScreen.z >= 1) {
				return;
			}


			hasProjection = true;
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			hasModelView = true;
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
			mutThis->pushMatrix();
			mutThis->loadIdentityMatrix();

			mutThis->translate(-1, -1, 0);

			mutThis->scale(2 / rViewport.width, 2 / rViewport.height, 1);

			mutThis->translate(dScreen.x, dScreen.y, 0);
		}
		//}
		//break;

		//default:
		//	break;
		//}

		// remember the current blend mode so that we can restore it at the end of this method.
		GLint blend_src, blend_dst;
		glGetIntegerv(GL_BLEND_SRC, &blend_src);
		glGetIntegerv(GL_BLEND_DST, &blend_dst);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef TARGET_OPENGLES
		// this temporarily enables alpha testing,
		// which discards pixels unless their alpha is 1.0f
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
#endif



		//// A. Draw mesh text 
		//ofMesh charMesh = bitmapFont.getMesh(textString, sx, sy, currentStyle.drawBitmapMode, vflipped);
		//mutThis->bind(bitmapFont.getTexture(), 0);
		//draw(charMesh, OF_MESH_FILL, false, true, false);
		//mutThis->unbind(bitmapFont.getTexture(), 0);


		// B. Draw mesh true type text 
		ofMesh charMesh = font->getStringMesh(textString, sx, sy, vflipped);
		g->draw(charMesh, OF_MESH_FILL);




#ifndef TARGET_OPENGLES
		glPopAttrib();
#endif
		// restore blendmode
		glBlendFunc(blend_src, blend_dst);

		if (hasModelView)
			mutThis->popMatrix();

		if (hasProjection)
		{
			mutThis->matrixMode(OF_MATRIX_PROJECTION);
			mutThis->popMatrix();
			mutThis->matrixMode(OF_MATRIX_MODELVIEW);
		}

		if (hasViewport)
			mutThis->popView();
	}
#endif

	//TODO:
	// Another approach is to draw the labels out of the camera,
	// using worldTo2D

	// Allows passing a font and the camera to customize style instead of using bitmap font
	//--------------------------------------------------------------
	inline void ofxDrawGridPlaneLabelsTrueTypeFont(float stepSize, size_t numberOfSteps, ofTrueTypeFont* font, bool bOffset, ofCamera* camera, float scale = -1.0f, ofColor c = SURFING_RULES_COLOR_LABELS)
	{
		if (font == nullptr) return;
		if (camera == nullptr) return;

		//offset labels
		bool bO = false;

		//ofPushStyle();
		ofColor prevColor = ofGetStyle().color;

		ofSetColor(c);

		string s;
		glm::vec2 p;
		glm::vec2 bb;
		ofRectangle r = ofGetCurrentViewport();

		// pass the label and world 3d cords and gets the screen 2d coord.
		auto doConvertCoord = [&](const std::string& _s, const int _x, const int _y, const int _z, const bool _bOffset = false) {
			const ofRectangle _r = ofGetCurrentViewport();
			glm::vec3 _p = camera->worldToScreen(glm::vec3(_x, _y, _z), _r);

			if (_bOffset) {
				const glm::vec2 _bb = font->getStringBoundingBox(_s, 0, 0).getTopRight();
				_p = _p - glm::vec2(_bb.x / 2, -_bb.x / 2);
			}

			return _p;
		};

		glm::vec3 pp(0);

		// Draw all the labels
		{
			// Draw numbers on axes
			{
				// point zero 
				s = ofToString(0);
				pp = doConvertCoord(s, 0, 0, 0, bOffset);
				font->drawString(s, pp.x, pp.y);

				for (float i = 1; i <= numberOfSteps; i++)
				{
					float yz = i * stepSize;

					float yz_ = yz;//for labels without scale fix!

					// From OF:
					/*
					renderer->drawString(ofToString(yz), 0, yz, 0);
					renderer->drawString(ofToString(-yz), 0, -yz, 0);
					renderer->drawString(ofToString(yz), 0, 0, yz);
					renderer->drawString(ofToString(-yz), 0, 0, -yz);
					*/

					//TODO: workaround fix
					// how to fix without using billboard mode like as ofBitmapFont?
					if (scale != -1.0f) yz_ *= scale;

					////TODO: fix
					//float ratio = 20;
					////float ratio = numberOfSteps / stepSize;
					//if (scale != -1.0f) yz_ *= scale * ratio;

					// floor x and floor z

					// fix flipped
					//(yz), 0, yz, 0;
					s = ofToString(yz);
					pp = doConvertCoord(s, yz_, 0, 0, bOffset);
					font->drawString(s, pp.x, pp.y);


					// fix flipped
					//(-yz), 0, -yz, 0;
					s = ofToString(-yz);
					pp = doConvertCoord(s, -yz_, 0, 0, bOffset);
					font->drawString(s, pp.x, pp.y);

					s = ofToString(yz);
					pp = doConvertCoord(s, 0, 0, yz_, bOffset);
					font->drawString(s, pp.x, pp.y);

					s = ofToString(-yz);
					pp = doConvertCoord(s, 0, 0, -yz_, bOffset);
					font->drawString(s, pp.x, pp.y);
				}
			}

			//--

			// Draw x, y, z labels
			{
				float labelPos = stepSize * (numberOfSteps + 0.5);
				float labelPos_ = labelPos;

				//TODO: workaround fix
				// how to fix without using billboard mode like as ofBitmapFont?
				if (scale != -1.0f) labelPos_ *= scale;

				s = "x";
				pp = doConvertCoord(s, labelPos_, 0, 0, bOffset);
				font->drawString(s, pp.x, pp.y);

				s = "y";
				pp = doConvertCoord(s, 0, labelPos_, 0, bOffset);
				font->drawString(s, pp.x, pp.y);

				s = "z";
				pp = doConvertCoord(s, 0, 0, labelPos_, bOffset);
				font->drawString(s, pp.x, pp.y);
			}
		}

		//ofPopStyle();
		ofSetColor(prevColor);
	};

	//--

	// 3D Scene Grids and Bg Helpers

	//--------------------------------------------------------------
	inline void SurfDrawBgGradient(ofColor c1 = SURFING_RULES_COLOR_BG_1, ofColor c2 = SURFING_RULES_COLOR_BG_2, ofGradientMode g = OF_GRADIENT_CIRCULAR)
	{
		ofBackgroundGradient(c1, c2, g);
	};

	//--------------------------------------------------------------
	inline void SurfDrawFloor(float size, float offsetHeight = 0, ofColor c1 = SURFING_RULES_COLOR_LINES_1, ofColor c2 = SURFING_RULES_COLOR_LINES_2)
	{
		bool bFlipz = true;//make it floor

		bool b = (bFlipz || (offsetHeight != 0));

		if (b) ofPushMatrix();
		ofPushStyle();

		if (offsetHeight != 0) ofTranslate(0, offsetHeight, 0);

		if (bFlipz) ofRotate(90, 0, 0, 1);

		ofSetColor(c1);
		ofDrawGridPlane(size, 1, false);

		ofSetColor(c2);
		ofDrawGridPlane(size / 2.f, 2, false);

		ofPopStyle();
		if (b) ofPopMatrix();
	};

};

//----

// Helper class
// Easily add to an ofApp.

#define SCENE_GRID_SMALL_FONT_RATIO 0.6f

namespace ofxSurfingHelpers
{
	class SurfSceneGrids
	{
	public:
		SurfSceneGrids() {
			setup();
		};

		~SurfSceneGrids() {
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Destructor() Save settings";
			doSave();
		};

		void setCameraPtr(ofCamera* _camera) { camera = _camera; };

	private:
		ofCamera* camera = nullptr;

	public:

		ofParameterGroup params{ "SceneGrids" };

		ofParameter<ofColor> cText{ "C Text", ofColor(250), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBig{ "C Big", ofColor(ofColor::blue, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cQuarter{ "C Quarter", ofColor(ofColor::orange, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cUnits{ "C Units", ofColor(128, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBg1{ "C Bg1", ofColor(128, 128), ofColor(0), ofColor(255,255) };
		ofParameter<ofColor> cBg2{ "C Bg2", ofColor(128, 128), ofColor(0), ofColor(255,255) };

		ofParameter<bool> bEnable{ "Floor Grid", true };
		//ofParameter<bool> bEnable{ "Enable", true };
		ofParameter<bool> bDefaultColors{ "Default Colors", true };
		ofParameter<bool> bForceBitmap{ "Force ofBitmapFont", false };
		ofParameter<bool> bFlipBg{ "Flip Bg", false };
		ofParameter<bool> bEnableBg{ "EnableBg", true };
		ofParameter<bool> bLabels{ "Labels", true };
		ofParameter<bool> bFontSmall{ "Small Font", false };
		ofParameter<bool> bOffsetLabels{ "Offset Labels", false };
		ofParameter<int> gradientType{ "Gradient Type", 1, 0, 2 };

	public:

		// Grid sizes
		float stepSize = 10;//exposed to user
		float numberOfSteps = 10;//exposed to user
		float gridSize = 100;//for debug only

		float scale = 1;//workaround trick to avoid transform errors..

		//--

		void setup() {
			params.add(bEnable);
			params.add(bForceBitmap);
			params.add(bLabels);
			params.add(bFontSmall);
			params.add(bOffsetLabels);
			params.add(bDefaultColors);
			params.add(cText);
			params.add(cBig);
			params.add(cQuarter);
			params.add(cUnits);
			params.add(bEnableBg);
			params.add(gradientType);
			params.add(cBg1);
			params.add(cBg2);
			params.add(bFlipBg);

			float sz = 10;
			string _FONT_FILES_PATH = "assets/fonts/";
			//string path = _FONT_FILES_PATH + string(FONT_FILE_SMALL);
			//string path = _FONT_FILES_PATH + string("JetBrainsMonoNL-ExtraBold.ttf");
			string path = _FONT_FILES_PATH + string("JetBrainsMonoNL-SemiBold.ttf");
			bool b = font.load(path, sz, true);
			if (!b) font.load(OF_TTF_MONO, sz, true);
			sz *= SCENE_GRID_SMALL_FONT_RATIO;
			b = fontSmall.load(path, sz, true);
			if (!b) fontSmall.load(OF_TTF_MONO, sz, true);

			doResetAll();

			doLoad();
		};

		void drawBg()
		{
			if (!bEnable) return;
			if (!bEnableBg) return;

			// Bg rounded gradient
			ofxSurfingHelpers::SurfDrawBgGradient((bFlipBg ? cBg1 : cBg2), (bFlipBg ? cBg2 : cBg1), ofGradientMode(gradientType.get()));
		};

		void drawOutsideCam()//draw labels
		{
			if (!bEnable) return;
			if (!bLabels) return;
			if (bForceBitmap) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

			//workaround 
			// Draw labels, out of the camera
			// custom font
			//if (!bForceBitmap)
			{
				ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

				if (bDefaultColors) ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale);
				else ofxSurfingHelpers::ofxDrawGridPlaneLabelsTrueTypeFont(stepSize, numberOfSteps, f, bOffsetLabels, camera, scale, cText);
			}
		};

		void drawInsideCam() {
			if (!bEnable) return;

			if (camera == nullptr)
			{
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Pointer to camera hast not been passed!";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Call sceneGrid.setCameraPtr(camera) on setup()";
				ofLogError("ofxSurfingHelpers::SurfSceneGrids") << "Forced ofBitmapFont!";
				return;
			}

			gridSize = numberOfSteps * stepSize;

			//axis
			float sza = gridSize / 20.f;
			ofDrawAxis(sza);

			//floor lines: big and quarters
			if (bDefaultColors) ofxSurfingHelpers::SurfDrawFloor(gridSize);
			else ofxSurfingHelpers::SurfDrawFloor(gridSize, 0, cBig.get(), cQuarter.get());

			//floor lines small/units/10ths (and labels when ofBitmapFont)
			if (!bForceBitmap)
			{
				ofTrueTypeFont* f = (bFontSmall.get() ? &fontSmall : &font);

				// custom font
				if (bDefaultColors) ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera);
				else ofxSurfingHelpers::ofxDrawGrid(stepSize, numberOfSteps, bLabels, false, true, false, f, camera, cUnits, cText);
			}
			else
			{
				// bitmap font
				if (bDefaultColors) ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, bLabels, false, true, false);
				else ofxSurfingHelpers::ofxDrawGridBitmapFont(stepSize, numberOfSteps, bLabels, false, true, false, cUnits, cText);
			}
		};

		void doSave() {
			ofxSurfingHelpers::saveGroup(params);
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Setup() Save settings";
		};
		void doLoad() {
			ofxSurfingHelpers::loadGroup(params);
			ofLogNotice("ofxSurfingHelpers::SurfSceneGrids") << "Setup() Load settings";
		};

		void doResetSettings() {
			bEnable = true;
			bEnableBg = true;
			bFlipBg = false;
			bLabels = true;
			bFontSmall = true;
			bOffsetLabels = false;
		};

		void doResetColors() {
			cText = ofColor{ 255, 255, 255, 200 };
			cBig = ofColor{ 96, 96, 96, 150 };
			cQuarter = ofColor{ 64, 64, 64, 150 };
			cUnits = ofColor{ 96, 96, 96, 24 };
			cBg1 = ofColor{ 40, 40, 40 };
			cBg2 = ofColor{ 0, 0, 0 };
		};

		void doResetAll() {
			doResetSettings();
			doResetColors();
		};

	private:
		ofTrueTypeFont font;
		ofTrueTypeFont fontSmall;
	};

	//--

	// EXAMPLE GUI:
/*
//--------------------------------------------------------------
void ofApp::drawImGuiSceneGrids()
{
	if (ui.BeginWindow(bGui))
	{
		static bool bInit = false;
		if (!bInit) {
			bInit = true;
			ui.AddStyle(sceneGrid.bEnable, OFX_IM_TOGGLE);
			ui.AddStyle(sceneGrid.bEnableBg, OFX_IM_TOGGLE);
			ui.AddStyle(sceneGrid.bForceBitmap, OFX_IM_TOGGLE_SMALL_BORDER_BLINK);
			ui.AddStyle(sceneGrid.bLabels, OFX_IM_TOGGLE_ROUNDED_SMALL);
			//ui.AddStyle(bGridInFront, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui.AddStyle(sceneGrid.cBg1, OFX_IM_COLOR_INPUTS_NO_ALPHA);
			ui.AddStyle(sceneGrid.cBg2, OFX_IM_COLOR_INPUTS_NO_ALPHA);	
		}

		ui.AddLabelHuge("Scene\n3dRules");
		ui.AddSpacing();

		ui.AddGroup(sceneGrid.params);
		ui.AddSpacing();
		
		if (ui.AddButton("Reset", OFX_IM_BUTTON, 3, true)) {
			sceneGrid.doResetSettings();
		}
		if (ui.AddButton("Reset Colors", OFX_IM_BUTTON, 3, true)) {
			sceneGrid.doResetColors();
		}
		if (ui.AddButton("Reset All", OFX_IM_BUTTON, 3, false)) {
			sceneGrid.doResetAll();
		}

		if (ui.AddButton("Save", OFX_IM_BUTTON, 2)) {
			sceneGrid.doSave();
		}
		ui.SameLine();
		if (ui.AddButton("Load", OFX_IM_BUTTON, 2)) {
			sceneGrid.doLoad();
		}
		ui.AddSpacing();

		string s = "DEBUG";
		s += "\nstepSize: " + ofToString(sceneGrid.stepSize);
		s += "\numberOfSteps: " + ofToString(sceneGrid.numberOfSteps);
		s += "\nsize: " + ofToString(sceneGrid.gridSize);
		s += "\nscale: " + ofToString(sceneGrid.scale);
		ui.AddLabel(s);

		ui.EndWindow();
	}
}
*/

}
