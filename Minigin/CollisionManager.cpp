#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include <algorithm>
#include <vector>

CollisionManager::CollisionManager()
{
	SetName("CollisionManager");
}

CollisionManager::~CollisionManager()
{
	for (CollisionData* pData : m_CollisionDatas) 
	{
		delete pData;
		pData = nullptr;
	}
	m_CollisionDatas.clear();
}

void CollisionManager::Update()
{
	// Add other game objects boxes
	if (m_FirstFrame) 
	{
		auto objects = dae::SceneManager::GetInstance().GetCurrentScene()->GetObjects();
		for(auto spOb : objects)
		{
			BoxTrigger* pBoxTrigger{ dynamic_cast<dae::GameObject*>(spOb.get())->GetComponent<BoxTrigger>() };
			if (pBoxTrigger) AddBox(pBoxTrigger);
		}
		m_FirstFrame = false;
	}

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
	for (auto pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::started) pTriggers.push_back(pData);
	}
	return pTriggers;
}

std::vector<CollisionData*> CollisionManager::GetTriggersColliding() const
{
	std::vector<CollisionData*> pTriggers;
	for (auto pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::colliding) pTriggers.push_back(pData);
	}
	return pTriggers;
}

std::vector<CollisionData*> CollisionManager::GetTriggersExited() const
{
	std::vector<CollisionData*> pTriggers;
	for (auto pData : m_CollisionDatas)
	{
		if (pData->GetState() == TriggerState::ended) pTriggers.push_back(pData);
	}
	return pTriggers;
}

void CollisionManager::DeleteBox(BoxTrigger* pBox)
{
	// Removing this box
	std::vector<CollisionData*> newCollisions;
	for (auto pColData : m_CollisionDatas) 
	{
		if (pColData->GetBoxA() != pBox && pColData->GetBoxB() != pBox) 
		{
			newCollisions.push_back(pColData);
		}
		else
		{
			delete pColData;
			pColData = nullptr;
		}
	}

	m_CollisionDatas = newCollisions;

	/*
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
	auto size{ m_CollisionDatas.size() - 1 };
	for (unsigned int i{}; i < toDelete.size(); i++) 
	{
		if (m_CollisionDatas[toDelete[i]]) 
		{
			if (m_CollisionDatas[toDelete[i]]->GetBoxA() && m_CollisionDatas[toDelete[i]]->GetBoxB())
			{
				delete m_CollisionDatas[toDelete[i]];
				m_CollisionDatas[toDelete[i]] = nullptr;
				m_CollisionDatas[toDelete[i]] = m_CollisionDatas[size - (i * 2)];
			}
		}
	}

	for (unsigned int i{}; i < toDelete.size(); i++) m_CollisionDatas.pop_back();*/
}
