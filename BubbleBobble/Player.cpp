#include "Player.h"
#include "RigidBody2D.h"
#include "SpriteComponent.h"
#include "AnimatorComponent.h"
#include "StateMachineComponent.h"
#include "TagComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "AudioManager.h"
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
	float ppm = float(pScene->GetPPM());

	// Adding components	
	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2BodyType::b2_dynamicBody, 1.f, 1.f, true);
	AddComponent(pRigidBody);

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, 1.1f * size) };
	AddComponent(pBoxTrigger);

	TagComponent* pTagComponent{ new TagComponent(this, "Player") };
	AddComponent(pTagComponent);

	// Add Observer
	AddObserver(static_cast<Observer*>(&AudioManager::GetInstance()));

	// Sprite components
	InitSprites();

	// State machine component
	InitStateMachine();

	// Creating controls
	InitControls();

	// Creating our sounds
	InitSounds();
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
		box.x = int(position.x - (size.x / 2.0f));
		box.y = int(position.y - (size.y / 2.0f));
		box.w = int(size.x);
		box.h = int(size.y);

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

	SpriteComponent* pCheffJump{ new SpriteComponent(this, "Jump", "Sprites/Cheff_Jump.png", 0.05f, 4, 4, true) };
	pCheffJump->SetSize(2.f, 1.2f);

	SpriteComponent* pCheffLand{ new SpriteComponent(this, "Fall", "Sprites/Cheff_Fall.png") };
	pCheffLand->SetSize(2.f, 1.2f);

	AnimatorComponent* pAnimator{ new AnimatorComponent(this) };
	pAnimator->AddSprite(pCheffIdle);
	pAnimator->AddSprite(pCheffRun);
	pAnimator->AddSprite(pCheffJump);
	pAnimator->AddSprite(pCheffLand);
	AddComponent(pAnimator);
}

void Player::InitStateMachine()
{
	// ----- STATE MACHINE ----- STATE MACHINE ----- STATE MACHINE ----- //
	// ----- STATES ----- //
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

	std::function<void()> onRunning
	{
		[this]()
		{
			Notify(this, ObserverEvent::playerStartedRunning);
		}
	};
	pRunningState->AddAction(onRunning);

	// Jumping state
	State* pJumpState{ new State("Jump") };
	std::function<void()> onEntryJump
	{
		[this]()
		{
			static_cast<RigidBody2D*>(GetComponent("RigidBody2D"))->SetCollisionGroup(CollisionGroup::colGroup5);
			static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent"))->SetActiveSprite("Jump");
		}
	};
	pJumpState->AddEntryAction(onEntryJump);

	// Falling state
	State* pFallingState{ new State("Falling") };
	std::function<void()> onEntryFall
	{
		[this]()
		{
			static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent"))->SetActiveSprite("Fall");
		}
	};
	std::function<void()> onExitFall
	{
		[this]()
		{
			static_cast<RigidBody2D*>(GetComponent("RigidBody2D"))->SetCollisionGroup(CollisionGroup::colGroup1);
		}
	};
	pFallingState->AddEntryAction(onEntryFall);
	pFallingState->AddExitAction(onExitFall);
	// ---



	// ----- TRANSITIONS ----- //
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

	// Transition: Idle -> Jumping
	Transition* pTranIdleToJump{ new Transition(pJumpState) };
	std::function<bool()> jumpingCondition
	{
		[]()
		{
			return dae::InputManager::GetInstance().InputActionPressed("Jump");
		}
	};
	pTranIdleToJump->AddCondition(jumpingCondition);
	pIdleState->AddTransition(pTranIdleToJump);

	// Transition: Running -> Jumping
	Transition* pTranRunToJump{ new Transition(pJumpState) };
	pTranRunToJump->AddCondition(jumpingCondition);
	pRunningState->AddTransition(pTranRunToJump);

	// Transition: Jumping -> Falling
	Transition* pTranJumpToFall{ new Transition(pFallingState) };
	std::function<bool()> fallingCondition
	{
		[this]()
		{
			return static_cast<RigidBody2D*>(GetComponent("RigidBody2D"))->GetVelocity().y < 0;
		}
	};
	pTranJumpToFall->AddCondition(fallingCondition);
	pJumpState->AddTransition(pTranJumpToFall);

	// Transition: Falling -> Idle
	Transition* pTranFallToIdle{ new Transition(pIdleState) };
	std::function<bool()> landingCondition
	{
		[this]()
		{
			return m_AllowedToJump;
		}
	};
	pTranFallToIdle->AddCondition(landingCondition);
	pFallingState->AddTransition(pTranFallToIdle);

	// Transition: Idle -> Falling
	Transition* pTranIdleToFall{ new Transition(pFallingState) };
	std::function<bool()> fallingCondition2
	{
		[this]()
		{
			return !m_AllowedToJump;
		}
	};
	pTranIdleToFall->AddCondition(fallingCondition2);
	pIdleState->AddTransition(pTranIdleToFall);

	// Transition: Running -> Falling
	Transition* pTranRunningToFall{ new Transition(pFallingState) };
	pTranRunningToFall->AddCondition(fallingCondition2);
	pRunningState->AddTransition(pTranRunningToFall);

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
	// ---



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

	// Jump
	Command* pJump{ new Command(
		// OnPress:
		[this]()
		{
			if (this->m_AllowedToJump) 
			{
				std::cout << "IM: On Press -> Player jumped\n";
				Notify(this, ObserverEvent::playerJumped);
				static_cast<RigidBody2D*>(this->GetComponent("RigidBody2D"))->AddForce(b2Vec2(0.f, 25.f), true);
			}
		},

		// OnRelease:
		[]() { },

		// OnDown: Move Right
		[this](){ }
	) };
	dae::InputManager::GetInstance().CreateInputAction("Jump", pJump, PhysicalButton::ButtonZ);

}

