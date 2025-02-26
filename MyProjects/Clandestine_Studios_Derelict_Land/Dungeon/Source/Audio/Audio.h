/******************************************************************************/
/*!
\file		Audio.h
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 1, 2022
\brief		Audio System


Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once

#include "Essential.h"
#include "System/System.h"
#include "GameObject/GameObject.h"
#include "fmod.hpp"
#include "Input/Input.h"
#include "Input/Keycodes.h"
#include "CoreSys/MasterObjects.h"

/*
	SOME STUFF FROM ASC ENGINE TALK ON AUDIO, MAYBE USEFUL?
	- How far do we want to go with audio?
		- few features or customizability?
	- L - debugging
	- audio system wrapper class -> provides abstraction
	- sound wrapper class maybe? arnd fmod::sound
	  - makes for SRP principle
	
	some features to think abt
	- per sfx vol ctrl
	- group vol ctrl (master, bgm, sfxx channel grps)
	- cross fading (increase one sfx vol as another decreases)
		- e.g., main menu sfx fades out as main game sfx fades in
	- pitch ctrl? makes sfx not sound the same
		- e.g. walking sfx - makes sfx not sound the same always
	- seeking (play sfx from a certain point)
	
	some stuff to think about (and probably nvr do)
	- ducking (reduce volume of bgm when another event ongoing)
	- dsp effects (reverb, echo, etc)
	
	audacity for audio editing
	avid, muse - music making tools
*/

/*
	Additional notes from Louis M.

	BGM and SFX are a set as a mutliplier of the MasterVol

	so if Master is at 0.6f, and BGM is at 0.4, resultant level is
	0.6*0.4 = 0.24f


	Currently, if a sfx channel is playing at a vol that is not as what is calculated,
	its vol is set again in playsfx.
*/



class Audio : public System
{
public:
	/*!*************************************************************************
	****
	\brief
		Default constructor.
	****************************************************************************
	***/
	Audio();
	/*!*************************************************************************
	****
	\brief
		Non default Constructor for the audio system. 
	****************************************************************************
	***/
	Audio(float mVol, float bgmVol, float sfxVol);
	
	Audio(const Audio&) = delete;
	Audio& operator=(const Audio&) = delete;
	Audio(Audio&&) = delete;
	Audio& operator=(Audio&&) = delete;
	
	/*!*************************************************************************
	****
	\brief
		Initialize the Audio system
	****************************************************************************
	***/
	virtual void Init();
	/*!*************************************************************************
	****
	\brief
		Shut down the Audio system
	****************************************************************************
	***/
	virtual void Shutdown();
	/*!*************************************************************************
	****
	\brief
		Updates Audio System
	\param[in] dt
		delta time
	****************************************************************************
	***/
	virtual void Update(float dt);

	/*!*************************************************************************
	****
	\brief
		Loads an audio file from filepath
	\param[in] str
		Filepath to audio file
	\param[in] loop
		Should this file loop
	****************************************************************************
	***/
	void LoadAudio(char const* str, bool loop);

	/*!*************************************************************************
	****
	\brief
		Loads audio files from a file containing filepaths to audio files
	\param[in] Filename
		File to load files from
	****************************************************************************
	***/
	void LoadAudioFiles(std::string Filename);

	/*!*************************************************************************
	****
	\brief
		Play audio as a SFX (in the SFX Channel)
	\param[in] index
		Index to play
	****************************************************************************
	***/
	void PlaySfx(int index);

	/*!*************************************************************************
	****
	\brief
		Play audio as BGM (in the BGM Channel)
	\param[in] index
		Index to play
	****************************************************************************
	***/
	void PlayMusic(int index);

	/*!*************************************************************************
	****
	\brief
		Stops the audio playing in SFX channel
	\param index
		Index of channel to stop (0 <= index < MaxSfxChannels).
		Values outside the range stop all channels.
	****************************************************************************
	***/
	void StopSfx(int index = -1);

	/*!*************************************************************************
	****
	\brief
		Stops the audio playing in BGM channel
	****************************************************************************
	***/
	void StopMusic();

	/*!*************************************************************************
	****
	\brief
		Gets Master volume
	\return
		Master volume (0.f - 1.f)
	****************************************************************************
	***/
	float& GetMasterVol();

	/*!*************************************************************************
	****
	\brief
		Gets SFX volume member variable value
	\return
		SFX volume (0.f - 1.f), -1.f if out of range
	****************************************************************************
	***/
	float GetSfxVol();

	/*!*************************************************************************
	****
	\brief
		Gets SFX Channel volume
	\param index
		Index of channel to get volume from.
	\return
		SFX volume (0.f - 1.f),(master*sfxvol)
	****************************************************************************
	***/
	float GetSfxChannelResultantVol(int index);

	/*!*************************************************************************
	****
	\brief
		Gets BGM Channel volume
	\return
		BGM volume (0.f - 1.f)
	****************************************************************************
	***/
	float GetMusicVol();

	/*!*************************************************************************
	****
	\brief
		Sets Master volume
	\param[in] volume
		Volume to set (0.f to 1.f)
	****************************************************************************
	***/
	void SetMasterVol(float volume);

	/*!*************************************************************************
	****
	\brief
		Sets SFX volume variable, and calls SetSfxChannelVol(), to set each SFX
		channel.
	\param[in] volume
		Volume to set (0.f to 1.f), this is before master * sfxvol
	****************************************************************************
	***/
	void SetSfxVol(float volume);

	/*!*************************************************************************
	****
	\brief
		Sets SFX Channel volume, only INDIVIDUAL CHANNEL;
	\param[in] resultant_vol
		Volume to set (0.f to 1.f), this is after master * sfxvol
	\param[in] index
		Index of channel to set. Fails if out of range
	****************************************************************************
	***/
	void SetSfxChannelVol(float resultant_vol, int index);

	/*!*************************************************************************
	****
	\brief
		Sets BGM Channel volume
	\param[in] volume
		Volume to set (0.f to 1.f)
	****************************************************************************
	***/
	void SetMusicVol(float volume);

	/*!*************************************************************************
	****
	\brief
		Toggles BGM Channel mute
	****************************************************************************
	***/
	void ToggleMuteMusic();

	/*!*************************************************************************
	****
	\brief
		Toggles SFX Channel mute
	****************************************************************************
	***/
	void ToggleMuteSound();

	/*!*************************************************************************
	****
	\brief
		Toggles BGM Channel pause
	****************************************************************************
	***/
	void ToggleMusicPause();

	/*!*************************************************************************
	****
	\brief
		Returns the value of the maximum sfx channels set
	\return
		int count of the existing sfx channels 
	****************************************************************************
	***/
	int GetSfxMaxChannels();

	static Audio* GetInstance();
private:
	FMOD::System* AudioSystem;
	//std::vector<FMOD::Sound*> AudioArray;
	FMOD::Channel* BgmChannel;
	std::vector<FMOD::Channel*> SfxChannel;
	FMOD::ChannelGroup* Master;
	
	FMOD::SoundGroup* BgmGroup; // not used
	FMOD::ChannelControl* BgmControl; //// not used
	
	
	float MasterVol;
	float BgmVol;
	float SfxVol;
	int MaxSfxChannels;
};

static Audio* audio = Audio::GetInstance();