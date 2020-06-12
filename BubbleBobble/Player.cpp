#include "Player.h"
#include "RigidBody2D.h"
#include "SpriteComponent.h"
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

	SpriteComponent* pCheffSpriteComponent{ new SpriteComponent(this, "Cheff.png", 0.05f, 12, 4, true) };
	pCheffSpriteComponent->SetSize(2.f, 1.2f);
	AddComponent(pCheffSpriteComponent);

	SpriteComponent* pBombSpriteComponent{ new SpriteComponent(this, "Bomb.png", 0.05f, 10, 5, true) };
	pBombSpriteComponent->SetOffset(-10.f, 50.f);
	pBombSpriteComponent->SetSize(1.6f, 1.f);
	pBombSpriteComponent->Flip();
	AddComponent(pBombSpriteComponent);

	TagComponent* pTagComponent{ new TagComponent(this, "Player") };
	AddComponent(pTagComponent);

	// Creating controls
	Command* pWalkRight{ new Command(
		// OnPress:
		[pRigidBody]() { pRigidBody->AddForce(b2Vec2(20.f, 0.f)); },

		// OnRelease:
		[pRigidBody]() { pRigidBody->AddForce(b2Vec2(20.f, 0.f)); },

		// OnDown: Move Right
		[pRigidBody]() { pRigidBody->AddForce(b2Vec2(20.f, 0.f)); }
	)};
	dae::InputManager::GetInstance().CreateButton(pWalkRight, PhysicalButton::ButtonA);
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

void Player::OnTriggerEnter()
{
	if (true) 
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
	if (true) 
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