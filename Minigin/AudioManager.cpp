#include "MiniginPCH.h"
#include "AudioManager.h"
#include "TagComponent.h"
#include "SDL.h"
#include "SDL_mixer.h"

void AudioManager::Destroy()
{
	for (SoundEffect* pSoundEffect : m_SoundEffects)
	{
		delete pSoundEffect;
		pSoundEffect = nullptr;
	}
	m_SoundEffects.clear();
}

void AudioManager::Init(const std::string& prefix)
{
	// Initialize our SDL_Mixer
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	m_Prefix = prefix;
}

void AudioManager::Notify(dae::GameObject* pObject, ObserverEvent event)
{
	// If the gameobject is not our player, return
	if (!static_cast<TagComponent*>(pObject->GetComponent("TagComponent"))->CompareTag("Player")) return;
	
	// Go over all the possible observe events we are interested in
	switch (event)
	{
	case ObserverEvent::playerStartedRunning:
		PlaySound(SoundEffectType::walk);
		break;

	case ObserverEvent::playerJumped:
		PlaySound(SoundEffectType::jump);
		break;

	case ObserverEvent::playerDropped:
		PlaySound(SoundEffectType::drop);
		break;
	}
}

void AudioManager::PlaySound(SoundEffectType type) const
{
	// Loop over all the sound effects
	for (SoundEffect* soundEffect : m_SoundEffects) 
	{
		if (soundEffect->GetType() == type) 
		{
			// Ayee we want to play this sound!
			soundEffect->Play();
		}
	}
}
