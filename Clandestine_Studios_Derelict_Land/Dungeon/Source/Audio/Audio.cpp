/******************************************************************************/
/*!
\file		Audio.cpp
\author 	Brandon Hsu
\par    	email: b.hsu\@digipen.edu , 620007421
\date   	March 1, 2022
\brief		Audio System


Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "Audio.h"
#include "Graphics/GLFWsetup.h"

FMOD_RESULT result;
FMOD::Sound* temp;
float voltemp;
bool paused;

/*!*************************************************************************
****
\brief
	Default constructor.
****************************************************************************
***/
Audio::Audio() : AudioSystem{ nullptr }, BgmChannel{ nullptr }, Master{ nullptr }, BgmGroup{ nullptr }, BgmControl{nullptr}
{
	MaxSfxChannels = 5;
	for (int i = 0; i < MaxSfxChannels; ++i) {
		SfxChannel.emplace_back(nullptr);
	}
	MasterVol = BgmVol = SfxVol = 1.0f;
}
/*!*************************************************************************
	****
	\brief
		Non default Constructor for the audio system.
****************************************************************************
***/
Audio::Audio(float mVol, float bgmVol, float sfxVol) : AudioSystem{ nullptr }, BgmChannel{ nullptr }, Master{ nullptr }
, BgmGroup{ nullptr }, BgmControl{ nullptr }
{
	MaxSfxChannels = 5;
	for (int i = 0; i < MaxSfxChannels; ++i) {
		SfxChannel.emplace_back(nullptr);
	}
	MasterVol = mVol; BgmVol = bgmVol; SfxVol = sfxVol;
}

/*!*************************************************************************
****
\brief
	Initialize the Audio system
****************************************************************************
***/
void Audio::Init()
{
	result = FMOD::System_Create(&AudioSystem);
	if (result != FMOD_OK) {
		std::cout << "WARNING: failed to initialize audio system! (create failed)\n";
		return;
	}
	result = AudioSystem->init(32, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		std::cout << "WARNING: failed to initialize audio system! (initialization failed)\n";
		return;
	}
	result = AudioSystem->createChannelGroup("Master", &Master);

	paused = false;

	audio->LoadAudioFiles("./Data/AudioData.txt");
	audio->SetMasterVol(MasterVol);
}

/*!*************************************************************************
****
\brief
	Shut down the Audio system
****************************************************************************
***/
void Audio::Shutdown()
{
	StopMusic();
	StopSfx();

	/*for (FMOD::Sound* audio : AudioArray)
	{
		audio->release();
	}
	AudioArray.clear();*/
	result = AudioSystem->close();
	result = AudioSystem->release();
}

/*!*************************************************************************
****
\brief
	Updates Audio System
\param[in] dt
	delta time
****************************************************************************
***/
void Audio::Update(float dt) {
	(void)dt;
	if (!GLSetup::IsFocus) { if (!paused) { ToggleMusicPause(); } paused = true; }
	else { if (paused) { ToggleMusicPause(); } paused = false; }
}

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
void Audio::LoadAudio(char const* str, bool loop) {
	result = AudioSystem->createSound(str, loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT, 0, &temp);
	if (result != FMOD_OK) {
		std::cout << "WARNING: failed to load audio file: " << str << " !\n";
		return;
	}
	else {
		//AudioArray.push_back(temp);
		//this->GetLevel()->GetAssets()->AddAudio(temp);
		MasterObjectList->MasterAssets.AddAudio(temp);
	}
}

/*!*************************************************************************
****
\brief
	Loads audio files from a file containing filepaths to audio files
\param[in] Filename
	File to load files from
****************************************************************************
***/
void Audio::LoadAudioFiles(std::string Filename) {
	std::ifstream ifs(Filename);
	if (!ifs) {
		std::cout << "Unable to open Audio data file!\n";
		return;
	}
	std::string Audiopath;
	while (std::getline(ifs, Audiopath)) {
		size_t pos = Audiopath.find_last_of('"');
		std::string Path = Audiopath.substr(1, pos - 1);
		bool looping = (Audiopath[Audiopath.size() - 1] == '1') ? true : false;
		LoadAudio(Path.c_str(), looping);
	}
}

/*!*************************************************************************
****
\brief
	Play audio as a SFX (in the SFX Channel)
\param[in] index
	Index to play
****************************************************************************
***/
void Audio::PlaySfx(int index) {
	//Play an audio file as a SFX
	//Find a free channel. If no free channel, don't play.
	for (int i = 0; i < MaxSfxChannels; ++i) {
		bool playing = true;
		FMOD_RESULT res = SfxChannel[i]->isPlaying(&playing);
		(void)res;
		if (!playing) {
			//result = AudioSystem->playSound(this->GetLevel()->GetAssets()->GetAudio(index), Master, false, &SfxChannel[i]);
			result = AudioSystem->playSound(MasterObjectList->MasterAssets.GetAudio(index), Master, false, &SfxChannel[i]);
			

			// volume readjustments
			float vol_aft;
			float supposed_vol = MasterVol * SfxVol;
			SfxChannel[i]->getVolume(&vol_aft);
			if (supposed_vol != vol_aft)
			{
				SfxChannel[i]->setVolume(supposed_vol);
			}
			std::cout << "Played SFX in channel " << i << std::endl;
			return;
		}
	}
	std::cout << "Could not play SFX with index " << index << ". No free SFX channel.\n";
}

