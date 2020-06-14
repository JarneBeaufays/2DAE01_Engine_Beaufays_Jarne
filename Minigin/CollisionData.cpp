#include "MiniginPCH.h"
#include "CollisionData.h"

#include "Renderer.h"

CollisionData::CollisionData(BoxTrigger* a, BoxTrigger* b)
	: m_TriggerState{ TriggerState::idle }
	, m_pBoxA{ a }
	, m_pBoxB{ b }
{
}

void CollisionData::Update()
{
	if (!m_pBoxA || !m_pBoxB) return;

	// In here we check if we started colliding / are colliding / stopped colliding
	switch (m_TriggerState)
	{
	case TriggerState::idle:
		if (RectInRect()) m_TriggerState = TriggerState::started;
		break;

	case TriggerState::started:
		if (RectInRect()) m_TriggerState = TriggerState::colliding;
		else m_TriggerState = TriggerState::ended;
		break;

	case TriggerState::colliding:
		if (!RectInRect()) m_TriggerState = TriggerState::ended;
		break;

	case TriggerState::ended:
		if (RectInRect()) m_TriggerState = TriggerState::started;
		else m_TriggerState = TriggerState::idle;
		break;
	}
}

bool CollisionData::RectInRect() const
{
	if (!m_pBoxA || !m_pBoxB) false;

	// Getting some variables
	Rectangle2V col{ m_pBoxA->GetRectangle() };
	Rectangle2V other{ m_pBoxB->GetRectangle() };

	// Check if rectangle is left of the other
	if ((col.m_Position.x + col.m_Size.x) < other.m_Position.x || (other.m_Position.x + other.m_Size.x) < col.m_Position.x) return false;

	// Check if rectangle is under the other
	if (col.m_Position.y > (other.m_Position.y + other.m_Size.y) || other.m_Position.y > (col.m_Position.y + col.m_Size.y)) return false;

	// We collided!
	return true;
}
