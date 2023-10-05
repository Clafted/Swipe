#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <irrKlang.h>
#include <string>

/*A Singleton class for handling all sorts of audio*/
class SoundManager
{
public:
	irrklang::ISoundEngine* soundEngine = nullptr;

	~SoundManager()
	{
		soundEngine->drop(); 
		delete soundEngine;
		delete instance;
	}

	static SoundManager* Instance()
	{
		if (!instance)
		{
			instance = new SoundManager();
			instance->soundEngine = irrklang::createIrrKlangDevice();
			if(!instance->soundEngine) std::cout << "Error creating soundEngine!";
		}
		return instance;
	}

private:
	static SoundManager* instance;
	SoundManager() {};
};
#endif