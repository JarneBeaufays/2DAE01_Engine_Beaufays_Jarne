#include "MiniginPCH.h"
#include "RigidBody2D.h"
#include "BoxCollider.h"
#include "Renderer.h"
#include "SDL.h"
#include "Time.h"
#include "Scene.h"

RigidBody2D::RigidBody2D(dae::GameObject* pParent)
	: ObjectComponent{ "RigidBody2D", pParent }
	, m_PPM{ 1 }
	, m_IsStatic{ true }
	, m_CollisionGroup{ CollisionGroup::colGroup1 }
{
	// Setting our collision ignore groups
	m_IgnoredGroup.resize(m_AmountOfCollisionGroups);
	for (int i{}; i < m_AmountOfCollisionGroups; i++) m_IgnoredGroup[i] = false;
	m_IgnoredGroup[5] = true;
}

void RigidBody2D::Initialize(dae::Scene* pScene, const b2Vec2& size, const b2Vec2& position, b2BodyType type, float density, float friction, bool disableRot)
{
	// Checking if we have a static body
	if (type != b2BodyType::b2_staticBody) m_IsStatic = false;

	// We need the pixel per meter for more realistic physics scaling
	m_PPM = pScene->GetPPM();

	// Creating our body definition
	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position.Set((position.x + (size.x / 2.0f)) / m_PPM, (position.y + (size.y / 2.0f)) / m_PPM);

	// Creating our body object
	m_pBody = pScene->CreateBody(&bodyDef);

	// Making sure our Rb doesn't rotate user wants
	m_pBody->SetFixedRotation(disableRot);

	// Filling in the fixture def
	m_FixtureDef.density = m_IsStatic ? 0.0f : density;
	m_FixtureDef.friction = friction;
	m_FixtureDef.restitution = 0.2f;
	m_FixtureDef.filter = GetFilter();

	// See if we already have a collider, return if not
	BoxCollider* pCollider{ static_cast<BoxCollider*>(m_pParent->GetComponent("BoxCollider")) };
	if (!pCollider) return;

	// Creating the fixture
	// Adding our shape
	m_FixtureDef.shape = &pCollider->GetShape();

	// Setting the fixture
	m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);
}

void RigidBody2D::Update()
{
	// Setting our transform position equal to our RigidBody transform * Pixel Per Meter
	b2Vec2 position{ m_pBody->GetPosition() };
	position.x *= m_PPM;
	position.y *= m_PPM;

	// Setting some Transform variables
	m_pParent->GetTransform().SetPosition(position);
	m_pParent->GetTransform().SetAngle(-m_pBody->GetAngle() * 180.f / 3.1415f);
}

bool RigidBody2D::AddCollider(ObjectComponent* pCollider)
{
	// Check if parameter it's a collider and return if it isn't
	BoxCollider* pBoxCollider{ static_cast<BoxCollider*>(pCollider) };
	if (!pBoxCollider) return false;

	// Create a new fixture with the given shape
	m_FixtureDef.density = m_IsStatic ? 0.0f : m_FixtureDef.density;
	m_FixtureDef.friction = m_FixtureDef.friction;
	m_FixtureDef.restitution = m_FixtureDef.restitution;
	m_FixtureDef.filter = m_FixtureDef.filter;
	m_FixtureDef.shape = &pBoxCollider->GetShape();

	// Setting the fixture and deleting the prev one
	m_pBody->DestroyFixture(m_pFixture);
	m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);

	// Returning Success!
	return true;
}

void RigidBody2D::SetPosition(const b2Vec2& position)
{
	// Setting the position
	m_pBody->SetTransform(position, m_pBody->GetAngle());
}

void RigidBody2D::SetLinVelocity(const b2Vec2& position)
{
	// Setting velocity
	m_pBody->SetLinearVelocity(position);
}

void RigidBody2D::SetCollision(bool value)
{
	// Setting the collision of the body on or off
	m_pBody->GetFixtureList()->SetSensor(!value);
}

void RigidBody2D::SetCollisionGroup(CollisionGroup group)
{
	if (group == m_CollisionGroup) return;
	
	// Setting our now new collision group
	m_CollisionGroup = group;
	m_FixtureDef.filter = GetFilter();
	m_pBody->DestroyFixture(m_pFixture);
	m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);
}

