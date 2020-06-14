#include "Enemy.h"
#include "GameComponents.h"
#include "EngineComponents.h"
#include "CollisionManager.h"
#include "GameInfo.h"
#include "..\Minigin\Time.h"
#include <iostream>

Enemy::Enemy(dae::Scene* pScene, b2Vec2 position, b2Vec2 size, EnemyType enemyType)
	: m_EnemyType{ enemyType }
{
	// Getting some variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	m_PPM = float(pScene->GetPPM());

	// Creating our components
	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, m_PPM) };
	AddComponent(pBoxCollider);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2_dynamicBody, 1.0f, 0.1f, true);
	pRigidBody->SetCollisionIgnoreGroup(CollisionGroup::colGroup3);
	pRigidBody->SetCollisionGroup(CollisionGroup::colGroup2);
	AddComponent(pRigidBody);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, 1.1f * size) };
	AddComponent(pBoxTrigger);

	TagComponent* pTag{ new TagComponent(this, "Enemy") };
	AddComponent(pTag);

	InitSprites();
	InitStateMachine();
}

void Enemy::Update()
{
	// Update components
	dae::GameObject::Update();

	if(GetComponent<StateMachineComponent>()->GetCurrentStateName() == "Wandering") m_CurrentTimer += Time::GetInstance().GetDeltaTime();
}

