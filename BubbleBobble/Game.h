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
	void CreateStartScene();
	void CreateWinScene();
	void CreateSceneOne();
	void CreateSceneTwo();
	void CreateSceneThree();
	void CreateEndScene();

	// Variables
	dae::Minigin* m_pEngine;
	std::string m_GameName;

};

