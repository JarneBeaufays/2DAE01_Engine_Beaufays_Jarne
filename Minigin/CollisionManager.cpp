#include "MiniginPCH.h"
#include "CollisionManager.h"

void CollisionManager::Update()
{
	// Update all our datas
	for (CollisionData* pData : m_CollisionDatas) pData->Update();
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

std::vector<CollisionData*> CollisionManager::GetTriggersEntered() const
{
	std::vector<CollisionData*> pTriggers;
	for (CollisionData* pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::started) pTriggers.push_back(pData);
	}
	return pTriggers;
}

std::vector<CollisionData*> CollisionManager::GetTriggersColliding() const
{
	std::vector<CollisionData*> pTriggers;
	for (CollisionData* pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::colliding) pTriggers.push_back(pData);
	}
	return pTriggers;
}

std::vector<CollisionData*> CollisionManager::GetTriggersExited() const
{
	std::vector<CollisionData*> pTriggers;
	for (CollisionData* pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::ended) pTriggers.push_back(pData);
	}
	return pTriggers;
}

void CollisionManager::DeleteBox(BoxTrigger* pBox)
{
	// We want to delete the collision datas with this box
	std::vector<unsigned int> toDelete;

	// Get the different indexes where we can find this box
	for (unsigned int i{}; i < m_CollisionDatas.size(); i++)
	{
		if (m_CollisionDatas[i]->GetBoxA() == pBox || m_CollisionDatas[i]->GetBoxB() == pBox)
		{
			toDelete.push_back(i);
		}
	}

	// Remove all the collisions
	for (unsigned int i{}; i < toDelete.size(); i++) 
	{
		delete m_CollisionDatas[toDelete[i]];
		m_CollisionDatas[toDelete[i]] = m_CollisionDatas[(m_CollisionDatas.size() - 1) - i];
	}

	for (unsigned int i{}; i < toDelete.size(); i++) m_CollisionDatas.pop_back();
}
