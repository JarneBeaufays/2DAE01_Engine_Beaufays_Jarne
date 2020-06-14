#include "SceneTimer.h"
#include "SceneManager.h"
#include "..\Minigin\Time.h"

void SceneTimer::Update()
{
	m_CurrentTimer += Time::GetInstance().GetDeltaTime();
	if (m_CurrentTimer > m_LeaveAfter) 
	{
		// Leave Scene
		dae::SceneManager::GetInstance().SetActiveScene("Bubble Bobble - Level 1");
	}
}
