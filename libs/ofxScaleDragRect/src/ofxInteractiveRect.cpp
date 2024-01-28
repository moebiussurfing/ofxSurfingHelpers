//
//  interactiveRect.cpp
//
//  Created by Roy Macdonald on 8/15/12.
//  Copyright (c) 2012 micasa. All rights reserved.
//
//  Updated by Aaron Richards on 13/05/2014.
//	Updated by moebiusSurfing 2021

#include "ofxInteractiveRect.h"

//--------------------------------------------------------------
ofxInteractiveRect::ofxInteractiveRect(string name, string path) {
	ofAddListener(ofEvents().exit, this, &ofxInteractiveRect::exit, OF_EVENT_ORDER_AFTER_APP);

	bEditMode.addListener(this, &ofxInteractiveRect::Changed_EditMode);
	rectParam.addListener(this, &ofxInteractiveRect::Changed_Rect);

	bLockResize = false;
	bLock = false;

	bIsEditing = false;
	bMove = false;
	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;
	bIsOver = false;
	this->name = name;
	//this->path = "";

	setRect(10, 10, 400, 400);
	if (bAutoSave) loadSettings();

	// Some gray coloring
	setColorBorderDraggable(ofColor(0, 128));
	setColorEditingPressedBorder(ofColor(0, 128));
	setColorEditingMoving(ofColor(128, 32));

	doConstraints();
}

//--------------------------------------------------------------
ofxInteractiveRect::~ofxInteractiveRect() {
	ofRemoveListener(ofEvents().exit, this, &ofxInteractiveRect::exit);
}

//--------------------------------------------------------------
void ofxInteractiveRect::exit(ofEventArgs & args) {
	exit();
}

//--------------------------------------------------------------
void ofxInteractiveRect::exit() {

	bEditMode.removeListener(this, &ofxInteractiveRect::Changed_EditMode);
	rectParam.removeListener(this, &ofxInteractiveRect::Changed_Rect);

	if (bAutoSave) saveSettings();
}

//--------------------------------------------------------------
void ofxInteractiveRect::enableEdit(bool enable) {
	ofLogVerbose("ofxInteractiveRect") << " " << (__FUNCTION__) << "Rect " << this->name << " edit : " << (string)(enable ? "true" : "false");

	if (enable != bIsEditing) {
		if (enable) {
			ofRegisterMouseEvents(this);
		} else {
			ofUnregisterMouseEvents(this);
			//saveSettings();
		}
		bIsEditing = enable;

		bEditMode.setWithoutEventNotifications(bIsEditing);
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::disableEdit() {
	enableEdit(false);
}

//--------------------------------------------------------------
void ofxInteractiveRect::toggleEdit() {
	enableEdit(!bIsEditing);
}

//--------------------------------------------------------------
ofRectangle ofxInteractiveRect::getRect() {
	return ofRectangle(x, y, width, height);
}

//--------------------------------------------------------------
void ofxInteractiveRect::saveSettings(string name, string path, bool bUseJson) {
	if (name != "") {
		this->name = name;
	}

	if (path != "") {
		this->path = path;
	}

	string filename = this->path + this->name + "_" + prefixName;

	if (bUseJson) {
		filename += ".json";
		ofSaveJson(filename, saveToJson());
	} else {
		filename += ".xml";
		saveToXml().save(filename);
	}

	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << filename;
}

//--------------------------------------------------------------
ofJson ofxInteractiveRect::saveToJson() {
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__);

	ofJson j; //("interactiveRect");

	j["x"] = this->ofRectangle::x;
	j["y"] = this->ofRectangle::y;
	j["width"] = this->ofRectangle::width;
	j["height"] = this->ofRectangle::height;
	j["name"] = this->name;
	j["path"] = this->path;
	j["isEditing"] = this->bIsEditing;

	return j;
}

//--------------------------------------------------------------
void ofxInteractiveRect::loadFromJson(const ofJson & j) {
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__);

	if (j == nullptr) return; //TODO: crash

	j["x"].get_to(this->ofRectangle::x);
	j["y"].get_to(this->ofRectangle::y);
	j["width"].get_to(this->ofRectangle::width);
	j["height"].get_to(this->ofRectangle::height);
	j["name"].get_to(this->name);
	j["path"].get_to(this->path);
	bool editing;

	j["isEditing"].get_to(editing);
	enableEdit(editing);

	doConstraints();
}

