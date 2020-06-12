#pragma once
#include "Headers.h"

class Game
{
public:

	// Public Functions
	Game(const std::string& gameName = "Game") : m_GameName{ gameName } { };
	bool Initialize();
	void StartGame();
	void Clean();

private:

	// Private Functions
	bool CreateScenes();
	void CleanPrivatePointers();
	void CreateSceneOne();

	// Variables
	dae::Minigin* m_pEngine;
	std::string m_GameName;

};

