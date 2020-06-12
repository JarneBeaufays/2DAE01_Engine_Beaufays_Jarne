#include "Player.h"
#include "RigidBody2D.h"
#include "SpriteComponent.h"
#include "AnimatorComponent.h"
#include "StateMachineComponent.h"
#include "TagComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "SDL.h"
#include <iostream>

Player::Player(dae::Scene* pScene, b2Vec2 position, b2Vec2 size)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	float ppm = pScene->GetPPM();

	// Adding components	
	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2BodyType::b2_dynamicBody);
	AddComponent(pRigidBody);

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, size) };
	AddComponent(pBoxTrigger);

	// Sprite components
	InitSprites();

	TagComponent* pTagComponent{ new TagComponent(this, "Player") };
	AddComponent(pTagComponent);

	// State machine component
	InitStateMachine();

	// Creating controls
	InitControls();
}

void Player::Update()
{
	// Updating children
	GameObject::Update();
}

void Player::Render() const
{
	// Debug
	if (false)
	{
		// Getting our position and renderer
		auto& renderer{ dae::Renderer::GetInstance() };
		const b2Vec2 position{ GetTransform().GetPosition() };
		const b2Vec2 size{ GetTransform().GetSize() };

		// Creating a box with correct values
		SDL_Rect box;
		box.x = position.x - (size.x / 2.0f);
		box.y = position.y - (size.y / 2.0f);
		box.w = size.x;
		box.h = size.y;

		// Using SDL to create magenta color and render rect
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 234, 10, 142, 255);
		SDL_RenderFillRect(renderer.GetSDLRenderer(), &box);
	}

	// Rendering children
	GameObject::Render();
}

void Player::InitSprites()
{
	// ----- SPRITES AND ANIMATIONS ----- //
	// Creating a regular sprite
	SpriteComponent* pBombSpriteComponent{ new SpriteComponent(this, "Fly", "Sprites/Bomb.png", 0.05f, 10, 5, true) };
	pBombSpriteComponent->SetOffset(-10.f, 50.f);
	pBombSpriteComponent->SetSize(1.6f, 1.f);
	pBombSpriteComponent->Flip();
	AddComponent(pBombSpriteComponent);

	// Creating an animator
	SpriteComponent* pCheffIdle{ new SpriteComponent(this, "Idle", "Sprites/Cheff_Idle.png", 0.05f, 16, 4, true) };
	pCheffIdle->SetSize(2.f, 1.2f);

	SpriteComponent* pCheffRun{ new SpriteComponent(this, "Run", "Sprites/Cheff_Run.png", 0.05f, 16, 4, true) };
	pCheffRun->SetSize(2.f, 1.2f);

	AnimatorComponent* pAnimator{ new AnimatorComponent(this) };
	pAnimator->AddSprite(pCheffIdle);
	pAnimator->AddSprite(pCheffRun);
	AddComponent(pAnimator);
}