//--------------------------------------------------------------
ofXml ofxInteractiveRect::saveToXml() {
	ofXml xml;
	auto r = xml.appendChild("interactiveRect");

	r.appendChild("path").set(this->path);
	r.appendChild("x").set(this->ofRectangle::x);
	r.appendChild("y").set(this->ofRectangle::y);
	r.appendChild("width").set(this->ofRectangle::width);
	r.appendChild("height").set(this->ofRectangle::height);
	r.appendChild("name").set(this->name);
	r.appendChild("isEditing").set(this->bIsEditing);

	return xml;
}

//--------------------------------------------------------------
bool ofxInteractiveRect::loadFromXml(const ofXml & xml) {
	auto r = xml.getChild("interactiveRect");
	if (r) {
		this->path = r.getChild("path").getValue();
		this->ofRectangle::x = r.getChild("x").getFloatValue();
		this->ofRectangle::y = r.getChild("y").getFloatValue();
		this->ofRectangle::width = r.getChild("width").getFloatValue();
		this->ofRectangle::height = r.getChild("height").getFloatValue();
		this->name = r.getChild("name").getValue();
		enableEdit(r.getChild("isEditing").getBoolValue());

		doConstraints();

		return true;
	}

	return false;
}

//--------------------------------------------------------------
bool ofxInteractiveRect::loadSettings(string name, string path, bool bUseJson) {
	if (name != "") {
		this->name = name;
	}
	if (path != "") {
		this->path = path;
	}

	//string filename = this->path + prefixName + this->name;
	string filename = this->path + this->name + "_" + prefixName;

	if (bUseJson) {
		filename += ".json";

		//avoid crash
		ofFile file(filename);
		if (!file.exists()) {
			return false;
		}

		loadFromJson(ofLoadJson(filename));
		return true;

	} else {
		filename += ".xml";

		//avoid crash
		ofFile file(filename);
		if (!file.exists()) {
			return false;
		}

		ofXml xml;
		if (xml.load(filename)) {
			if (loadFromXml(xml)) {
				return true;
			}
		}
	}

	ofLogVerbose("ofxInteractiveRect") << " " << (__FUNCTION__) << "unable to load : " << filename;
	rectParam.set(this->getRect()); //?

	doConstraints();

	return false;
}

