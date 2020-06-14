#pragma once
#include "GameObject.h"

class SceneTimer : public dae::GameObject
{
public:

	// Functions
	SceneTimer() = default;
	void Update() override;

private:

	// Variables
	float m_CurrentTimer{ 0.f };
	int m_LeaveAfter{ 6 };

};

