#pragma once
#include "ofMain.h"

/*

	TODO:

*/

//#define USE_SURFING_DEBUG_ALPHA // -> uncomment for testing alpha problems between lines

//--

// WARNING!
// This is an attempt to draw 3d planes but not using ofBitmapFont for the labels,
// and using ofTrueTypeFonts instead!
// One drawback is that we need to handle a trick to handle scale transformation on the camera!
// https://forum.openframeworks.cc/t/how-to-draw-oftruetype-labels-inside-an-ofcamera-as-ofdrawbitmapstring-does/41542/4

#include "ofxSurfingHelpers.h"

namespace ofxSurfingHelpers
{
    // Default Colors
#define DEBUG_COLORS__SCENE 0
#if(DEBUG_COLORS__SCENE==0)

    // Grey scale
    // dark
    // static const ofColor SURFING_RULES_COLOR_TEXT = ofColor{255, 255, 255, 200};
    // static const ofColor SURFING_RULES_COLOR_LINES_BIG = ofColor{128, 128, 128, 150};
    // static const ofColor SURFING_RULES_COLOR_LINES_QUARTER = ofColor{96, 96, 96, 150};
    // static const ofColor SURFING_RULES_COLOR_LINES_SIXTEENTH = ofColor{64, 64, 64, 150};
    // static const ofColor SURFING_RULES_COLOR_LINES_UNITS = ofColor{64, 64, 64, 32};
    // static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{ 40, 40, 40 };
    // static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{ 0, 0, 0 };
    // light
	static const ofColor SURFING_RULES_COLOR_TEXT = ofColor{0, 0, 0, 200};
	static const ofColor SURFING_RULES_COLOR_LINES_BIG = ofColor{0, 0, 0, 150};
	static const ofColor SURFING_RULES_COLOR_LINES_QUARTER = ofColor{24, 24, 24, 55};
	static const ofColor SURFING_RULES_COLOR_LINES_SIXTEENTH = ofColor{48, 48, 48, 66};
	static const ofColor SURFING_RULES_COLOR_LINES_UNITS = ofColor{64, 64, 64, 22};
	static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{20, 20, 20};
	static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{130, 130, 130};

    //// Green scale
    //static const ofColor SURFING_RULES_COLOR_TEXT = ofColor{ 255, 255, 255, 200 };
    //static const ofColor SURFING_RULES_COLOR_LINES_BIG = ofColor{ 0, 255, 175, 150 };
    //static const ofColor SURFING_RULES_COLOR_LINES_QUARTER = ofColor{ 0, 255, 203, 75 };
    //static const ofColor SURFING_RULES_COLOR_LINES_SIXTEENTH = ofColor{ 181, 181, 181, 48 };
    //static const ofColor SURFING_RULES_COLOR_LINES_UNITS = ofColor{ 0, 0, 0, 24 };
    //static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{ 0, 0, 0 };
    //static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{ 64, 64, 64 };
#else
	// Debug colors. Weird colors to have big contrast
	static const char a = 65;
	static const ofColor SURFING_RULES_COLOR_TEXT = ofColor(0, 255, 255, a);
	static const ofColor SURFING_RULES_COLOR_LINES_BIG = ofColor(255, 0, 0, a);//big
	static const ofColor SURFING_RULES_COLOR_LINES_QUARTER = ofColor(0, 255, 0, a);//quarter
	static const ofColor SURFING_RULES_COLOR_LINES_SIXTEENTH = ofColor(0, 0, 255, a);//sixteenth
	static const ofColor SURFING_RULES_COLOR_LINES_UNITS = ofColor(255, 255, 0, a);//units
	static const ofColor SURFING_RULES_COLOR_BG_1 = ofColor{ 70, 70, 70 };
	static const ofColor SURFING_RULES_COLOR_BG_2 = ofColor{ 10, 10, 10 };
#endif

    //--

    // Drawing methods. Mostly based on OF internals.

    //--------------------------------------------------------------
    inline void ofxDrawString(string s, float x, float y, float z, ofTrueTypeFont* font, ofCamera* camera)
    {
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
    inline void ofxDrawGridPlane(float stepSize, size_t numberOfSteps, bool labels, ofTrueTypeFont* font,
                                 ofCamera* camera, ofColor c1 = SURFING_RULES_COLOR_LINES_SIXTEENTH,
                                 ofColor c2 = SURFING_RULES_COLOR_TEXT)
    {
        ofPushStyle();

        auto renderer = ofGetCurrentRenderer();

        //#ifdef USE_SURFING_DEBUG_ALPHA
        //TODO: fixing overlap transparencies..
        ofEnableAlphaBlending();
        ofEnableDepthTest();
        renderer->setBlendMode(OF_BLENDMODE_ALPHA);
        //#endif

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
                else if (i % 2 == 0)
                {
                    renderer->setLineWidth(1.5); // major
                }
                else
                {
                    renderer->setLineWidth(1); // minor
                }

                if (iDimension == 0)
                {
                    renderer->drawLine(0, yz, -scale, 0, yz, scale);
                    if (yz != 0) renderer->drawLine(0, -yz, -scale, 0, -yz, scale);
                }
                else
                {
                    renderer->drawLine(0, -scale, yz, 0, scale, yz);
                    if (yz != 0) renderer->drawLine(0, -scale, -yz, 0, scale, -yz);
                }
            }
        }
        renderer->setLineWidth(lineWidth);