void Enemy::InitStateMachine()
{
	// ----- STATE MACHINE ----- STATE MACHINE ----- STATE MACHINE ----- //
	// ----- STATES ----- //
	// Wandering
	State* pWanderingState{ new State("Wandering") };
	
	// Creating actions
	std::function<void()> onEntry{ [this]() {
		AnimatorComponent* temp{ this->GetComponent<AnimatorComponent>() };
		if(temp)temp->SetActiveSprite("Wandering");
		GetComponent<RigidBody2D>()->SetCollisionGroup(CollisionGroup::colGroup2);
	} };

	// Adding actions
	pWanderingState->AddEntryAction(onEntry);

	// Jumping
	State* pJumping{ new State("Jumping") };

	// Creating actions
	std::function<void()> onEntryJump{ [this]() {
		RigidBody2D* pRb{ GetComponent<RigidBody2D>() };
		pRb->SetCollisionGroup(CollisionGroup::colGroup5);
		pRb->SetLinVelocity(b2Vec2{ 0.f, pRb->GetVelocity().y });
		pRb->AddForce(b2Vec2(0.f, 30.f), true);
	} };

	std::function<void()> onExitJump{ [this]() {
		RigidBody2D* pRb{ GetComponent<RigidBody2D>() };
		pRb->SetCollisionGroup(CollisionGroup::colGroup2);
	} };

	// Adding actions
	pJumping->AddEntryAction(onEntryJump);
	pJumping->AddExitAction(onExitJump);

	// Falling
	State* pFalling{ new State("Falling") };

	// Creating actions
	std::function<void()> onEntryFall{ [this]() {
		RigidBody2D* pRb{ GetComponent<RigidBody2D>()};
		pRb->SetCollisionGroup(CollisionGroup::colGroup5);
	} };
	std::function<void()> onExitFall{ [this]() {
		RigidBody2D* pRb{ GetComponent<RigidBody2D>()};
		pRb->SetCollisionGroup(CollisionGroup::colGroup2);
	} };

	// Adding actions
	pFalling->AddEntryAction(onEntryFall);
	pFalling->AddExitAction(onExitFall);

	// InBubble
	State* pInBubble{ new State("InBubble") };

	// Creating actions
	std::function<void()> onEntryBubble{ [this]() {
		GetComponent<RigidBody2D>()->SetCollisionGroup(CollisionGroup::colGroup2);
		AnimatorComponent* temp{ this->GetComponent<AnimatorComponent>() };
		temp->SetActiveSprite("InBubble");
	} };

	std::function<void()> onActionBubble{ [this]() {
		RigidBody2D* temp{ this->GetComponent<RigidBody2D>() };
	} };

	// Adding actions
	pInBubble->AddEntryAction(onEntryBubble);
	pInBubble->AddAction(onActionBubble);
	// ---



	// ----- TRANSITIONS ----- //
	Transition* pTranANYToBubble{ new Transition(pInBubble) };
	std::function<bool()> bubbleCondition
	{
		[this]()
		{
			if (GetComponent<StateMachineComponent>()->GetCurrentStateName() != "InBubble") 
			{
				for (CollisionData* pColData : CollisionManager::GetInstance().GetTriggersEntered())
				{
					// Get tags from triggers
					TagComponent* tagA{ static_cast<TagComponent*>(pColData->GetBoxA()->GetParent()->GetComponent("TagComponent")) };
					TagComponent* tagB{ static_cast<TagComponent*>(pColData->GetBoxB()->GetParent()->GetComponent("TagComponent")) };
					if (tagA && tagB)
					{
						if (tagA->CompareTag("Enemy") || tagB->CompareTag("Enemy"))
						{
							if (tagA->CompareTag("BubbleBullet") || tagB->CompareTag("BubbleBullet"))
							{
								// Getting our base GameObjects
								GameObject* pEnemy;
								GameObject* pBullet;

								if (tagA->CompareTag("Enemy")) { pEnemy = pColData->GetBoxA()->GetParent(); pBullet = pColData->GetBoxB()->GetParent(); }
								else { pEnemy = pColData->GetBoxB()->GetParent(); pBullet = pColData->GetBoxA()->GetParent(); }

								if (pEnemy == this)
								{
									// Delete bubble bullet
									pBullet->Delete();

									// We should go in bubble state!
									return true;
								}
							}
						}
					}
				}
			}

			// We had no collision with a bullet
			return false;
		}
	};
	pTranANYToBubble->AddCondition(bubbleCondition);

	Transition* pTranWanderToJump{ new Transition(pJumping) };
	std::function<bool()> jumpCondition
	{
		[this]()
		{
			RigidBody2D* pRb{ GetComponent<RigidBody2D>() };
			if (m_CurrentTimer > m_TimeWalking && pRb->GetPosition().y < 500)
			{
				// Seeing if our enemy can jump
				m_CurrentTimer = 0.f;
				return true;
			}

			return false;
		}
	};
	pTranWanderToJump->AddCondition(jumpCondition);
	pWanderingState->AddTransition(pTranWanderToJump);

	Transition* pTranJumpToWander{ new Transition(pWanderingState) };
	std::function<bool()> actionToWander
	{
		[this]()
		{
			m_CurrentTimeInAction += Time::GetInstance().GetDeltaTime();
			if (m_CurrentTimeInAction > m_TimeInAction)
			{
				m_CurrentTimeInAction = 0.f;
				m_CurrentTimer = 0.0f;
				return true;
			}
			return false;
		}
	};
	pTranJumpToWander->AddCondition(actionToWander);
	pJumping->AddTransition(pTranJumpToWander);
	
	Transition* pTranWanderToFall{ new Transition(pFalling) };
	std::function<bool()> fallCondition
	{
		[this]()
		{
			RigidBody2D* pRb{ GetComponent<RigidBody2D>() };
			if (m_CurrentTimer > m_TimeWalking&& pRb->GetPosition().y > 500)
			{
				// Seeing if our enemy can fall
				m_CurrentTimer = 0.f;
				return true;
			}

			return false;
		}
	};
	pTranWanderToFall->AddCondition(fallCondition);
	pWanderingState->AddTransition(pTranWanderToFall);

	Transition* pTranFallToWander{ new Transition(pWanderingState) };
	std::function<bool()> fallToWander
	{
		[this]()
		{
			m_CurrentTimeInAction += Time::GetInstance().GetDeltaTime();
			if (m_CurrentTimeInAction > m_TimeFalling)
			{
				m_CurrentTimeInAction = 0.f;
				m_CurrentTimer = 0.0f;
				return true;
			}
			return false;
		}
	};
	pTranFallToWander->AddCondition(fallToWander);
	pFalling->AddTransition(pTranFallToWander);

	Transition* pTranBubbleToWander{ new Transition(pWanderingState) };
	std::function<bool()> wanderCondition
	{
		[this]() 
		{
			m_CurrentTimeInBubble += Time::GetInstance().GetDeltaTime();
			if (m_CurrentTimeInBubble > m_TimeBeforeFreed)
			{
				m_CurrentTimeInBubble = 0.0f;
				return true;
			}
			return false;
		}
	};
	pTranBubbleToWander->AddCondition(wanderCondition);
	pInBubble->AddTransition(pTranBubbleToWander);

	// ---



	// ----- COMPONENT CREATION ----- //
	StateMachineComponent* pStateMachine{ new StateMachineComponent(this, pWanderingState) };
	pStateMachine->AddAnyStateTransition(pTranANYToBubble);
	AddComponent(pStateMachine);
	// ---
}

