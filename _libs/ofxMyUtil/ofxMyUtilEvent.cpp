#include "ofxMyUtilEvent.h"
#include "ofMath.h"
#include <algorithm>

//--------------------------------------------------------------
// Event
//--------------------------------------------------------------
using namespace std;
using namespace ofxMyUtil;

//--------------------------------------------------------------
bool Event::RectButton(ofRectangle rect, int mouseX, int mouseY) 
{
	if (rect.x <= mouseX &&
		mouseX <= rect.x + rect.width &&
		rect.y <= mouseY &&
		mouseY <= rect.y + rect.height)
	{
		return 1;
	}
	else {
		return 0;
	}
}

//--------------------------------------------------------------
bool Event::RectButton(int rect_x, int rect_y, int rect_w, int rect_h, int mouseX, int mouseY) 
{
	return RectButton(ofRectangle(rect_x, rect_y, rect_w, rect_h), mouseX, mouseY);
}

//--------------------------------------------------------------
bool Event::CircleButton(ofCircleInfo circle, int mouseX, int mouseY) 
{
	if (ofDist(circle.GetX(), circle.GetY(), mouseX, mouseY) < circle.GetRadius()) return 1;
	else return 0;
}

//--------------------------------------------------------------
bool Event::CircleButton(int circle_x, int circle_y, float radius, int mouseX, int mouseY) 
{
	return CircleButton(ofCircleInfo(circle_x, circle_y, radius), mouseX, mouseY);
}

//--------------------------------------------------------------
bool Event::EllipseButton(ofVec4f ellipse, int mouseX, int mouseY) 
{
	return RectButton(ofRectangle(glm::vec2(ellipse.x - ellipse.z / 2, ellipse.y - ellipse.w / 2), ellipse.z, ellipse.w), mouseX, mouseY);
}

//--------------------------------------------------------------
bool Event::EllipseButton(int ellipse_x, int ellipse_y, int ellipse_w, int ellipse_h, int mouseX, int mouseY) 
{
	return EllipseButton(ofVec4f(ellipse_x, ellipse_y, ellipse_w, ellipse_h), mouseX, mouseY);
}

