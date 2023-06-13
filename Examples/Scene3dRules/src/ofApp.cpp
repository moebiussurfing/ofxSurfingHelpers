#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofxSurfingHelpers::setMonitorsLayout(-1, true, true);

	ofSetCircleResolution(200);

	sceneGrid.setCameraPtr(&cam);
	sceneGrid.setPathGlobal("data");

	g.add(bGui);
	g.add(scale);
	g.add(zoom);
	g.add(bRotate);
	g.add(bObject);
	g.add(bFlipColors);
	ofxSurfingHelpers::load(g);

	cam.setup();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	ofxSurfingHelpers::save(g);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 0) + " FPS");
	
	{
		// Enables mouse cam control only when mouse is not over gui.
		bool b = ui.isMouseOverGui();
		if (b && cam.getMouseInputEnabled()) {
			cam.disableMouseInput();
		}
		else if (!b && !cam.getMouseInputEnabled()) {
			cam.enableMouseInput();
		}
	}

	sceneGrid.drawBg();

	//--

	ofEnableDepthTest();
	cam.begin();
	{
		// Camera transforms will need fixes!
		float _scaleTotal = 1.f;

		//--

		// Zoom
		{
			const float zmax = 3.f;//max range
			float _zoom = 1.f;//mapped
			if (zoom != 0) _zoom = ofMap(zoom, -1, 1, 1 / zmax, zmax);

#define DO_SMOOTH 1
#if(DO_SMOOTH==1)
			static float _zoomo = 1.f;
			float smooth = 0.94f;
			ofxSurfingHelpers::ofxKuValueSmooth(_zoomo, _zoom, smooth);

			ofScale(_zoomo);
			_scaleTotal *= _zoomo;
#else
			ofScale(_zoom);
			_scaleTotal *= _zoom;
#endif
		}

		//--

		// Scale

		ofScale(scale);
		_scaleTotal *= scale;

		//--

		//TODO: WIP: note that rotation transform can be done to the camera..
		// to allow this, we need to disable ttf and to use bitmap fonts!
		if (bRotate) ofRotateY(-ofWrap(ofGetFrameNum() * 0.25f, 0, 359));

		//--

		// 3D object
		if (bObject)
		{
			//ofEnableDepthTest();

			ofPushMatrix();
			ofPushStyle();

			ofNoFill();
			if (bFlipColors) ofSetColor(0, 255);
			else ofSetColor(255, 255);

			ofRotateX(-180);
			ofRotateY(ofWrap(ofGetFrameNum(), 0, 359));
			ofSetConeResolution(3, 1, 1);
			float sz = 40;
			ofTranslate(0, -sz, 0);
			ofDrawCone(sz * 0.618, sz);

			ofPopStyle();
			ofPopMatrix();
		}

		//--

		//ofDisableDepthTest();//optional
		sceneGrid.scale = _scaleTotal;//workaround trick

		sceneGrid.drawInsideCam();
	}
	cam.end();

	ofDisableDepthTest();
	cam.drawInteractionArea();
	cam.drawHelpText();

	//ofDisableDepthTest();//optional
	sceneGrid.drawOutsideCam();

	//--

	//ofDisableDepthTest();
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui()
{
	if (!bGui) return;

	ui.Begin();
	{
		cam.drawImGui(ui);

		if (ui.BeginWindow(bGui))
		{
			ui.AddLabelBig("Scene");
			ui.Add(bObject, OFX_IM_TOGGLE_SMALL);
			ui.Add(bFlipColors);

			ui.AddSpacingSeparated();

			ui.AddLabelBig("Camera");
			ui.Add(cam.bGui, OFX_IM_TOGGLE_ROUNDED);
			ui.Add(zoom, OFX_IM_HSLIDER_SMALL);
			ui.Add(bRotate);
			ui.AddSpacing();

			ui.Add(scale);
			ui.Add(scale, OFX_IM_STEPPER);
			if (ui.AddButton("Reset Scale"))
			{
				scale = 1;
			}

#ifdef USE_IM_GUI__SCENE
			ui.AddSpacingSeparated();

			ui.Add(sceneGrid.bGui, OFX_IM_TOGGLE_ROUNDED);
#endif
			ui.EndWindow();
		}

		//-

#ifdef USE_IM_GUI__SCENE
		sceneGrid.drawImGuiWindow(ui);
#endif
	}
	ui.End();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') bGui = !bGui;
	if (key == OF_KEY_RETURN) sceneGrid.bForceBitmap = !sceneGrid.bForceBitmap;

	cam.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	cam.keyReleased(key);
}