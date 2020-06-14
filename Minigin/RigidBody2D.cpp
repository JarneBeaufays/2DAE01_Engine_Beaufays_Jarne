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
	, m_IgnoredGroup{ CollisionGroup::colGroup5 }
{
}

void RigidBody2D::Initialize(dae::Scene* pScene, const b2Vec2& size, const b2Vec2& position, b2BodyType type, float density, float friction, bool disableRot)
{
	// Checking if we have a static body
	if(type != b2BodyType::b2_staticBody) m_IsStatic = false;

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
	if (group == m_IgnoredGroup) return;

	// Setting our now new collision ignore group
	m_IgnoredGroup = group;
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
	for (int i{}; i < m_AmountOfCollisionGroups; i++)
	{
		// We have to do the pow in order to find a correct value to compare in our enum
		if (GetGroup((uint16)pow(2, i)) != m_IgnoredGroup) mask |= (uint16)GetGroup((uint16)pow(2, i));
	}

	// Setting our filter
	filter.categoryBits = (uint16)m_CollisionGroup;
	filter.maskBits = mask;

	// Returning
	return filter;
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