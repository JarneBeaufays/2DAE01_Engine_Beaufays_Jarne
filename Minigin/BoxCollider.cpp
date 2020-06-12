#include "MiniginPCH.h"
#include "BoxCollider.h"
#include "RigidBody2D.h"

BoxCollider::BoxCollider(dae::GameObject* pParent, const b2Vec2& offset, const b2Vec2& size, float ppm)
	: m_Offset{ offset }
	, m_Size{ size }
	, ObjectComponent{ "BoxCollider", pParent }
{
	// Creating our shape
	m_Shape.SetAsBox((m_Size.x / ppm) / 2.0f, (m_Size.y / ppm) / 2.0f);

	// Check if we already have a rigidbody
	RigidBody2D* pRigidBody{ static_cast<RigidBody2D*>(m_pParent->GetComponent("RigidBody2D")) };
	if (pRigidBody) pRigidBody->AddCollider(this);
}

BoxCollider::BoxCollider(dae::GameObject* pParent, float xOffset, float yOffset, float width, float height, float ppm)
	: BoxCollider(pParent, b2Vec2{ xOffset, yOffset }, b2Vec2{ width, height }, ppm)
{
}