#include "Box.h"
#include "SpriteComponent.h"
#include "TagComponent.h"
#include "RigidBody2D.h"
#include "BoxCollider.h"
#include "BoxTrigger.h"
#include "Renderer.h"

// SDL Includes
#include <SDL.h>

void Box::Initialize(std::shared_ptr<dae::Scene> pScene, const b2Vec2& position, const b2Vec2& size)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	float ppm = float(pScene->GetPPM());

	// Adding components	
	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2BodyType::b2_dynamicBody);
	AddComponent(pRigidBody);

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, 1.2f * size) };
	AddComponent(pBoxTrigger);

	SpriteComponent* pSprite{ new SpriteComponent(this, "Idle", "Box.png") };
	AddComponent(pSprite);

	TagComponent* pTagComponent{ new TagComponent(this, "Box") };
	AddComponent(pTagComponent);
}

void Box::Update()
{
	// Updating children
	GameObject::Update();
}

void Box::Render() const
{
	// Rendering children
	GameObject::Render();

	// Render square if we don't have a imagecomponent
	if (!GetComponent("SpriteComponent"))
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
}
