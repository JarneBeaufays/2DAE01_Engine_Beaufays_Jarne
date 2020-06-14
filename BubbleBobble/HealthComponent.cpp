#include "HealthComponent.h"
#include "SceneManager.h"
#include "..\Minigin\Time.h"
#include <iostream>

HealthComponent::HealthComponent(dae::GameObject* pParent, int maxHealth, int secondsInvincibleAfterHit)
	: m_MaxHealth{ maxHealth }
	, m_InvincibleAfterHit{ secondsInvincibleAfterHit }
	, m_CurrentHealth{ maxHealth }
	, m_CurrentTimerInvincible{ 0.f }
	, m_Alive{ true }
	, m_Invincible{ false }
	, ObjectComponent{ "HealthComponent", pParent }
{
}

void HealthComponent::Update()
{
	// Check if our invincible trigger is running
	if (m_Invincible) 
	{
		m_CurrentTimerInvincible += Time::GetInstance().GetDeltaTime();
		if (m_CurrentTimerInvincible >= m_InvincibleAfterHit) 
		{
			// We need to stop being invincible
			m_Invincible = false;
			m_CurrentTimerInvincible = 0.0f;
		}
	}
}

void HealthComponent::DealDamage(int damage)
{
	// Check if we are invincible
	if (!m_Invincible && m_Alive)
	{
		// Deal damage
		m_CurrentHealth -= damage;
		m_Invincible = true;
		std::cout << "HC: Player took " << std::to_string(damage) << " damage - Health: " << std::to_string(m_CurrentHealth) << '\n';

		// Check if we are dead
		if (m_CurrentHealth > 0) return;

		// We are dead!
		m_Alive = false;
		m_CurrentHealth = 0;
		std::cout << "HC: Auwch ... Player is dead. Better luck next time!\n";
		dae::SceneManager::GetInstance().SetActiveScene("Bubble Bobble - EndScreen");
	}
}
