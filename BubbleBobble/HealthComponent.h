#pragma once
#include "GameObject.h"
#include "ObjectComponent.h"

class HealthComponent : public ObjectComponent
{
public:

	// Functions
	HealthComponent(dae::GameObject* pParent, int maxHealth, int secondsInvincibleAfterHit = 1);
	void Update() override;
	void Render() const override { };
	void DealDamage(int damage);

	// Getters and Setters
	int GetHealth() const { return m_CurrentHealth; }
	int GetIsDead() const { return !m_Alive; }
	int GetIsAlive() const { return m_Alive; }

private:

	// Variables
	const int m_MaxHealth;
	const int m_InvincibleAfterHit;
	float m_CurrentTimerInvincible;
	int m_CurrentHealth;
	bool m_Alive;
	bool m_Invincible;

};

