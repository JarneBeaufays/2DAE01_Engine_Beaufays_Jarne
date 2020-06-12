#include "MiniginPCH.h"
#include "RigidBody.h"

RigidBody::RigidBody(float mass)
	: ObjectComponent("RigidBody")
	, m_Mass{ mass }
{
}

void RigidBody::SetMass(float mass)
{
	m_Mass = mass;
}

float RigidBody::GetMass() const
{
	return m_Mass;
}
