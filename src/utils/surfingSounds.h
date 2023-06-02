#pragma once
#include "ofMain.h"

/*

Simple sounds manager to add sounds to your app ui.

TODO:
add handle by name instead of index


*/

class SurfingSounds
{
private:
	vector<ofSoundPlayer> sounds;
	vector<ofSoundPlayer> soundsKeys;

public:
	SurfingSounds() {
		ofLogNotice("SurfingSounds - Constructor");

		ofSoundPlayer s0;//intro
		s0.load("assets/sounds/0.wav");
		sounds.push_back(s0);

		ofSoundPlayer s1;//send
		s1.load("assets/sounds/1.wav");
		sounds.push_back(s1);

		ofSoundPlayer s2;//receive
		s2.load("assets/sounds/2.wav");
		sounds.push_back(s2);

		ofSoundPlayer s3;//error
		s3.load("assets/sounds/3.wav");
		sounds.push_back(s3);

		ofSoundPlayer s4;//clear
		s4.load("assets/sounds/4.ogg");
		sounds.push_back(s4);

		ofSoundPlayer s5;//error
		s5.load("assets/sounds/5.mp3");
		sounds.push_back(s5);

		// keys
		string p = "assets/sounds/keys/";
		soundsKeys.clear();
		for (size_t i = 0; i < 9; i++)
		{
			string n = "keys-00" + ofToString(i) + ".wav";
			ofSoundPlayer sk;
			sk.load(p + n);
			sk.setVolume(1.f);
			sk.setMultiPlay(0);
			soundsKeys.push_back(sk);
		}

		for (auto& s : sounds)
		{
			s.setVolume(1.f);
			s.setMultiPlay(1);
		}
		sounds[4].setVolume(0.5);

		// intro
		sounds[0].play();
	}

	~SurfingSounds() {
		ofLogNotice("SurfingSounds - Destructor");
	}

	// play sound by index
	void play(int i) {
		if (i > sounds.size() - 1) {
			ofLogError("SurfingSounds") << "play. Out of range " << i;
			return;
		}

		ofLogNotice("SurfingSounds") << "play " << i;
		sounds[i].play();
	}

	// trig random sound to imitate a mechanical kyboard clicking.
	void playKeys() {
		int i = ofRandom(soundsKeys.size());
		ofLogNotice("SurfingSounds") << "Sound keys: " << i;
		soundsKeys[i].play();
	}
};