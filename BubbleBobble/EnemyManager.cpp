#include "EnemyManager.h"
#include "..\Minigin\Time.h"
#include "SceneManager.h"

EnemyManager::EnemyManager(dae::Scene* pScene,  const int amount)
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
			if (dae::SceneManager::GetInstance().GetCurrentScene()->GetName() == "Bubble Bobble - Level 1") 
			{
				dae::SceneManager::GetInstance().SetActiveScene("Bubble Bobble - Level 2");
			}
			else
			{
				dae::SceneManager::GetInstance().SetActiveScene("Bubble Bobble - Win");
			}
		}
	}
}

void EnemyManager::Notify(dae::GameObject* pObject, ObserverEvent event)
{
	if (event == ObserverEvent::killedEnemy && m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) m_AmountRemaining--;
}