/*!*************************************************************************
****
\brief
	Play audio as BGM (in the BGM Channel)
\param[in] index
	Index to play
****************************************************************************
***/
void Audio::PlayMusic(int index) {
	//Play an audio file as Music
	//result = AudioSystem->playSound(AudioArray[index], Master, false, &BgmChannel);
	// if bgm is already playing, stop it
	if (BgmChannel) { BgmChannel->stop(); }
	//result = AudioSystem->playSound(this->GetLevel()->GetAssets()->GetAudio(index), Master, false, &BgmChannel);
	result = AudioSystem->playSound(MasterObjectList->MasterAssets.GetAudio(index), Master, false, &BgmChannel);
}

/*!*************************************************************************
****
\brief
	Stops the audio playing in SFX channel
\param index
	Index of channel to stop (0 <= index < MaxSfxChannels).
	Values outside the range stop all channels.
****************************************************************************
***/
void Audio::StopSfx(int index) {
	if (index >= 0 && index < MaxSfxChannels) {
		SfxChannel[index]->stop();
	}
	else {
		for (int i = 0; i < MaxSfxChannels; ++i) {
			SfxChannel[i]->stop();
		}
	}
}

/*!*************************************************************************
****
\brief
	Stops the audio playing in BGM channel
****************************************************************************
***/
void Audio::StopMusic() {
	BgmChannel->stop();
}

/*!*************************************************************************
****
\brief
	Gets Master volume
\return
	Master volume (0.f - 1.f)
****************************************************************************
***/
float& Audio::GetMasterVol() {
	//float vol;
	//Master->getVolume(&vol);
	return MasterVol;
}

/*!*************************************************************************
****
\brief
	Gets SFX volume member variable value
\return
	SFX volume (0.f - 1.f), -1.f if out of range
****************************************************************************
***/
float Audio::GetSfxVol() {
	/*
	if (index >= 0 && index < MaxSfxChannels) {
		SfxChannel[index]->getVolume(&voltemp);
		return voltemp;
	}
	else return -1.f;
	*/
	return SfxVol;
}

/*!*************************************************************************
****
\brief
	Gets SFX Channel resultant volume,(master vol * sfx vol)
\param index
	Index of channel to get volume from.
\return
	SFX volume (0.f - 1.f), -1.f if out of range
****************************************************************************
***/
float Audio::GetSfxChannelResultantVol(int index)
{
	if (index >= 0 && index < MaxSfxChannels) {
		SfxChannel[index]->getVolume(&voltemp);
		return voltemp;

	}
	else
	{
		std::cout << "Unable to get SFX channel volume, index out of range!\n";
		return -1.f;
	}
		
}

/*!*************************************************************************
****
\brief
	Gets BGM Channel volume, percentage of the master vol
\return
	BGM volume (0.f - 1.f)
****************************************************************************
***/
float Audio::GetMusicVol(){
	BgmChannel->getVolume(&voltemp);
	//return voltemp;
	return BgmVol;
}

/*!*************************************************************************
****
\brief
	Sets Master volume
\param[in] volume
	Volume to set (0.f to 1.f)
****************************************************************************
***/
void Audio::SetMasterVol(float volume) {
	MasterVol = volume;
	Master->setVolume(volume);
}

/*!*************************************************************************
****
\brief
	Sets SFX volume variable, and calls SetSfxChannelVol(), to set each SFX
	channel.
\param[in] volume
	Volume to set (0.f to 1.f), this is before master * sfxvol
****************************************************************************
***/
void Audio::SetSfxVol(float volume)
{
	SfxVol = volume;
	float resultantVol = volume * MasterVol;

	for (int i = 0; i < MaxSfxChannels; i++)
	{
		SetSfxChannelVol(resultantVol, i);
	}
}

/*!*************************************************************************
****
\brief
	Sets SFX Channel volume, only INDIVIDUAL CHANNEL;
\param[in] resultant_vol
	Volume to set (0.f to 1.f), this is after (master * sfxvol)
****************************************************************************
***/
void Audio::SetSfxChannelVol(float resultant_vol, int index) {
	
	if (index >= 0 && index < MaxSfxChannels) {
		//SfxChannel[index]->setPaused(true);
		SfxChannel[index]->setVolume(resultant_vol);
		//SfxChannel[index]->setPaused(false);
	}
	else std::cout << "Unable to set SFX channel volume, index out of range!\n";
}

/*!*************************************************************************
****
\brief
	Sets BGM Channel volume
\param[in] volume
	Volume to set (0.f to 1.f)
****************************************************************************
***/
void Audio::SetMusicVol(float volume) {
	BgmVol = volume;
	BgmChannel->setVolume(BgmVol * MasterVol);
}

/*!*************************************************************************
****
\brief
	Toggles BGM Channel mute
****************************************************************************
***/
void Audio::ToggleMuteMusic() {
	bool res;
	BgmChannel->getMute(&res);
	BgmChannel->setMute(!res);
}

/*!*************************************************************************
****
\brief
	Toggles SFX Channel mute
****************************************************************************
***/
void Audio::ToggleMuteSound() {
	bool res;
	for (int i = 0; i < MaxSfxChannels; ++i) {
		SfxChannel[i]->getMute(&res);
		SfxChannel[i]->setMute(!res);
	}
}

/*!*************************************************************************
****
\brief
	Toggles BGM Channel pause
****************************************************************************
***/
void Audio::ToggleMusicPause() {
	bool res;
	BgmChannel->getPaused(&res);
	BgmChannel->setPaused(!res);
}

/*!*************************************************************************
	****
	\brief
		Returns the value of the maximum sfx channels set
	\return
		int count of the existing sfx channels
	****************************************************************************
	***/
int Audio::GetSfxMaxChannels()
{
	return MaxSfxChannels;
}

Audio* Audio::GetInstance() { if (!audio) audio = new Audio(); return audio; }