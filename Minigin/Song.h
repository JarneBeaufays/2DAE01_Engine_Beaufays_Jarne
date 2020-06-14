#pragma once
#include "SDL_mixer.h"
#include <string>

class Song
{
public:

	// Functions
	Song(const std::string& fileName);
	void Play() const;

private:

	// Variables
	Mix_Music* m_pSong;

};