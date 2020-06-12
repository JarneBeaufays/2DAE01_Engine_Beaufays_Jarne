#pragma once
#include "GameObject.h"

class HighScore : public dae::GameObject
{
public:

	// Functions
	HighScore();
	void Update() override;

private:

	// Variables
	float m_CurrentTimer = 0.0f;
	int m_SecondsInGame = 0;

};

