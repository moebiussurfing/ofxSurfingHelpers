#pragma once

#include "ofMain.h"

///----
///
#define INCLUDE_RECORDER
///
///----

//TODO: BUG: when enabled antialias 16 or RGBF32 recording goes grey...?
#ifdef INCLUDE_RECORDER
//macOS screen recorder
#ifdef TARGET_OSX
#define USE_MAC_RECORDER
#ifdef USE_MAC_RECORDER
#include "ofxMacScreenRecorder.h"
#endif
#endif
//windows ffmpeg screen recorder
#ifdef TARGET_WIN32
#define USE_FFMPEG_RECORDER//TODO: disable
#ifdef USE_FFMPEG_RECORDER
#include "ofxFFmpegRecorder.h"
#include "ofxFastFboReader.h"
#endif
#endif
#endif

class CaptureWindow
{

public:
	CaptureWindow() {};
	~CaptureWindow() {};

public:
	//macOS screen recorder
#ifdef USE_MAC_RECORDER
	ofxMacScreenRecorder recorder;
	ofxMacScreenRecorderSetting recorderSetting;
#endif
	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	ofxFFmpegRecorder cap_Recorder;
	ofFbo cap_Fbo;
	ofPixels cap_Pix;
	ofxFastFboReader cap_Reader;
	ofFbo::Settings cap_Fbo_Settings;
	bool bRecPrepared = false;
#endif

	

public:
	//--------------------------------------------------------------
	void setup() {
		

	//----

	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	float w, h;
	w = ofGetWidth();
	h = ofGetHeight();

	cap_Fbo_Settings.internalformat = GL_RGB;
	//cap_Fbo_Settings.internalformat = GL_RGBA;
	//cap_Fbo_Settings.internalformat = GL_RGB32F;
	cap_Fbo_Settings.width = w;
	cap_Fbo_Settings.height = h;
	cap_Fbo_Settings.useDepth = true;//3d depth
	cap_Fbo_Settings.useStencil = true;//?
	cap_Fbo_Settings.numSamples = 16;//this breakes the recording...grey screen
	//cap_Fbo_Settings.depthStencilAsTexture = true;//?
	cap_Fbo.allocate(cap_Fbo_Settings);

	/*
	//-
	//
	//instagram settings
	//
	//https://www.bing.com/search?q=best+ffmpeg+settings+for+instagram+video&cvid=6165323b777d42699a0de21423b93032&FORM=ANNTA1&PC=U531
	//https://www.oberlo.com/blog/best-instagram-video-format
	//https://blog.snappa.com/instagram-video-format/
	//https://www.youtube.com/watch?v=ZjA2aYyC_0I
	//
	//The best Instagram video dimensions: 864 pixels(width) by 1080 pixels(height)
	//aspect ratio of 4:5.
	//H.264 codec
	//AAC audio
	//3500 kbps bitrate
	//Frame rate of 30 fps(frames per second)
	//Video can be a maximum of 60 seconds
	//Maximum video width is 1080 px(pixels) wide
	//Videos should be 920 pixels tall
	//
	Output #0, avi, to 'F:\openFrameworks\addons\ofxSphereCam2\ofxSphereCam2-example\bin\data\captures/2020-08-12-01-31-00-756.avi':
	 Metadata:
	ISFT            : Lavf57.67.100
	Stream #0:0: Video: mpeg4 (FMP4 / 0x34504D46), yuv420p, 864x1061, q=2-31, 28000 kb/s, 30 fps, 30 tbn, 30 tbc
	Metadata:
	  encoder         : Lavc57.86.103 mpeg4
	Side data:
	  cpb: bitrate max/min/avg: 0/0/28000000 buffer size: 0 vbv_delay: -1
	frame=  676 fps= 30 q=2.0 Lsize=     870kB time=00:00:22.53 bitrate= 316.4kbits/s speed=   1x
	video:848kB audio:0kB subtitle:0kB other streams:0kB global headers:0kB muxing overhead: 2.600246%
	//-
	*/

	//cap_Recorder.setup(true, false, glm::vec2(1920, 1080));

	//resolution
	//cap_Recorder.setup(true, false, glm::vec2(w, h));
	//cap_Recorder.setup(true, false, glm::vec2(1920, 1080), 30);

	//quality
	//cap_Recorder.setBitRate(10000);

	//presets

	////instagram
	//cap_Recorder.setBitRate(28000);//instagram 3500*8 = 28000
	//cap_Recorder.setup(true, false, glm::vec2(w, h), 30);

	//hq
	cap_Recorder.setBitRate(100000);
	//cap_Recorder.setBitRate(80000);
	cap_Recorder.setup(true, false, glm::vec2(w, h), 60);

	//options
	cap_Recorder.setOverWrite(true);
	cap_Recorder.setFFmpegPathToAddonsPath();
	//cap_Recorder.addAdditionalOutputArgument("-f rawvideo");
	//cap_Recorder.setVideoCodec("libx264");
	//cap_Reader.setAsync(true);
#endif
	//macOS screen recorder
#ifdef USE_MAC_RECORDER
	recorderSetting.codecType = ofxMacScreenRecorder::CodecType::ProRes4444;
	if (!recorder.setup(recorderSetting))
		ofExit(-1);
	recorder.registerFinishWritingCallback([this](const std::string &path)
		{
			ofLogNotice() << "success recording. save to: " << path;
		});
	recorder.registerStartWritingCallback([this]
		{
});
#endif

	//-------
	}


