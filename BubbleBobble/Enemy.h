#pragma once
#include "GameObject.h"
#include "Observer.h"

enum class EnemyType 
{
	Maita, ZenChan
};

class Enemy : public dae::GameObject, public Subject
{
public:

	// Functions
	Enemy(dae::Scene* pScene, b2Vec2 position, b2Vec2 size, EnemyType enemyType);
	void Update() override;
	void Render() const override { dae::GameObject::Render(); };
	
	void OnTriggerCollision() override;

private:

	// Private functions
	void InitStateMachine();
	void InitSprites();

	// Variables
	EnemyType m_EnemyType;
	const int m_TimeWalking{ 2 };
	const int m_TimeInAction{ 1 };
	const float m_TimeFalling{ 1.5f };
	const int m_TimeBeforeFreed{ 4 };
	float m_CurrentTimeInBubble{ 0.f };
	float m_PPM{ 0.f };
	float m_CurrentTimer{ 0.f };
	float m_CurrentTimeInAction{ 0.f };
	bool m_InBubble{ false };

};

