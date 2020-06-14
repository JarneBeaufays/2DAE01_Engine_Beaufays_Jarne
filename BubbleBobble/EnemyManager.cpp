#include "EnemyManager.h"
#include "..\Minigin\Time.h"
#include "SceneManager.h"

EnemyManager::EnemyManager(std::shared_ptr<dae::Scene> pScene,  const int amount)
	: m_AmountRemaining{ amount }
	, m_pScene{ pScene }
{
}

void EnemyManager::Update()
{
	if (m_AmountRemaining <= 0) 
	{
		m_CurrentTimer += Time::GetInstance().GetDeltaTime();
		if (m_CurrentTimer > m_SecondsBeforeSwap)
		{ 
			dae::SceneManager::GetInstance().NextScene();
		}
	}
}

void EnemyManager::Notify(dae::GameObject* pObject, ObserverEvent event)
{
	if (event == ObserverEvent::killedEnemy) m_AmountRemaining--;
}
