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

#include "ofxSurfingHelpers.h"

class CaptureWindow
{

public:
	CaptureWindow() {
		cap_w = 1920;
		cap_h = 1080;
		cap_Bitrate = 10000;
		cap_Framerate = 30;
	};
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
	int cap_w, cap_h;
	int cap_Bitrate;
	int cap_Framerate;
#endif
	string _pathFolder;

	//TEST: BUG: antialias
	//ofTexture cap_Tex;
	ofFbo blitFbo;

public:
	//--------------------------------------------------------------
	void init() {///must be called after bitrate, framerate and size w/h are setted

		//setup and resolution
		cap_Recorder.setup(true, false, glm::vec2(cap_w, cap_h), cap_Framerate);

		//quality
		cap_Recorder.setBitRate(cap_Bitrate);

		//-

		//extra options
		cap_Recorder.setOverWrite(true);
		cap_Recorder.setFFmpegPathToAddonsPath();
		//cap_Recorder.addAdditionalOutputArgument("-f rawvideo");
		//cap_Recorder.setVideoCodec("libx264");
		//cap_Reader.setAsync(true);
	}

	//--------------------------------------------------------------
	void setup(std::string path = "captures/") {///call with the path folder if you want to customize
		_pathFolder = path;
		ofxSurfingHelpers::CheckFolder(_pathFolder);

		//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
		cap_w = ofGetWidth();
		cap_h = ofGetHeight();

		//TEST: BUG: antialias
		//cap_Pix.allocate(cap_w, cap_h, GL_RGB);
		//cap_Pix.clear();

		//fbo
		{
			cap_Fbo_Settings.internalformat = GL_RGB;
			//cap_Fbo_Settings.internalformat = GL_RGB32F;

			cap_Fbo_Settings.width = cap_w;
			cap_Fbo_Settings.height = cap_h;
			cap_Fbo_Settings.useDepth = true;
			cap_Fbo_Settings.numSamples = 16;//BUG: on ofxFastFboReader requires aux blitFbo..
			//cap_Fbo_Settings.useStencil = true;
			//cap_Fbo_Settings.depthStencilAsTexture = true;
			//cap_Fbo_Settings.maxFilter

			cap_Fbo.allocate(cap_Fbo_Settings);
		}

		//TEST: BUG: antialias
		blitFbo.allocate(cap_Fbo.getWidth(), cap_Fbo.getHeight(), GL_RGB);
		//blitFbo.allocate(cap_Fbo.getWidth(), cap_Fbo.getHeight(), GL_RGB32F);

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

		//-

		//presets

		////instagram
		//cap_Bitrate = 28000;
		//cap_Framerate = 30;

		//hq
		cap_Bitrate = 80000;
		cap_Framerate = 60;

		//-

		init();
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
	}

	//--------------------------------------------------------------
	void begin() {///call before draw the scene to record

		//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
		if (bRecPrepared) {
			cap_Fbo.begin();
			//ofBackground(0);
			//ofSetColor(255);
			//ofClear(0, 255);
		}
#endif
	}

