#pragma once

#include <HAPI_lib.h>

enum soundID
{
	click = 0,
	buy = 1,
	sell = 2,
	endGameJingle = 3,
	upgrade = 4
};

class SoundComponent
{
public:
	static SoundComponent& getInstance()
	{
		static SoundComponent instance; 
		// Instantiated on first use.
		return instance;
	}

	void SwitchSong(int songNum);
	void PlayASound(int soundID);
private:
	SoundComponent();

	// technique of deleting the methods we don't want.
	SoundComponent(SoundComponent const&) = delete;
	void operator=(SoundComponent const&) = delete;

	std::string songList[3]; //list of names of tracks here, could auto load but wouldn't be worth it for 3 songs
	int fxList[5];
};