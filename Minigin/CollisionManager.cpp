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
