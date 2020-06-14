#include "Player.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "Renderer.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "SDL.h"
#include "GameComponents.h"
#include "EngineComponents.h"
#include "BubbleBullet.h"
#include "..\Minigin\Time.h"
#include <iostream>

Player::Player(dae::Scene* pScene, b2Vec2 position, b2Vec2 size)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	m_PPM = float(pScene->GetPPM());

	// Adding components	
	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, m_PPM) };
	AddComponent(pBoxCollider);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2BodyType::b2_dynamicBody, 1.f, 1.f, true);
	pRigidBody->SetCollisionGroup(CollisionGroup::colGroup3);
	AddComponent(pRigidBody);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, 1.1f * size) };
	AddComponent(pBoxTrigger);

	TagComponent* pTagComponent{ new TagComponent(this, "Player") };
	AddComponent(pTagComponent);

	HealthComponent* pHealth{ new HealthComponent(this, 4) };
	AddComponent(pHealth);

	SetName("Player");

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

	// Setting scene
	m_pScene = pScene;
}

Player::~Player()
{
	dae::SceneManager::GetInstance().SetActiveScene("Bubble Bobble - EndScreen");
}

void Player::Update()
{
	// Updating children
	GameObject::Update();

	// Handle shooting
	HandleShooting();

	// See if the player's position needs adjusting
	TeleportPlayer();
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
	// Creating an animator
	SpriteComponent* pRun{ new SpriteComponent(this, "Run", "Sprites/Bob_Run.png", 0.05f, 8, 8, true) };
	pRun->SetSize(1.6f, 1.f);

	AnimatorComponent* pAnimator{ new AnimatorComponent(this) };
	pAnimator->AddSprite(pRun);
	AddComponent(pAnimator);
}

void Player::InitStateMachine()
{
	// ----- STATE MACHINE ----- STATE MACHINE ----- STATE MACHINE ----- //
	// ----- STATES ----- //
	State* pDeadState{ new State("Dead") };

	// Idle State
	State* pIdleState{ new State("Idle") };
	std::function<void()> onEntryIdle
	{
		[this]()
		{ 
			AnimatorComponent* pAnim{ static_cast<AnimatorComponent*>(this->GetComponent("AnimatorComponent"))};
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
		}
	};
	pJumpState->AddEntryAction(onEntryJump);

	// Falling state
	State* pFallingState{ new State("Falling") };
	std::function<void()> onEntryFall
	{
		[this]()
		{
			static_cast<RigidBody2D*>(GetComponent("RigidBody2D"))->SetCollisionGroup(CollisionGroup::colGroup5);
		}
	};
	std::function<void()> onExitFall
	{
		[this]()
		{
			static_cast<RigidBody2D*>(GetComponent("RigidBody2D"))->SetCollisionGroup(CollisionGroup::colGroup3);
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

	// Transition: ANY -> Dead
	Transition* pTranAnyToIdle{ new Transition(pDeadState) };
	std::function<bool()> deadCondition
	{
		[this]()
		{
			return this->GetComponent<HealthComponent>()->GetIsDead();
		}
	};
	pTranAnyToIdle->AddCondition(deadCondition);
	// ---



	// Now we create the component and give our starting state to it
	StateMachineComponent* pStateMachine{ new StateMachineComponent(this, pIdleState) };
	pStateMachine->AddAnyStateTransition(pTranAnyToIdle);
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
			if(m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) if (GetComponent<HealthComponent>()->GetIsAlive()) GetComponent<AnimatorComponent>()->SetFlipped(false);
		},

		// OnRelease:
		[]() { },

		// OnDown: Move Right
		[this]()
		{
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) if (GetComponent<HealthComponent>()->GetIsAlive()) GetComponent<RigidBody2D>()->AddForce(b2Vec2(20.f, 0.f));
		}
	) };
	dae::InputManager::GetInstance().CreateInputAction("WalkRight", pWalkRight, PhysicalButton::ButtonD);

	// Walk left
	Command* pWalkLeft{ new Command(
		// OnPress:
		[this]()
		{
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) if (GetComponent<HealthComponent>()->GetIsAlive()) GetComponent<AnimatorComponent>()->SetFlipped(true);
		},

		// OnRelease:
		[]() { },

		// OnDown: Move Right
		[this]()
		{
			// If we are alive, move
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) if(GetComponent<HealthComponent>()->GetIsAlive()) GetComponent<RigidBody2D>()->AddForce(b2Vec2(-20.f, 0.f));
		}) };
	dae::InputManager::GetInstance().CreateInputAction("WalkLeft", pWalkLeft, PhysicalButton::ButtonA);

	// Jump
	Command* pJump{ new Command(
		// OnPress:
		[this]()
		{
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) 
			{
				if (m_AllowedToJump && GetComponent<HealthComponent>()->GetIsAlive())
				{
					Notify(this, ObserverEvent::playerJumped);
					GetComponent<RigidBody2D>()->AddForce(b2Vec2(0.f, 25.f), true);
				}
			}
		},

		// OnRelease:
		[]() { },

		// OnDown: Move Right
		[this](){ }
	) };
	dae::InputManager::GetInstance().CreateInputAction("Jump", pJump, PhysicalButton::ButtonZ);

	// Drop
	Command* pDrop{ new Command(
		// OnPress:
		[this]()
		{
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get())
			{
				if (GetComponent<HealthComponent>()->GetIsAlive() && m_AllowedToJump&& this->GetComponent<RigidBody2D>()->GetPosition().y > 150.f)
				{
					std::cout << "IM: On Press -> Player dropped\n";
					Notify(this, ObserverEvent::playerDropped);
					m_AllowedToJump = false;
				}
			}
		},

		// OnRelease:
		[]() {},

		// OnDown: Move Right
		[this]() {}
		) };
	dae::InputManager::GetInstance().CreateInputAction("Drop", pDrop, PhysicalButton::ButtonS);

	// Shoot
	Command* pShoot{ new Command(
		// OnPress:
		[this]()
		{
			// Let's shoot a bubble!
			if (m_pScene == dae::SceneManager::GetInstance().GetCurrentScene().get()) 
			{
				if (m_AllowedToShoot)
				{
					m_AllowedToShoot = false;
					std::shared_ptr<BubbleBullet> spBullet{ std::make_shared<BubbleBullet>(this) };
					dae::SceneManager::GetInstance().GetCurrentScene()->Add(spBullet);
					dae::SceneManager::GetInstance().GetCurrentScene()->GetCollisionManager()->AddBox(spBullet->GetComponent<BoxTrigger>());
				}
			}
		},
		[]() {},
		[]() {}
		) };
	dae::InputManager::GetInstance().CreateInputAction("Shoot", pShoot, PhysicalButton::ButtonE);
}

