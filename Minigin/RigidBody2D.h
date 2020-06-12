#pragma once
#include "ObjectComponent.h"
#include "Box2d.h"

namespace dae { class Scene; }

class RigidBody2D : public ObjectComponent
{
public:

	// Functions
	RigidBody2D(dae::GameObject* pParent);
	void Initialize(dae::Scene* pScene, const b2Vec2& size, const b2Vec2& position, b2BodyType type = b2_dynamicBody, float density = 1.0f, float friction = 1.0f, bool disableRot = false);
	void Update() override;
	void AddForce(b2Vec2 force, bool pulse = false);
	bool AddCollider(ObjectComponent* pCollider);

	// Setters and Getters
	b2Vec2 GetPosition() const { return b2Vec2{ m_pBody->GetPosition().x * m_PPM, m_pBody->GetPosition().y * m_PPM }; }
	b2Vec2 GetVelocity() const { return b2Vec2{ m_pBody->GetLinearVelocity().x * m_PPM, m_pBody->GetLinearVelocity().y * m_PPM }; }
	float GetAngleRad() const { return m_pParent->GetTransform().GetAngle() / 180.f * 3.1415f; }
	float GetAngleDeg() const { return m_pParent->GetTransform().GetAngle(); }

private:

	// Private variables
	b2Body* m_pBody;
	b2Fixture* m_pFixture;
	b2FixtureDef m_FixtureDef;
	int m_PPM;
	bool m_IsStatic;

};

