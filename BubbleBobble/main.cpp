#pragma once
#include "Headers.h"
#include "Game.h"
#include <vld.h>

int main(int, char* [])
{
	// Creating our game object
	Game* pGame = new Game{ "Bubble Bobble - By Jarne Beaufays" };

	// Start the loop of the game
	if(pGame->Initialize()) pGame->StartGame();

	// Cleaning up memory
	pGame->Clean();

	// End the engine
	return 0;
}