void Player::InitSounds()
{
	// Add a sound to our audio manager
	AudioManager::GetInstance().AddSoundEffect("Sounds/Walk.wav", SoundEffectType::walk);
	AudioManager::GetInstance().AddSoundEffect("Sounds/Jump.wav", SoundEffectType::jump);
	AudioManager::GetInstance().AddSoundEffect("Sounds/Drop.wav", SoundEffectType::drop);
}

void Player::HandleShooting()
{
	if (!m_AllowedToShoot) 
	{
		m_CurrentShootTimer += Time::GetInstance().GetDeltaTime();
		if (m_CurrentShootTimer > m_TimeBetweenAttack)
		{
			m_AllowedToShoot = true;
			m_CurrentShootTimer = 0.0f;
		}
	}
}

void Player::TeleportPlayer()
{
	// Getting some variables
	RigidBody2D* pRb{ GetComponent<RigidBody2D>() };
	b2Vec2 position{ pRb->GetPosition() };

	// Check if the player is escaping the map
	if (position.y * m_PPM < -50.f)
	{
		// Set player position to the top
		pRb->SetPosition(b2Vec2{ position.x / m_PPM, (dae::Renderer::GetInstance().GetWindowHeight() - 50.f) / m_PPM });
		pRb->SetLinVelocity(b2Vec2{ 0.f, 0.f });
	}
}

void Player::OnTriggerEnter()
{
	// Get all the triggers that just started
	std::vector<CollisionData*> triggersEntered{ dae::SceneManager::GetInstance().GetCurrentScene()->GetCollisionManager()->GetTriggersEntered() };
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
					if (pGround->GetComponent<RigidBody2D>()->GetPosition().y < pPlayer->GetComponent<RigidBody2D>()->GetPosition().y)
					{
						// Now that we know the ground is below us, we know we are landing on it
						m_AllowedToJump = true;
					}
				}
			}
		}
	}
}

void Player::OnTriggerExit()
{
	// Get all the triggers that just started
	std::vector<CollisionData*> triggersExited{ dae::SceneManager::GetInstance().GetCurrentScene()->GetCollisionManager()->GetTriggersExited() };
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
						if (pGround->GetComponent<RigidBody2D>()->GetPosition().y < pPlayer->GetComponent<RigidBody2D>()->GetPosition().y)
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