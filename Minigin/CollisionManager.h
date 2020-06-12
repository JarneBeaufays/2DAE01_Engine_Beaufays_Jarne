#pragma once
#include "Singleton.h"
#include "CollisionData.h"
#include <vector>

class CollisionManager : public dae::Singleton<CollisionManager>
{
public:

	// Functions
	void Update();
	void AddBox(BoxTrigger* pBox);
	void DeleteBox(BoxTrigger* pBox);

	// Getters and Setters
	std::vector<CollisionData*> GetTriggersEntered() const { return m_TriggersEntered; };
	std::vector<CollisionData*> GetTriggersColliding() const { return m_TriggersColliding; };
	std::vector<CollisionData*> GetTriggersExited() const { return m_TriggersExited; };

private:

	// Variables
	std::vector<BoxTrigger*> m_CollisionObjects;
	std::vector<CollisionData*> m_CollisionDatas;

	std::vector<CollisionData*> m_TriggersEntered;
	std::vector<CollisionData*> m_TriggersColliding;
	std::vector<CollisionData*> m_TriggersExited;

};