#pragma once
#include "ObjectComponent.h"
#include "Box2d.h"

namespace dae { class Scene; }

enum class CollisionGroup 
{
	none = 0x0000,
	colGroup1 = 0x0001,
	colGroup2 = 0x0002,
	colGroup3 = 0x0004,
	colGroup4 = 0x0008,
	colGroup5 = 0x0010,
	colGroup6 = 0x0020,
	colGroup7 = 0x0040,
	colGroup8 = 0x0080,
	colGroup9 = 0x0100,
	colGroup10 = 0x0200,
	colGroup11 = 0x0400,
	colGroup12 = 0x0800,
	colGroup13 = 0x1000,
	colGroup14 = 0x2000,
	colGroup15 = 0x4000,
	colGroup16 = 0x8000
};

class RigidBody2D : public ObjectComponent
{
public:

	// Functions
	RigidBody2D(dae::GameObject* pParent);
	void Initialize(dae::Scene* pScene, const b2Vec2& size, const b2Vec2& position, b2BodyType type = b2_dynamicBody, float density = 1.0f, float friction = 0.1f, bool disableRot = false);
	void Update() override;
	void AddForce(b2Vec2 force, bool pulse = false);
	bool AddCollider(ObjectComponent* pCollider);

	// Setters and Getters
	b2Vec2 GetPosition() const { return b2Vec2{ m_pBody->GetPosition().x * m_PPM, m_pBody->GetPosition().y * m_PPM }; }
	b2Vec2 GetVelocity() const { return b2Vec2{ m_pBody->GetLinearVelocity().x * m_PPM, m_pBody->GetLinearVelocity().y * m_PPM }; }
	float GetAngleRad() const { return m_pParent->GetTransform().GetAngle() / 180.f * 3.1415f; }
	float GetAngleDeg() const { return m_pParent->GetTransform().GetAngle(); }
	void SetPosition(const b2Vec2& position);
	void SetLinVelocity(const b2Vec2& position);
	void SetCollision(bool value);
	void SetCollisionGroup(CollisionGroup group);
	void SetCollisionIgnoreGroup(CollisionGroup group);
	void SetGravity(bool value);

private:

	// Private Functions
	b2Filter GetFilter() const;
	CollisionGroup GetGroup(int i) const;

	// Variables
	b2Body* m_pBody;
	b2Fixture* m_pFixture;
	b2FixtureDef m_FixtureDef;
	std::vector<bool> m_IgnoredGroup;
	CollisionGroup m_CollisionGroup;
	int m_PPM;
	bool m_IsStatic;

	// Static variables
	static const int m_AmountOfCollisionGroups = 6;

};

