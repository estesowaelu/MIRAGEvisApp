//    SkyPaint:  Virtual Stargazing With a Touch of Magic
//    Copyright (C) 2006-2008  Ken Kopecky (allegrocm@gmail.com)
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.



/*
 *  SoundManager.h
 *  Boat
 *
 *  written by Ken
  */

#ifndef KENSoundManager_h
#define KENSoundManager_h

#include <string>

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
#else
 //   #include "fmod/wincompat.h"
#endif

#include "fmod/fmod.hpp"

#include "fmod/fmod_dsp.h"
#include <vector>
#include <stdio.h>
//uses fmodEX
typedef FMOD::Channel* SkySound;


class PlayListEntry
{
	public:
	PlayListEntry(std::string name, float time)	{filename = name, trackTime = time;}
	std::string filename;
	float trackTime;

};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	static SoundManager* instance(){static SoundManager man; return &man;}
	/* 
	 * playSong()
	 *
	 * returns true if successful, false if not
	 *
	 * name: the name of the sound file
	 * volume: between 0 and 1 (0 = mute, 1 = full volume)
	 * stereo: between -1 and 1 (-1 = left, 0 = balanced, 1 = right)
	 */
	bool playSong(std::string name, float volume = 1, float stereo = 0, bool loop = true);
	void setSongPitch(float p);
	/** Pauses/unpauses current song.  Does nothing if no song is currently playing. */
	void pauseSong();
	void setSongPosition(float px, float py, float pz, float vx = 0, float vy = 0, float vz = 0);
	
	/** Stops the current song and closes the stream.  To replay that song
	 *  you have to call playSong again.  Does nothing if no song is currently
	 *  playing.
	 */
	void stopSong();
	
	/** Plays a sound file.  Returns false if there was an error. */
	bool playSound(std::string name, float volume = 1.0, float stereo = 0.0, bool echo = true, bool loop = false);
	SkySound play3DSound(std::string name, float volume, float x, float y, float z, bool echo = true, bool loop = false, float fallOffStart = 2);
	void updateListener(float dt, float transform[16], float velx, float vely, float velz);	//send in transform and positional info for the listener
	/*
	 * preLoad()
	 *
	 * use this to try to make sounds read from memory instead of hard drive
	 */
	bool preLoad(std::string name);
	void update(float dt);
	void fadeIntoSong(float time, const std::string& nextSong);
	void setMusicVolume(float v);
	void setMusicEcho(float on);				//makes our music echo...or not echo.  in this case music is an engine noise
	float getEchoAmount()		{return mEchoAmount;}
	void setSound3DInfo(SkySound soundChannel, float px, float py, float pz, float vx, float vy, float vz);
	void setSoundVolume(SkySound soundChannel, float volume);
	void setSoundFrequency(SkySound soundChannel, float frequency);
	void letSoundDie(SkySound soundchannel);	//removes all priority from this sound so it can be canceled with no trouble
	void stopSound(SkySound sound);
	void loadPlaylist(std::string name);
	void skipSong();			//skip playlist songs
	void previousSong();		//rewind the playlist
	std::string getSongName()	{return mCurrentSongName;}
	void setSilent(bool s)		{mSilent = s;}
private:
	FMOD::System* mSystem;		//fmod object
	bool mMusicEcho;
	float songBaseFrequency;
	FMOD::Sound* song;
	FMOD::DSP*	mEcho;
	FMOD::DSP* mReverb;
	FMOD::Channel* songChannel;
	std::string mCurrentSongName;
	float musicFadeTime, musicFadeTimeCounter;
	bool fading;
	std::string nextSong;
	void checkErrors();					//print out any pending errors we have
	static const int FULL_VOLUME = 255;
	static const int FULL_RIGHT_CHANNEL = 255;
	float mEchoAmount;					//magnitue of our general echo
	FMOD_RESULT result;					//generic fmod call result
	std::vector<PlayListEntry> mPlayList;
	float mTrackTime;
	int mTrack;
	bool mUsePlayList;
	float mMusicVolume;
	bool mInitFailed;
	bool mSilent;
};

#endif

