#pragma once
#include "SDL_mixer.h"
#include <string>

enum class SoundEffectType 
{
	walk, jump, land, drop
};

class SoundEffect
{
public:

	// Functions
	SoundEffect(const std::string& fileName, SoundEffectType type);
	~SoundEffect();
	SoundEffectType GetType() const { return m_SoundEffectType; }
	void Play() const;

private:

	// Variables
	Mix_Chunk* m_pSoundEffect;
	SoundEffectType m_SoundEffectType;

};