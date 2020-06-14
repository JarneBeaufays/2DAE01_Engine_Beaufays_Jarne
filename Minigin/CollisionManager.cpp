#include "MiniginPCH.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
	// Update all our datas
	for (CollisionData* pData : m_CollisionDatas) pData->Update();

	// Getting some variables
	std::vector<CollisionData*> triggerStarted;
	std::vector<CollisionData*> triggerColliding;
	std::vector<CollisionData*> triggerExited;

	// Checking what objects are in what states
	for (CollisionData* pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::started) triggerStarted.push_back(pData);
		else if (pData->GetState() == TriggerState::colliding) triggerColliding.push_back(pData);
		else if (pData->GetState() == TriggerState::ended) triggerExited.push_back(pData);
	}

	// Updating our variables
	m_TriggersEntered = triggerStarted;
	m_TriggersColliding = triggerColliding;
	m_TriggersExited = triggerExited;
}

void CollisionManager::AddBox(BoxTrigger* pBox)
{
	// For each box, we create a collision data with the new box
	m_CollisionObjects.push_back(pBox);
	for (BoxTrigger* pOther : m_CollisionObjects) 
	{
		if (pOther != pBox) m_CollisionDatas.push_back(new CollisionData(pBox, pOther));
	}
}

void CollisionManager::DeleteBox(BoxTrigger* pBox)
{
	// We want to delete the collision datas with this box
	for (unsigned int i{}; i < m_CollisionDatas.size(); i++)
	{
		if (m_CollisionDatas[i]->GetBoxA() == pBox || m_CollisionDatas[i]->GetBoxB() == pBox)
		{
			// We store the one we want to delete in a temp var
			CollisionData* pTemp{ m_CollisionDatas[i] };

			// We move our back in the open spot and popback
			m_CollisionDatas[i] = m_CollisionDatas.back();
			m_CollisionDatas.pop_back();

			// Delete the one that has to go
			delete pTemp;
			pTemp = nullptr;
		}
	}
}
