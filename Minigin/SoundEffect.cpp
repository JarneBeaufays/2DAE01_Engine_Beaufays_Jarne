#include "MiniginPCH.h"
#include "SoundEffect.h"
#include "Renderer.h"

SoundEffect::SoundEffect(const std::string& fileName, SoundEffectType type)
	: m_SoundEffectType{ type }
{
	// Try to parse the soundeffect
	m_pSoundEffect = Mix_LoadWAV(fileName.c_str());

	// See if we get a nullptr
	if (!m_pSoundEffect) std::cout << "Illegal sound effect with name: " << fileName << '\n';
}

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(m_pSoundEffect);
}

void SoundEffect::Play() const
{
	// Play the sound: -1 = channel, 0 = loops?
	if (m_pSoundEffect) 
	{
		// If the sound is not playing already, play it
		//if(!Mix_Playing(-1))
			Mix_PlayChannel(-1, m_pSoundEffect, 0);
	}
}
