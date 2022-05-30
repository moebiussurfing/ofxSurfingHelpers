#pragma once
#include "ofRectangle.h"
#include "ofCircleInfo.h"
#include "ofVec4f.h"

//--------------------------------------------------------------
// ofxMyUtil Event
//--------------------------------------------------------------
namespace ofxMyUtil 
{
	namespace Event 
	{
		bool RectButton(ofRectangle rect, int mouseX, int mouseY);
		bool RectButton(int rect_x, int rect_y, int rect_w, int rect_h, int mouseX, int mouseY);		
		bool CircleButton(ofCircleInfo circle, int mouseX, int mouseY);
		bool CircleButton(int circle_x, int circle_y, float radius, int mouseX, int mouseY);
		bool EllipseButton(ofVec4f ellipse, int mouseX, int mouseY);
		bool EllipseButton(int ellipse_x, int ellipse_y, int ellipse_w, int ellipse_h, int mouseX, int mouseY);
	}
}