        // Draw all the labels

        if (labels)
        {
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
    inline void ofxDrawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z,
                            ofTrueTypeFont* font, ofCamera* camera, bool enableLines = true,
                            ofColor c1 = SURFING_RULES_COLOR_LINES_UNITS, ofColor c2 = SURFING_RULES_COLOR_TEXT)
    {
        //TODO: must implement disable enableLines! 

        auto renderer = ofGetCurrentRenderer();

#ifdef USE_SURFING_DEBUG_ALPHA
		//TODO: fixing overlap transparencies..
		ofEnableAlphaBlending();
		ofEnableDepthTest();
		renderer->setBlendMode(OF_BLENDMODE_ALPHA);
#endif

        //ofPushStyle();
        ofColor prevColor = ofGetStyle().color;

        ofSetColor(c1);

        // lines with each plane labels

        if (font == nullptr || camera == nullptr)
        {
            // bitmap
            if (x)
            {
                ofDrawGridPlane(stepSize, numberOfSteps, labels);
            }
            if (y)
            {
                glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
                renderer->pushMatrix();
                renderer->multMatrix(m);
                ofDrawGridPlane(stepSize, numberOfSteps, labels);
                renderer->popMatrix();
            }
            if (z)
            {
                glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
                renderer->pushMatrix();
                renderer->multMatrix(m);
                ofDrawGridPlane(stepSize, numberOfSteps, labels);
                renderer->popMatrix();
            }
        }
        else
        {
            // custom ttf

            if (x)
            {
                ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
            }
            if (y)
            {
                glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
                ofPushMatrix();
                ofMultMatrix(m);
                ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
                ofPopMatrix();
            }
            if (z)
            {
                glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
                ofPushMatrix();
                ofMultMatrix(m);
                ofxDrawGridPlane(stepSize, numberOfSteps, labels, font, camera, c1, c2);
                ofPopMatrix();
            }
        }

        if (labels)
        {
            // xyz
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
    inline void ofxDrawGridBitmapFont(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z,
                                      bool enableLines = true, ofColor c1 = SURFING_RULES_COLOR_LINES_UNITS,
                                      ofColor c2 = SURFING_RULES_COLOR_TEXT)
    {
        ofxDrawGrid(stepSize, numberOfSteps, labels, x, y, z, nullptr, nullptr, enableLines, c1, c2);
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
    inline void ofxDrawGridPlaneLabelsTrueTypeFont(float stepSize, size_t numberOfSteps, ofTrueTypeFont* font,
                                                   bool bOffset, ofCamera* camera, float scale = -1.0f,
                                                   ofColor c = SURFING_RULES_COLOR_TEXT)
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
        auto doConvertCoord = [&](const std::string& _s, const int _x, const int _y, const int _z,
                                  const bool _bOffset = false)
        {
            const ofRectangle _r = ofGetCurrentViewport();
            glm::vec3 _p = camera->worldToScreen(glm::vec3(_x, _y, _z), _r);

            if (_bOffset)
            {
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

                    float yz_ = yz; //for labels without scale fix!

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

                    //TODO: remove unnecessary decimals
                    int ndigits;
                    auto nDigitsFor = [](float v)
                    {
                        int n = 1; //default
                        if (std::floor(v) == v) //isInt 
                            n = 0;
                        return n;
                    };

                    // fix flipped
                    //(yz), 0, yz, 0;
                    ndigits = nDigitsFor(yz);
                    s = ofToString(yz, ndigits);
                    pp = doConvertCoord(s, yz_, 0, 0, bOffset);
                    font->drawString(s, pp.x, pp.y);


                    // fix flipped
                    //(-yz), 0, -yz, 0;
                    ndigits = nDigitsFor(yz);
                    s = ofToString(-yz, ndigits);
                    pp = doConvertCoord(s, -yz_, 0, 0, bOffset);
                    font->drawString(s, pp.x, pp.y);

                    ndigits = nDigitsFor(yz);
                    s = ofToString(yz, ndigits);
                    pp = doConvertCoord(s, 0, 0, yz_, bOffset);
                    font->drawString(s, pp.x, pp.y);

                    ndigits = nDigitsFor(yz);
                    s = ofToString(-yz, ndigits);
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
    inline void ofxDrawBgGradient(ofColor c1 = SURFING_RULES_COLOR_BG_1, ofColor c2 = SURFING_RULES_COLOR_BG_2,
                                  ofGradientMode g = OF_GRADIENT_CIRCULAR)
    {
        ofBackgroundGradient(c1, c2, g);
    };

    //--------------------------------------------------------------
    inline void ofxDrawFloor(float size, bool bEnable1 = true, bool bEnable2 = true,
                             ofColor c1 = SURFING_RULES_COLOR_LINES_QUARTER,
                             ofColor c2 = SURFING_RULES_COLOR_LINES_SIXTEENTH, float offsetHeight = 0)
    {
        bool bFlipz = true; //make it floor
        bool b = (bFlipz || (offsetHeight != 0));
        if (b) ofPushMatrix();
        ofPushStyle();

        if (offsetHeight != 0) ofTranslate(0, offsetHeight, 0);
        if (bFlipz) ofRotate(90, 0, 0, 1);

        {
            if (bEnable1)
            {
                ofSetColor(c1);
                ofDrawGridPlane(size, 1, false);
            }

            if (bEnable2)
            {
                ofSetColor(c2);
                ofDrawGridPlane(size / 2.f, 2, false);
            }
        }

        ofPopStyle();
        if (b) ofPopMatrix();
    };

    //--------------------------------------------------------------
    inline void ofxDrawFloorRectangle(float gridSize)
    {
        float scale = gridSize;

        auto renderer = ofGetCurrentRenderer();

        float lineWidth = renderer->getStyle().lineWidth;
        renderer->setLineWidth(2);

#ifdef USE_SURFING_DEBUG_ALPHA
		//TODO: fixing overlap transparencies..
		ofEnableAlphaBlending();
		ofEnableDepthTest();
		renderer->setBlendMode(OF_BLENDMODE_ALPHA);
#endif

        renderer->drawLine(scale, 0, -scale, scale, 0, scale);
        renderer->drawLine(-scale, 0, -scale, -scale, 0, scale);
        renderer->drawLine(-scale, 0, scale, scale, 0, scale);
        renderer->drawLine(-scale, 0, -scale, scale, 0, -scale);

        renderer->setLineWidth(lineWidth);
    }


    //--------------------------------------------------------------
    inline void ofxDrawGridPlaneLabelsBitmapFonts(float stepSize, size_t numberOfSteps,
                                                  ofColor c = SURFING_RULES_COLOR_TEXT)
    {
        float scale = stepSize * numberOfSteps;
        auto renderer = ofGetCurrentRenderer();
        float lineWidth = renderer->getStyle().lineWidth;

        //draw numbers on axes
        ofColor prevColor = renderer->getStyle().color;
        ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;

        renderer->setColor(c);
        //renderer->setColor(255, 255, 255);

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


    //--------------------------------------------------------------
    inline void ofxDrawGridLabelsBitmapFonts(float stepSize, size_t numberOfSteps, bool x, bool y, bool z,
                                             ofColor c = SURFING_RULES_COLOR_TEXT)
    {
        auto renderer = ofGetCurrentRenderer();

        ofColor prevColor = ofGetStyle().color;

        if (x)
        {
            ofxDrawGridPlaneLabelsBitmapFonts(stepSize, numberOfSteps, c);
        }
        if (y)
        {
            glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 0, -1));
            renderer->pushMatrix();
            renderer->multMatrix(m);
            ofxDrawGridPlaneLabelsBitmapFonts(stepSize, numberOfSteps, c);
            renderer->popMatrix();
        }
        if (z)
        {
            glm::mat4 m = glm::rotate(glm::mat4(1.0), glm::half_pi<float>(), glm::vec3(0, 1, 0));
            renderer->pushMatrix();
            renderer->multMatrix(m);
            ofxDrawGridPlaneLabelsBitmapFonts(stepSize, numberOfSteps, c);
            renderer->popMatrix();
        }

        ofSetColor(c); //color is not internal

        // bitmap
        ofDrawBitmapMode mode = ofGetStyle().drawBitmapMode;
        float labelPos = stepSize * (numberOfSteps + 0.5);

        ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
        ofDrawBitmapString("x", labelPos, 0, 0);
        ofDrawBitmapString("y", 0, labelPos, 0);
        ofDrawBitmapString("z", 0, 0, labelPos);

        ofSetDrawBitmapMode(mode);

        ofSetColor(prevColor);
    };
};
