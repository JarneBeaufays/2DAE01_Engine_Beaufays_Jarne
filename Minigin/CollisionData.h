#pragma once
#include "BoxTrigger.h"

enum class TriggerState
{
	idle = 0,
	started = 1,
	colliding = 2,
	ended = 3
};

class CollisionData
{
public:

	// Functions
	CollisionData(BoxTrigger* a, BoxTrigger* b);
	void Update();

	// Getters and Setters
	TriggerState GetState() const { return m_TriggerState; }
	BoxTrigger* GetBoxA() const { return m_pBoxA; }
	BoxTrigger* GetBoxB() const { return m_pBoxB; }

private:

	// Private functions
	bool RectInRect() const;

	// Variables
	BoxTrigger* m_pBoxA;
	BoxTrigger* m_pBoxB;
	TriggerState m_TriggerState;

};

