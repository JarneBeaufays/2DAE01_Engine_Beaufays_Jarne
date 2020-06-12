#pragma once
#include "Observer.h"
#include "Singleton.h"
#include "GameObject.h"
#include "SoundEffect.h"

class AudioManager : public Observer, public dae::Singleton<AudioManager>
{
public:

	// Functions
	void Init(const std::string& prefix);
	void AddSoundEffect(const std::string& fileName, SoundEffectType type) { m_SoundEffects.push_back(SoundEffect{ std::string(m_Prefix + fileName), type }); }
	void Notify(dae::GameObject* pObject, ObserverEvent event) override;
	void PlaySound(SoundEffectType type) const;

private:

	// Variables
	std::vector<SoundEffect> m_SoundEffects;
	std::string m_Prefix;

};