void Player::InitSounds()
{
	// Add a sound to our audio manager
	AudioManager::GetInstance().AddSoundEffect("Sounds/Walk.wav", SoundEffectType::walk);
	AudioManager::GetInstance().AddSoundEffect("Sounds/Jump.wav", SoundEffectType::jump);
}

void Player::OnTriggerEnter()
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
			if (tagA->CompareTag("Player") || tagB->CompareTag("Player")) 
			{
				if (tagA->CompareTag("Ground") || tagB->CompareTag("Ground")) 
				{
					// Getting our base GameObjects
					GameObject* pPlayer;
					GameObject* pGround;

					if (tagA->CompareTag("Player")) { pPlayer = colData->GetBoxA()->GetParent(); pGround = colData->GetBoxB()->GetParent(); }
					else { pPlayer = colData->GetBoxB()->GetParent(); pGround = colData->GetBoxA()->GetParent(); }

					// Checking if the ground is below us
					if (pGround->GetTransform().GetPosition().y > pPlayer->GetTransform().GetPosition().y)
					{
						// Now that we know the ground is below us, we know we are landing on it
						m_AllowedToJump = true;
					}
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
			if (m_AllowedToJump) 
			{
				if (tagA->CompareTag("Player") || tagB->CompareTag("Player"))
				{
					if (tagA->CompareTag("Ground") || tagB->CompareTag("Ground"))
					{
						// Getting our base GameObjects
						GameObject* pPlayer;
						GameObject* pGround;

						if (tagA->CompareTag("Player")) { pPlayer = colData->GetBoxA()->GetParent(); pGround = colData->GetBoxB()->GetParent(); }
						else { pPlayer = colData->GetBoxB()->GetParent(); pGround = colData->GetBoxA()->GetParent(); }

						// Checking if the ground is below us
						if (pGround->GetTransform().GetPosition().y > pPlayer->GetTransform().GetPosition().y)
						{
							// Now that we know the ground is below us, we know we are jumping
							m_AllowedToJump = false;
						}
					}
				}
			}
		}
	}
}