void Player::InitStateMachine()
{
	// ----- STATE MACHINE ----- //
	// Idle State
	State* pIdleState{ new State("Idle") };
	std::function<void()> onEntryIdle
	{
		[this]()
		{ 
			AnimatorComponent* pAnim{ static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent"))};
			pAnim->SetActiveSprite("Idle");
		}
	};
	pIdleState->AddEntryAction(onEntryIdle);

	// Running State
	State* pRunningState{ new State("Running") };
	std::function<void()> onEntryRunning
	{
		[this]()
		{ 
			static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent"))->SetActiveSprite("Run"); 
		} 
	};
	pRunningState->AddEntryAction(onEntryRunning);

	// Transition: Idle -> Running
	Transition* pTranIdleToRun{ new Transition(pRunningState) };
	std::function<bool()> runningCondition
	{
		[]() 
		{ 
			return dae::InputManager::GetInstance().InputActionPressed("WalkRight") || dae::InputManager::GetInstance().InputActionPressed("WalkLeft"); 
		} 
	};
	pTranIdleToRun->AddCondition(runningCondition);
	pIdleState->AddTransition(pTranIdleToRun);

	// Transition: Running -> Idle
	Transition* pTranRunningToIdle{ new Transition(pIdleState) };
	std::function<bool()> idleCondition
	{ 
		[]() 
		{ 
			return !dae::InputManager::GetInstance().InputActionPressed("WalkRight") && !dae::InputManager::GetInstance().InputActionPressed("WalkLeft"); 
		} 
	};
	pTranRunningToIdle->AddCondition(idleCondition);
	pRunningState->AddTransition(pTranRunningToIdle);

	// Now we create the component and give our starting state to it
	StateMachineComponent* pStateMachine{ new StateMachineComponent(this, pIdleState) };
	AddComponent(pStateMachine);
}

void Player::InitControls()
{
	// ----- CONTROLS ----- //
	// Walk right
	Command* pWalkRight{ new Command(
		// OnPress:
		[this]()
		{ 
			std::cout << "IM: On Press -> Player started walking -> Right ->\n"; 
			AnimatorComponent* pAnim{ static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent")) };
			pAnim->SetFlipped(false);
		},

		// OnRelease:
		[]() {std::cout << "IM: On Release -> Player stopped walking -> Right ->\n"; },

		// OnDown: Move Right
		[this]()
		{
			static_cast<RigidBody2D*>(this->GetComponent("RigidBody2D"))->AddForce(b2Vec2(20.f, 0.f)); 
		}
	) };
	dae::InputManager::GetInstance().CreateInputAction("WalkRight", pWalkRight, PhysicalButton::ButtonD);

	// Walk left
	Command* pWalkLeft{ new Command(
		// OnPress:
		[this]()
		{
			std::cout << "IM: On Press -> Player started walking <- Left <-\n";
			AnimatorComponent* pAnim{ static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent")) };
			pAnim->SetFlipped(true);
		},

		// OnRelease:
		[]() {std::cout << "IM: On Release -> Player stopped walking <- Left <-\n"; },

		// OnDown: Move Right
		[this]()
		{
			static_cast<RigidBody2D*>(this->GetComponent("RigidBody2D"))->AddForce(b2Vec2(-20.f, 0.f));
		}) };
	dae::InputManager::GetInstance().CreateInputAction("WalkLeft", pWalkLeft, PhysicalButton::ButtonA);
}

void Player::OnTriggerEnter()
{
	if (false)
	{
		// Get all the triggers that just started
		std::vector<CollisionData*> triggersEntered{ CollisionManager::GetInstance().GetTriggersEntered() };
		for (CollisionData* colData : triggersEntered)
		{
			// Get tags from triggers
			TagComponent* tagA{ static_cast<TagComponent*>(colData->GetBoxA()->GetParent()->GetComponent("TagComponent")) };
			TagComponent* tagB{ static_cast<TagComponent*>(colData->GetBoxB()->GetParent()->GetComponent("TagComponent")) };
			if (tagA && tagB)
			{
				// If the tags are not nullptrs
				if ((tagA->CompareTag("Player") && tagB->CompareTag("Box")) || (tagA->CompareTag("Box") && tagB->CompareTag("Player")))
				{
					// The player triggerd with a box
					std::cout << "Trigger Entered\n";
				}
			}
		}
	}
}

void Player::OnTriggerCollision()
{
	if (false)
	{
		// Get all the triggers that just started
		int i{  };
		std::vector<CollisionData*> triggersCollided{ CollisionManager::GetInstance().GetTriggersColliding() };
		for (CollisionData* colData : triggersCollided)
		{
			// Get tags from triggers
			TagComponent* tagA{ static_cast<TagComponent*>(colData->GetBoxA()->GetParent()->GetComponent("TagComponent")) };
			TagComponent* tagB{ static_cast<TagComponent*>(colData->GetBoxB()->GetParent()->GetComponent("TagComponent")) };
			if (tagA && tagB)
			{
				// If the tags are not nullptrs
				if ((tagA->CompareTag("Player") && tagB->CompareTag("Box")) || (tagA->CompareTag("Box") && tagB->CompareTag("Player")))
				{
					// The player triggerd with a box
					i++;
				}
			}
		}

		std::cout << std::to_string(i) << '\n';
	}
}

void Player::OnTriggerExit()
{
	if (false)
	{
		// Get all the triggers that just started
		std::vector<CollisionData*> triggersExited{ CollisionManager::GetInstance().GetTriggersExited() };
		for (CollisionData* colData : triggersExited)
		{
			// Get tags from triggers
			TagComponent* tagA{ static_cast<TagComponent*>(colData->GetBoxA()->GetParent()->GetComponent("TagComponent")) };
			TagComponent* tagB{ static_cast<TagComponent*>(colData->GetBoxB()->GetParent()->GetComponent("TagComponent")) };
			if (tagA && tagB)
			{
				// If the tags are not nullptrs
				if ((tagA->CompareTag("Player") && tagB->CompareTag("Box")) || (tagA->CompareTag("Box") && tagB->CompareTag("Player")))
				{
					// The player triggerd with a box
					std::cout << "Trigger Exited\n";
				}
			}
		}
	}
}