#pragma once
#include "GameObject.h"
#include "Observer.h"
#include "Scene.h"

class EnemyManager : public Observer, public dae::GameObject
{
public:

	// Functions
	EnemyManager(dae::Scene* pScene, const int amount);
	void Update() override;
	void Notify(dae::GameObject* pObject, ObserverEvent event) override;
	void AddEnemy() { m_AmountRemaining++; }

private:

	// Variables
	dae::Scene* m_pScene;
	float m_CurrentTimer{ 0.0f };
	int m_SecondsBeforeSwap{ 3 };
	int m_AmountRemaining;
	bool m_WantsChange{ false };

};

