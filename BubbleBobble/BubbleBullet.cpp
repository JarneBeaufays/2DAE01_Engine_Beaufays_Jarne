#include "BubbleBullet.h"
#include "GameComponents.h"
#include "EngineComponents.h"
#include "CollisionManager.h"
#include "SceneManager.h"

BubbleBullet::BubbleBullet(GameObject* pParent)
{
	// Setting some variables
	GetTransform().SetPosition(pParent->GetTransform().GetPosition());
	GetTransform().SetSize(30.f, 30.f);

	// Setting components
	TagComponent* pTag{ new TagComponent(this, "BubbleBullet") };
	AddComponent(pTag);

	BoxCollider* pBoxCollider{ new BoxCollider(this, b2Vec2(), GetTransform().GetSize(), dae::SceneManager::GetInstance().GetCurrentScene()->GetPPM()) };
	AddComponent(pBoxCollider);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, GetTransform().GetPosition(), 1.1f * GetTransform().GetSize()) };
	AddComponent(pBoxTrigger);

	SpriteComponent* pSprite{ new SpriteComponent(this, "BubbleBullet", "Sprites/Bubble.png") };
	AddComponent(pSprite);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(dae::SceneManager::GetInstance().GetCurrentScene().get(), GetTransform().GetSize(), GetTransform().GetPosition());
	pRigidBody->SetCollisionIgnoreGroup(CollisionGroup::colGroup2); // Enemies
	pRigidBody->SetCollisionIgnoreGroup(CollisionGroup::colGroup3); // Player
	pRigidBody->SetGravity(false);
	AddComponent(pRigidBody);

	State* pShooting = new State("Shooting");
	State* pFlying = new State("Flying");
	std::function<void()> entryFly
	{
		[this]()
		{
			// Resetting our velocity
			auto rb{ GetComponent<RigidBody2D>() };
			rb->SetLinVelocity(b2Vec2(0.f, 0.f));
			rb->SetCollision(false);
			rb->SetGravity(true);
		}
	};
	pFlying->AddEntryAction(entryFly);


	Transition* pTranShootToFly = new Transition(pFlying);
	std::function<bool()> flyCondition
	{
		[this]() 
		{
			// Check if our velocity is small
			if ((!this->GetLookingLeft() && GetComponent<RigidBody2D>()->GetVelocity().x < 0.001f) || (this->GetLookingLeft() && GetComponent<RigidBody2D>()->GetVelocity().x > -0.001f)) return true;
			return false;
		}
	};
	pTranShootToFly->AddCondition(flyCondition);
	pShooting->AddTransition(pTranShootToFly);

	StateMachineComponent* pMachine{ new StateMachineComponent(this, pShooting) };
	AddComponent(pMachine);

	// Check what way player is looking
	m_GoingLeft = pParent->GetComponent<AnimatorComponent>()->GetFlipped();
	pRigidBody->AddForce(b2Vec2{ m_GoingLeft ? -30.f : 20.f, 0.f }, true);
}

void BubbleBullet::Update()
{
	// Update children
	dae::GameObject::Update();
}

void BubbleBullet::Render() const
{
	dae::GameObject::Render();
}
