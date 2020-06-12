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
{
}

void RigidBody2D::Initialize(dae::Scene* pScene, const b2Vec2& size, const b2Vec2& position, b2BodyType type, float density, float friction, bool disableRot)
{
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

	// Checking if this is a static object
	if (type != b2BodyType::b2_staticBody) 
	{
		// Filling in the fixture def
		m_FixtureDef.density = density;
		m_FixtureDef.friction = friction;
		m_FixtureDef.restitution = 0.2f;
		m_IsStatic = false;
	}

	// See if we already have a collider, return if not
	BoxCollider* pCollider{ static_cast<BoxCollider*>(m_pParent->GetComponent("BoxCollider")) };
	if (!pCollider) return;

	// Creating the fixture
	// For moveable objects
	if (type != b2BodyType::b2_staticBody) 
	{
		// Adding our shape
		m_FixtureDef.shape = &pCollider->GetShape();

		// Setting the fixture
		m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);
	}

	// For static objects
	else
	{
		// If we already have a collider, create the fixture
		m_pFixture = m_pBody->CreateFixture(&pCollider->GetShape(), 0.0f);
	}
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
	// For non static objects
	if (!m_IsStatic)
	{
		// Creating the fixture def
		m_FixtureDef.density = m_FixtureDef.density;
		m_FixtureDef.friction = m_FixtureDef.friction;
		m_FixtureDef.restitution = m_FixtureDef.restitution;
		m_FixtureDef.shape = &pBoxCollider->GetShape();

		// Setting the fixture
		m_pFixture = m_pBody->CreateFixture(&m_FixtureDef);

		// Returning Success!
		return true;
	}
	
	// For static objects
	m_pFixture = m_pBody->CreateFixture(&pBoxCollider->GetShape(), 0.0f);

	// Returning Success!
	return true;
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