	//--------------------------------------------------------------
	void begin() {
	//--

	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	if (bRecPrepared) {
		cap_Fbo.begin();
		ofBackground(0);
		ofSetColor(255);
		//ofClear(0, 255);
	}
#endif
}



	//--------------------------------------------------------------
	void end() {
	//--
	//--

	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	if (bRecPrepared) {
		ofClearAlpha();
		cap_Fbo.end();

		if (cap_Recorder.isRecording())
		{
			////A. faster
			////ofxFastFboReader can be used to speed this up:
			//cap_Reader.readToPixels(cap_Fbo, cap_Pix, OF_IMAGE_COLOR);
			//if (cap_Pix.getWidth() > 0 && cap_Pix.getHeight() > 0) {
			//	cap_Recorder.addFrame(cap_Pix);
			//}

			//B. standard
			cap_Fbo.readToPixels(cap_Pix);
			if (cap_Pix.getWidth() > 0 && cap_Pix.getHeight() > 0) {
				cap_Recorder.addFrame(cap_Pix);
			}
		}

		//-

		cap_Fbo.draw(0, 0);//drawing is required outside fbo
	}
#endif

}

	//--------------------------------------------------------------
	void draw() {
	//--

	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	int y = ofGetHeight() - 150;
	int x = 20;
	//draw red circle and info when recording
	if (cap_Recorder.isRecording())
	{
		ofPushStyle();
		ofSetColor(ofColor::red);
		ofDrawCircle(ofPoint(x + 8, y), 8); y += 28;
		ofDrawBitmapStringHighlight("DURATION: " + ofToString(cap_Recorder.getRecordedDuration(), 1), x, y); y += 20;
		ofDrawBitmapStringHighlight("FPS: " + ofToString(ofGetFrameRate(), 0), x, y); y += 20;
		ofPopStyle();
	}
	else if (bRecPrepared)
	{
		y += 28;
		ofDrawBitmapStringHighlight("RECORD MOUNTED... PRESS 'U' TO START", x, y); y += 20;
		ofDrawBitmapStringHighlight(ofToString(cap_Fbo_Settings.width) + "x" + ofToString(cap_Fbo_Settings.height), x, y);
	}
#endif
}

//--------------------------------------------------------------
void keyPressed(ofKeyEventArgs &eventArgs) {
	const int key = eventArgs.key;
	//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	//---

	//screen recorder

	//-

	switch (key)
	{
		//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
	//case OF_KEY_F11://screenshot pict
	//cap_Recorder.saveThumbnail(0, 0, 2, ("data/captures/cap" + ofGetTimestampString() + ".png"), ofVec2f(0, 0), ofRectangle(0, 0, 500, 400));
	//break;

//	//set instagram size
//	case 'i':
//	{
//		int w, h;
//		w = 864;
//		h = 1080 + 19;
//		ofSetWindowShape(w, h);
//		windowResized(w, h);
//
//		//--
//
////windows ffmpeg screen recorder
//#ifdef USE_FFMPEG_RECORDER	
//		cap_Fbo_Settings.width = ofGetWidth();
//		cap_Fbo_Settings.height = ofGetHeight();
//		cap_Fbo.allocate(cap_Fbo_Settings);
//
//		////instagram
//		//cap_Recorder.setBitRate(28000);//instagram 3500*8 = 28000
//		//cap_Recorder.setup(true, false, glm::vec2(w, h), 30);
//		//hq
//		cap_Recorder.setBitRate(80000);
//		cap_Recorder.setup(true, false, glm::vec2(w, h), 60);
//#endif
//	}
//	break;

	//prepare video record
	case 'u':
		bRecPrepared = !bRecPrepared;
		break;

		//start video record
	case 'U':
	{
		if (cap_Recorder.isRecording())//stop
		{
			cap_Recorder.stop();
			ofLogWarning(__FUNCTION__) << "Stop Recording";

			bRecPrepared = false;
		}
		else//start
		{
			//string path = "data/";
			//ofxSurfingHelpers::CheckFolder(path);
			//std::string fileCap = "data/capture" + ofGetTimestampString() + ".avi";
			//std::string fileCap = path + "capture_01.avi";
			//cap_Recorder.setOutputPath(fileCap);

			//bin/data
			string _path = ofToDataPath("captures/" + ofGetTimestampString() + ".avi", true);
			cap_Recorder.setOutputPath(_path);

			//start record
			cap_Recorder.startCustomRecord();
			ofLogWarning(__FUNCTION__) << "Start Recording: " << _path;
		}
	}
	break;

	case 'I'://gets a frame from the located video. not a live screenshot!
	{
		string _path = ofToDataPath("captures/Screenshot_" + ofToString(ofGetTimestampString()) + ".png");
		cap_Recorder.saveThumbnail(0, 0, 2, _path, ofVec2f(0, 0), ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
		ofLogWarning(__FUNCTION__) << "Snapshot: " << _path;
	}
	break;
#endif
	//macOS screen recorder
#ifdef USE_MAC_RECORDER
	case OF_KEY_F11:
		ofLogNotice() << ofGetWindowPositionX() << ", " << ofGetWindowPositionY();
		recorder.setSetting(recorderSetting);
		recorder.start(ofToDataPath("./test"));
		break;
	case OF_KEY_F12:
		recorder.stop();
		break;
#endif
	}
}

//--------------------------------------------------------------
void windowResized(int w, int h) {
	
	//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER	
	cap_Fbo_Settings.width = w;
	cap_Fbo_Settings.height = h;
	cap_Fbo.allocate(cap_Fbo_Settings);
#endif
}
	//---
};