void Enemy::InitSprites()
{
	// Creating our sprite components
	SpriteComponent* pWander{ new SpriteComponent(this, "Wandering", "Sprites/Zen_Chan.png", 0.05f, 8, 8, true) };
	pWander->SetSize(1.6f, 1.f);
	SpriteComponent* pBubble{ new SpriteComponent(this, "InBubble", "Sprites/Zen_Chan_In_Bubble.png", 0.05f, 8, 8, true) };
	pBubble->SetSize(1.3f, 0.8f);

	// Creating our animator and adding sprites
	AnimatorComponent* pAnimator{ new AnimatorComponent(this) };
	pAnimator->AddSprite(pWander);
	pAnimator->AddSprite(pBubble);
	AddComponent(pAnimator);
}

void Enemy::OnTriggerCollision()
{
	// Get all the triggers that just started
	std::vector<CollisionData*> triggersCollided{ CollisionManager::GetInstance().GetTriggersColliding() };
	for (CollisionData* colData : triggersCollided)
	{
		// Get tags from triggers
		TagComponent* tagA{ static_cast<TagComponent*>(colData->GetBoxA()->GetParent()->GetComponent("TagComponent")) };
		TagComponent* tagB{ static_cast<TagComponent*>(colData->GetBoxB()->GetParent()->GetComponent("TagComponent")) };
		if (tagA && tagB)
		{
			if (tagA->CompareTag("Player") || tagB->CompareTag("Player"))
			{
				if (tagA->CompareTag("Enemy") || tagB->CompareTag("Enemy"))
				{
					// Getting our base GameObjects
					GameObject* pPlayer;
					GameObject* pEnemy;

					if (tagA->CompareTag("Player")) { pPlayer = colData->GetBoxA()->GetParent(); pEnemy = colData->GetBoxB()->GetParent(); }
					else { pPlayer = colData->GetBoxB()->GetParent(); pEnemy = colData->GetBoxA()->GetParent(); }

					if (pEnemy == this) 
					{
						// Checking in what state we are
						StateMachineComponent* pStateMachine{ pEnemy->GetComponent<StateMachineComponent>() };
						std::string currentState{ pStateMachine->GetCurrentStateName() };

						// Checking if we can damage the player
						if (currentState == "InBubble")
						{
							// Player kills enemy -> Player has to get 100 points
							GameInfo::GetInstance().AddPoints(100.f);
							pEnemy->Delete();
							Notify(this, ObserverEvent::killedEnemy);
						}
						else
						{
							// Enemy damages player
							HealthComponent* pHealth{ pPlayer->GetComponent<HealthComponent>() };
							pHealth->DealDamage(1);
						}
					}
				}
			}
		}
	}
}