	//--------------------------------------------------------------
	void end() {///call after draw the scene to record

		//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
		if (bRecPrepared) {
			//ofClearAlpha();
			cap_Fbo.end();

			if (cap_Recorder.isRecording())
			{
				////A. faster
				////ofxFastFboReader can be used to speed this up:
				//cap_Reader.readToPixels(cap_Fbo, cap_Pix, OF_IMAGE_COLOR);
				//if (cap_Pix.getWidth() > 0 && cap_Pix.getHeight() > 0) {
				//	cap_Recorder.addFrame(cap_Pix);
				//}

				////B. standard
				//cap_Fbo.readToPixels(cap_Pix);
				//if (cap_Pix.getWidth() > 0 && cap_Pix.getHeight() > 0) {
				//	cap_Recorder.addFrame(cap_Pix);
				//}

				//-

				//TEST: BUG: antialias
				//cap_Tex.allocate(cap_Pix);

				//TEST: BUG: antialias
				//C. blitting test
				blitFbo.begin();
				ofClear(0);
				cap_Fbo.draw(0, 0, cap_w, cap_h);
				blitFbo.end();

				cap_Reader.readToPixels(blitFbo, cap_Pix, OF_IMAGE_COLOR);
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
	void draw() {///draw the gui info if desired

		//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER

		if (bRecPrepared || cap_Recorder.isRecording()) {
			int y = ofGetHeight() - 200;
			int x = 20;

			//cap info
			string str;
			str = "SIZE " + ofToString(cap_w) + "x" + ofToString(cap_h);
			str += " | BITRATE " + ofToString(cap_Bitrate);
			str += " | FRAMERATE " + ofToString(cap_Framerate);

			//draw red circle and info when recording
			ofPushStyle();

			//red rec circle
			if (cap_Recorder.isRecording())
			{
				ofFill();
				ofSetColor(ofColor::red);
				ofDrawCircle(ofPoint(x + 8, y), 8);
				ofNoFill();
				ofSetLineWidth(2.f);
				ofSetColor(ofColor::black);
				ofDrawCircle(ofPoint(x + 8, y), 8);
				y += 28;
			}
			else if (bRecPrepared)
			{
				if (ofGetFrameNum() % 60 < 20) {
					ofFill();
					ofSetColor(ofColor::red);
					ofDrawCircle(ofPoint(x + 8, y), 8);
				}
				ofNoFill();
				ofSetLineWidth(2.f);
				ofSetColor(ofColor::black);
				ofDrawCircle(ofPoint(x + 8, y), 8);
				y += 28;
			}

			//fps
			ofDrawBitmapStringHighlight("FPS " + ofToString(ofGetFrameRate(), 0), x, y);
			y += 20;

			//cap info
			ofDrawBitmapStringHighlight(str, x, y);
			y += 20;

			if (cap_Recorder.isRecording())
			{
				ofDrawBitmapStringHighlight("RECORD DURATION: " + ofToString(cap_Recorder.getRecordedDuration(), 1), x, y);
				y += 20;
				ofDrawBitmapStringHighlight("KEY U: STOP", x, y);
				y += 20;
			}
			else if (bRecPrepared)
			{
				ofDrawBitmapStringHighlight("RECORD MOUNTED. READY...", x, y);
				y += 20;
				ofDrawBitmapStringHighlight("KEY U: START", x, y);
				y += 20;
			}

			ofPopStyle();

			////TEST: BUG: antialias
			//if (cap_Recorder.isRecording())
			//{
			//	cap_Tex.draw(100, 100, 800, 600);
			//}
		}
#endif
	}

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs &eventArgs) {///to received short keys control commands
		const int key = eventArgs.key;
		//bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);

		//-

		switch (key)
		{
			//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER
		//case OF_KEY_F11://screenshot pict
		//cap_Recorder.saveThumbnail(0, 0, 2, ("data/captures/cap" + ofGetTimestampString() + ".png"), ofVec2f(0, 0), ofRectangle(0, 0, 500, 400));
		//break;

		//set instagram size
		case 'i':
		{
			int w, h;
			w = 864;
			h = 1080 + 19;
			ofSetWindowShape(w, h);
			windowResized(w, h);

			cap_w = w;
			cap_h = h;

			//--

			//windows ffmpeg screen recorder
#ifdef USE_FFMPEG_RECORDER	
			cap_Fbo_Settings.width = cap_w;
			cap_Fbo_Settings.height = cap_h;
			cap_Fbo.allocate(cap_Fbo_Settings);
			blitFbo.allocate(cap_Fbo.getWidth(), cap_Fbo.getHeight());

			//-

			//presets

			//instagram
			cap_Bitrate = 28000;
			cap_Framerate = 30;

			////hq
			//cap_Bitrate = 80000;
			//cap_Framerate = 60;

			//-

			init();
#endif
		}
		break;

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
				string _path = ofToDataPath(_pathFolder + ofGetTimestampString() + ".avi", true);
				cap_Recorder.setOutputPath(_path);

				//start record
				cap_Recorder.startCustomRecord();
				ofLogWarning(__FUNCTION__) << "Start Recording: " << _path;
			}
		}
		break;

		case 'I'://gets a frame from the located video. not a live screenshot!
		{
			string _path = ofToDataPath(_pathFolder + "Screenshot_" + ofToString(ofGetTimestampString()) + ".png");
			cap_Recorder.saveThumbnail(0, 0, 2, _path, ofVec2f(0, 0), ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
			ofLogWarning(__FUNCTION__) << "Snapshot: " << _path;
		}
		break;
#endif
		//-

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

	//windows ffmpeg screen recorder
	//--------------------------------------------------------------
	void windowResized(int w, int h) {///must be called to resize the fbo and video resolution
#ifdef USE_FFMPEG_RECORDER
		cap_w = w;
		cap_h = h;

		cap_Fbo_Settings.width = cap_w;
		cap_Fbo_Settings.height = cap_h;
		cap_Fbo.allocate(cap_Fbo_Settings);
		blitFbo.allocate(cap_Fbo.getWidth(), cap_Fbo.getHeight());

		//-

		//presets

		////instagram
		//cap_Bitrate = 28000;
		//cap_Framerate = 30;

		////hq
		//cap_Bitrate = 80000;
		//cap_Framerate = 60;

		//-

		init();
#endif
	}
};