void RigidBody2D::SetCollisionIgnoreGroup(CollisionGroup group)
{
	// Setting our now new collision ignore group
	switch (group)
	{
	case CollisionGroup::colGroup1: m_IgnoredGroup[1] = !m_IgnoredGroup[1]; break;
	case CollisionGroup::colGroup2: m_IgnoredGroup[2] = !m_IgnoredGroup[2]; break;
	case CollisionGroup::colGroup3: m_IgnoredGroup[3] = !m_IgnoredGroup[3]; break;
	case CollisionGroup::colGroup4: m_IgnoredGroup[4] = !m_IgnoredGroup[4]; break;
	case CollisionGroup::colGroup5: m_IgnoredGroup[5] = !m_IgnoredGroup[5]; break;
	case CollisionGroup::colGroup6: m_IgnoredGroup[6] = !m_IgnoredGroup[6]; break;
	case CollisionGroup::colGroup7: m_IgnoredGroup[7] = !m_IgnoredGroup[7]; break;
	case CollisionGroup::colGroup8: m_IgnoredGroup[8] = !m_IgnoredGroup[8]; break;
	case CollisionGroup::colGroup9: m_IgnoredGroup[9] = !m_IgnoredGroup[9]; break;
	case CollisionGroup::colGroup10: m_IgnoredGroup[10] = !m_IgnoredGroup[10]; break;
	case CollisionGroup::colGroup11: m_IgnoredGroup[11] = !m_IgnoredGroup[11]; break;
	case CollisionGroup::colGroup12: m_IgnoredGroup[12] = !m_IgnoredGroup[12]; break;
	case CollisionGroup::colGroup13: m_IgnoredGroup[13] = !m_IgnoredGroup[13]; break;
	case CollisionGroup::colGroup14: m_IgnoredGroup[14] = !m_IgnoredGroup[14]; break;
	case CollisionGroup::colGroup15: m_IgnoredGroup[15] = !m_IgnoredGroup[15]; break;
	case CollisionGroup::colGroup16: m_IgnoredGroup[16] = !m_IgnoredGroup[16]; break;
	}
	m_FixtureDef.filter = GetFilter();
	m_pBody->DestroyFixture(m_pFixture);
	m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);
}

b2Filter RigidBody2D::GetFilter() const
{
	// Creating our filter
	b2Filter filter;
	uint16 mask{ 0x0000 };

	// Going over all of our possible collisiongroups if it isn't ignored
	for (int i{ 1 }; i < m_AmountOfCollisionGroups; i++)
	{
		// We have to do the pow in order to find a correct value to compare in our enum
		if (!m_IgnoredGroup[i]) mask |= (uint16)GetGroup(i);
	}

	// Setting our filter
	filter.categoryBits = (uint16)m_CollisionGroup;
	filter.maskBits = mask;

	// Returning
	return filter;
}

CollisionGroup RigidBody2D::GetGroup(int i) const
{
	switch (i)
	{
	case 0: return CollisionGroup::none;
	case 1: return CollisionGroup::colGroup1;
	case 2: return CollisionGroup::colGroup2;
	case 3: return CollisionGroup::colGroup3;
	case 4: return CollisionGroup::colGroup4;
	case 5: return CollisionGroup::colGroup5;
	case 6: return CollisionGroup::colGroup6;
	case 7: return CollisionGroup::colGroup7;
	case 8: return CollisionGroup::colGroup8;
	case 9: return CollisionGroup::colGroup9;
	case 10: return CollisionGroup::colGroup10;
	case 11: return CollisionGroup::colGroup11;
	case 12: return CollisionGroup::colGroup12;
	case 13: return CollisionGroup::colGroup13;
	case 14: return CollisionGroup::colGroup14;
	case 15: return CollisionGroup::colGroup15;
	case 16: return CollisionGroup::colGroup16;
	}

	return CollisionGroup::none;
}

void RigidBody2D::AddForce(b2Vec2 force, bool pulse)
{
	// Multiplying delta time
	if (!pulse)
	{
		float deltaTime{ Time::GetInstance().GetDeltaTime() };
		force.x *= deltaTime;
		force.y *= deltaTime;
	}

	// Adding force
	m_pBody->ApplyLinearImpulse(force, m_pBody->GetWorldCenter(), true);
}

void RigidBody2D::SetGravity(bool value) 
{
	// Setting our gravity to 0 or full gravity scale
	m_pBody->SetGravityScale(value ? 1.0f : 0.0f);
}