#pragma once
#include "GameObject.h"

class HighScore : public dae::GameObject
{
public:

	// Functions
	HighScore();
	void Update() override;
	void AddPoints(float score) { m_Points += score; }

private:

	// Variables
	float m_Points{ 0.0f };
	float m_CurrentTimer{ 0.0f };
	int m_SecondsInGame{ 0 };

};

