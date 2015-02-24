#include "SoundComponent.h"

SoundComponent::SoundComponent()
{
	songList[0] = "sound/HappyJanian.mp3";
	songList[1] = "sound/Sunshine.mp3";

	HAPI->LoadSound("sound/Click01.wav", &fxList[0]);
	HAPI->LoadSound("sound/Buy.wav", &fxList[1]);
	HAPI->LoadSound("sound/ring_inventory.wav", &fxList[2]);
	HAPI->LoadSound("sound/EndGameCountDown.wav", &fxList[3]);
	HAPI->LoadSound("sound/Upgrade.wav", &fxList[4]);
}

void SoundComponent::SwitchSong(int songNum)
{
	HAPI->PlayStreamedMedia(songList[songNum - 1]);
}

void SoundComponent::PlayASound(int soundID)
{
	HAPI->PlayASound(soundID,false,1000,500);
}