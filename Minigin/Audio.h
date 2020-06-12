#pragma once
class Audio
{
public:

	virtual ~Audio() = default;
	virtual void PlaySound(int soundID) = 0;
	virtual void StopSound(int soundID) = 0;
	virtual void StopAllSounds() = 0;

};

class NullAudio final : public Audio 
{
	void PlaySound(int soundID) override { int i = soundID; i += 1; };
	void StopSound(int soundID) override { int i = soundID; i += 1; };
	void StopAllSounds() override {};
};