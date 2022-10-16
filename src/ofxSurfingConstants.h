#pragma once

//---------
//
// OPTIONAL // -> defines (modes) are here "to share between addons" in one place
//
//#define USE_IM_GUI
#define USE_JSON		//uncomment to use default xml instead json for ofParameterGroup de/serializers
//#define USE_ofxGui
//
//---------

// COMMONTLY USED FONTS

#define FONT_FILES_PATH "assets/fonts/"

#define FONT_FILE_BIG "JetBrainsMonoNL-ExtraBold.ttf"
#define FONT_FILE_SMALL "JetBrainsMono-Bold.ttf"
#define FONT_FILE_ALT1 "telegrama_render.otf"
#define FONT_FILE_ALT2  "JetBrainsMono-Bold.ttf"

#define FONT_SIZE_BIG 16
#define FONT_SIZE_SMALL 10

// EXAMPLE TO CHECK IF FONT FILES ARE LOCATED OR NOT TO AVOID CRASHES
/*
	fPath = "fonts/GTAmerica-ExtendedBlack.ttf";
	//fPath = "fonts/GTAmerica-ExtendedBold.ttf";

	if (!ofFile::doesFileExist(fPath.get())) {
		fPath = FONT_FILES_PATH + ofToString(FONT_FILE_BIG);
		if (!ofFile::doesFileExist(fPath.get())) {
			fPath = FONT_FILES_PATH + ofToString(FONT_FILE_SMALL);
			if (!ofFile::doesFileExist(fPath.get())) {
				fPath = FONT_FILES_PATH + ofToString(FONT_FILE_ALT1);
				if (!ofFile::doesFileExist(fPath.get())) {
					fPath = FONT_FILES_PATH + ofToString(FONT_FILE_ALT2);
					if (!ofFile::doesFileExist(fPath.get())) {
						fPath = OF_TTF_SANS;
					}
				}
			}
		}
	}
*/