//TODO:
//--------------------------------------------------------------
void ofxInteractiveRect::drawBorder() {
	if (bTransparent) return;

	//--

	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2.0);
	if (bEditMode)
		ofSetColor(ofColor(colorEditingMoving.r, colorEditingMoving.g, colorEditingMoving.b, colorEditingMoving.a * Bounce()));
	else
		ofSetColor(colorEditingMoving);

	if (bRounded)
		ofDrawRectRounded(*this, rounded);
	else
		ofDrawRectangle(*this);

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxInteractiveRect::draw() {
	if (bTransparent) return;

	//--

	if (bIsEditing) {
		ofPushStyle();

		// Hover

		if (bIsOver) {
			// Border

			ofNoFill();
			if (!bPressed) {
				//ofSetColor(ofColor(ofColor::orange, 128));
				ofSetColor(colorEditingHover);
			} else {
				//ofSetColor(ofColor(ofColor::red, 128));
				ofSetColor(colorEditingPressedBorder);
			}

			if (bRounded)
				ofDrawRectRounded(*this, rounded);
			else
				ofDrawRectangle(*this);
		}

		// Border

		//else
		{
			drawBorder(); // will blink
		}

		//--

		// Fill

		ofFill();

		bool bDual = (bUp || bDown) && (bLeft || bRight);

		if (bMove) // when moving
		{
			ofSetColor(colorEditingMoving);

			if (bRounded)
				ofDrawRectRounded(*this, rounded);
			else
				ofDrawRectangle(*this);
		} else // when static
		{
			// Draggable borders

			ofSetColor(colorBorderDraggable.r, colorBorderDraggable.g, colorBorderDraggable.b, colorBorderDraggable.a * 0.5);

			float xx, yy, ww, hh;

			// height
			if (bUp) {
				// x, y, width, here are "inherited" from ofRectangle
				ww = width;
				hh = height;
				xx = x;
				yy = y;

				if (bRounded)
					ofDrawRectRounded(xx, yy, ww, BORDER_DRAG_SIZE, rounded);
				else
					ofDrawRectangle(xx, yy, ww, BORDER_DRAG_SIZE);
			} else if (bDown) {
				ww = width;
				hh = height;
				xx = x;
				yy = y + hh - BORDER_DRAG_SIZE;

				if (bRounded)
					ofDrawRectRounded(xx, yy, ww, BORDER_DRAG_SIZE, rounded);
				else
					ofDrawRectangle(xx, yy, ww, BORDER_DRAG_SIZE);
			}

			// width
			if (bLeft) {
				ww = width;
				hh = height;
				xx = x;
				yy = y;

				if (bDual) {
					if (bUp) {
						yy = y + BORDER_DRAG_SIZE;
						hh = height - BORDER_DRAG_SIZE;
					} else if (bDown) {
						yy = y;
						hh = height - BORDER_DRAG_SIZE;
					}
				}

				if (bRounded)
					ofDrawRectRounded(xx, yy, BORDER_DRAG_SIZE, hh, rounded);
				else
					ofDrawRectangle(xx, yy, BORDER_DRAG_SIZE, hh);
			} else if (bRight) {
				ww = width;
				hh = height;
				xx = x + width - BORDER_DRAG_SIZE;
				yy = y;

				if (bDual) {
					if (bUp) {
						yy = y + BORDER_DRAG_SIZE;
						hh = height - BORDER_DRAG_SIZE;
					} else if (bDown) {
						yy = y;
						hh = height - BORDER_DRAG_SIZE;
					}
				}

				if (bRounded)
					ofDrawRectRounded(xx, yy, BORDER_DRAG_SIZE, hh, rounded);
				else
					ofDrawRectangle(xx, yy, BORDER_DRAG_SIZE, hh);
			}
		}

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseMoved(ofMouseEventArgs & mouse) {
	if (bLock) return;

	if (!bPressed) {
		bIsOver = inside(mouse.x, mouse.y);

		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;

		if (bIsOver) {
			bMove = true;

			if (!bLockResize) {
				if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW) {
					bLeft = true;
					bMove = false;
				} else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW) {
					bRight = true;
					bMove = false;
				}

				if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH) {
					bUp = true;
					bMove = false;
				} else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH) {
					bDown = true;
					bMove = false;
				}
			}
		} else {
			bMove = false;
		}
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mousePressed(ofMouseEventArgs & mouse) {
	if (bLock) return;
	if (!bEditMode) return;
	if (!this->isMouseOver()) return;

	mousePrev = mouse;
	bPressed = true;

	if (!bAllScreenMouse)
		bIsOver = inside(mouse.x, mouse.y);
	else
		bIsOver = true;

	bLeft = false;
	bRight = false;
	bUp = false;
	bDown = false;

	if (bIsOver) {
		bMove = true;

		if (!bLockResize) {
			if (mouse.x < x + BORDER_DRAG_SIZE && mouse.x > x && !bLockW) {
				bLeft = true;
				bMove = false;
			} else if (mouse.x < x + width && mouse.x > x + width - BORDER_DRAG_SIZE && !bLockW) {
				bRight = true;
				bMove = false;
			}
			if (mouse.y > y && mouse.y < y + BORDER_DRAG_SIZE && !bLockH) {
				bUp = true;
				bMove = false;
			} else if (mouse.y > y + height - BORDER_DRAG_SIZE && mouse.y < y + height && !bLockH) {
				bDown = true;
				bMove = false;
			}
		}
	} else {
		bMove = false;
	}
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseDragged(ofMouseEventArgs & mouse) {
	if (bLock) return;
	if (!bEditMode) return;

	float _x = mouse.x;
	float _y = mouse.y;

	// skip
	if (_x < 0 || _y < 0) {
		mousePrev = mouse;
		return;
	}
	if (_x > ofGetWidth() || _y > ofGetHeight()) {
		mousePrev = mouse;
		return;
	}

	////TODO:
	//_x = MAX(0, _x);
	//_y = MAX(0, _y);
	//cout << _x << "," << _y << endl;

	//_x = ofClamp(_x, xpad, ofGetWidth() - xpad);
	//_y = ofClamp(_y, ypad, ofGetHeight() - ypad);

	diffx = _x - mousePrev.x;
	diffy = _y - mousePrev.y;

	//TODO:
	// fix weird behavior
	// try to limit diff previously
	//diffx = ofClamp(diffx, xpad, ofGetHeight() - xpad);

	//--

	if (!bLockResize) //
	{
		if (bUp && !bLockY) {
			y += diffy;

			y = ofClamp(y, ypad, ofGetHeight() - ypad);

			height -= diffy;

			if (bLockAspectRatio) width = aspectRatio * height;
		} else if (bDown && !bLockH) {
			height += diffy;
			height = MIN(height, ofGetHeight() - y - 2 * ypad);

			if (bLockAspectRatio) width = aspectRatio * height;
		}

		//--

		if (bLeft && !bLockX) {
			x += diffx;

			x = ofClamp(x, xpad, ofGetWidth() - xpad);

			width += -diffx;

			if (bLockAspectRatio) height = width / aspectRatio;
		} else if (bRight && !bLockW) {
			if (diffx + width > (x + width + diffx)) {
			}

			width += diffx;
			//width = MIN(width, ofGetWidth() - x - 2 * xpad);

			if (bLockAspectRatio) height = width / aspectRatio;
		}
	}

	//--

	if (bMove) {
		x += diffx;
		y += diffy;

		// clamp
		x = ofClamp(x, xpad, ofGetWidth() - xpad);
		y = ofClamp(y, ypad, ofGetHeight() - ypad);
	}

	doConstraints();

	//--

	mousePrev = mouse;
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseReleased(ofMouseEventArgs & mouse) {
	if (bLock) return;
	if (!bEditMode) return;
	//if (!this->isMouseOver()) return;

	if (!bLockResize) {
		bLeft = false;
		bRight = false;
		bUp = false;
		bDown = false;
	}

	bMove = false;
	bPressed = false;

	doConstraints();

	rectParam.setWithoutEventNotifications(this->getRect());
}

//--------------------------------------------------------------
void ofxInteractiveRect::mouseScrolled(ofMouseEventArgs & mouse) {
	if (!this->isMouseOver()) return;
	if (!bEnableMouseWheel) return;
	if (!bEditMode) return;
	if (bLockResize) return;

	//--

	//// Skip window oversize
	////TODO: workaround
	//const float gap = 20;
	//float xgap = 2 * xpad + gap;
	//float ygap = 2 * ypad + gap;
	//
	//// up
	//if (mouse.scrollY > 0)
	//{
	//	if ((width >= ofGetWidth() - xgap) || (height >= ofGetHeight() - ygap))
	//	{
	//		if (bLockAspectRatio) height = width / aspectRatio;
	//		doConstraints();
	//		return;
	//	}
	//}

	//--

	float d = 0.1f;
	float s = ofMap(mouse.scrollY, -2, 2, 1.f - d, 1.f + d);

	bool bKeyModShift = ofGetKeyPressed(OF_KEY_LEFT_SHIFT);
	bool bKeyModCtrl = ofGetKeyPressed(OF_KEY_CONTROL);
	bool bKeyModAlt = ofGetKeyPressed(OF_KEY_ALT);

	if (bKeyModCtrl && !bLockW && !bLockH)
		this->scaleFromCenter(s);
	else if (bKeyModShift && !bLockW)
		this->scale(s, 1);
	else if (bKeyModAlt && !bLockH)
		this->scale(1, s);
	else //no mod
	{
		if (!bLockW) this->scale(s, 1);
		if (!bLockH) this->scale(1, s);
	}

	if (bLockAspectRatio) height = width / aspectRatio;

	doConstraints();
}

//--------------------------------------------------------------
void ofxInteractiveRect::doConstraints() {
	//TODO:
	//return;

	// Apply constraints if defined
	if (bConstrainedMin) {
		this->width = MAX(width, shapeConstraintMin.x);
		this->height = MAX(height, shapeConstraintMin.y);
	}
	if (bConstrainedMax) {
		this->width = MIN(width, shapeConstraintMax.x);
		this->height = MIN(height, shapeConstraintMax.y);
	}

	// Clamp size
	if (this->getWidth() > ofGetWidth() - 2 * xpad) {
		this->setWidth(ofGetWidth() - 2 * xpad);
	} else if (this->getHeight() > ofGetHeight() - 2 * ypad) {
		this->setHeight(ofGetHeight() - 2 * ypad);
	}

	//// Clamp pad to borders //fails on text widget
	//this->x = ofClamp(x, xpad, ofGetWidth() - xpad - width);
	//this->y = ofClamp(y, ypad, ofGetHeight() - ypad - height);
	// Clamp pad to borders
	this->x = ofClamp(x, xpad, ofGetWidth());
	this->y = ofClamp(y, ypad, ofGetHeight());
}

void ofxInteractiveRect::mouseEntered(ofMouseEventArgs & mouse) { }
void ofxInteractiveRect::mouseExited(ofMouseEventArgs & mouse) { }

//--------------------------------------------------------------
void ofxInteractiveRect::Changed_EditMode(bool & b) {
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << b;

	enableEdit(b);
}

//--------------------------------------------------------------
void ofxInteractiveRect::Changed_Rect(ofRectangle & r) {
	ofLogNotice("ofxInteractiveRect") << " " << (__FUNCTION__) << r;

	this->set(r);

	doConstraints();
}