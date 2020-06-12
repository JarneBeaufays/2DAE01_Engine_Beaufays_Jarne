#pragma once
#include "Audio.h"

class AudioService
{
public:
	
	static void Init() { audioService = &defaultAudioService; }
	static Audio& GetAudio() { return *audioService; }
	static void RegisterAudioService(Audio* service) { audioService = (service == nullptr) ? &defaultAudioService : service; }

private:
	
	static Audio* audioService;
	static NullAudio defaultAudioService;

};

