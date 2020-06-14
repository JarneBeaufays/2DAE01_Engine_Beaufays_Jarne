#pragma once
#include "GameObject.h"

class BubbleBullet : public dae::GameObject
{
public:

	// Functions
	BubbleBullet(GameObject* pParent);
	void Update() override;
	void Render() const override;

	// Getters and Setters
	bool GetLookingLeft() const { return m_GoingLeft; }

private:

	// Variables
	const int m_TimeAlive{ 5 };
	float m_CurrentTimeAlive{ 0.0f };
	bool m_GoingLeft = false;
};