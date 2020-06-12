#include "Ground.h"
#include "SpriteComponent.h"
#include "TagComponent.h"
#include "BoxCollider.h"
#include "BoxTrigger.h"

Ground::Ground(dae::Scene* pScene, const b2Vec2& position, const b2Vec2& size)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	float ppm = float(pScene->GetPPM());

	// Adding components
	SpriteComponent* pSprite{ new SpriteComponent(this, "Idle", "BBSprites/ground.png") };
	AddComponent(pSprite);

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	// BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, size) };
	// AddComponent(pBoxTrigger);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2_staticBody);
	AddComponent(pRigidBody);

	TagComponent* pTagComponent{ new TagComponent(this, "Ground") };
	AddComponent(pTagComponent);
}

void Ground::Update()
{
	// Updating children
	GameObject::Update();
}

void Ground::Render() const
{
	// Rendering children
	GameObject::Render();
}