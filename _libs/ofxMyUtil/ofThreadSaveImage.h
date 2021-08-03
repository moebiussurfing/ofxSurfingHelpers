#pragma once
#include "ofThread.h"
#include "ofPixels.h"
#include "ofImage.h"
#include <queue>


namespace ofxMyUtil {

	using ofSaveImageQueue = struct 
	{
		std::string savePath = "/";
		ofPixels pix = ofPixels();
		ofImageQualityType qType = ofImageQualityType::OF_IMAGE_QUALITY_BEST;
	};


	class ofThreadSaveImage : public ofThread
	{
	private:
		std::queue< ofSaveImageQueue> q;
	public:
		ofThreadSaveImage() {}
		~ofThreadSaveImage() { Stop(); }

		void Run() { startThread(); }
		void Stop() { waitForThread(); }

		void threadedFunction() {
			while (isThreadRunning()) {
				if (!q.empty()) {
					ofSaveImageQueue i = q.front();
					ofSaveImage(i.pix, i.savePath, i.qType);
					q.pop();
				}
			}
		}

		void AddQueue(const ofSaveImageQueue &queue) { q.push(queue); }
		void AddQueue(const ofPixels &pix, const std::string &savePath, const ofImageQualityType type) 
		{
			ofSaveImageQueue _q;
			_q.pix = pix;
			_q.savePath = savePath;
			_q.qType = type;
			AddQueue(_q);
		}
	};
}

