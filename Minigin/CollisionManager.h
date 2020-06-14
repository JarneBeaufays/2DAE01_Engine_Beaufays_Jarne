#pragma once
#include "Singleton.h"
#include "CollisionData.h"
#include <vector>
#include <memory>

class CollisionManager : public dae::GameObject
{
public:

	// Functions
	CollisionManager();
	void Update();
	void AddBox(BoxTrigger* pBox);
	void DeleteBox(BoxTrigger* pBox);

	// Getters and Setters
	std::vector<CollisionData*> GetTriggersEntered() const;
	std::vector<CollisionData*> GetTriggersColliding() const;
	std::vector<CollisionData*> GetTriggersExited() const;

private:

	// Variables
	std::vector<BoxTrigger*> m_CollisionObjects;
	std::vector<CollisionData*> m_CollisionDatas;
	bool m_FirstFrame{ true };

};