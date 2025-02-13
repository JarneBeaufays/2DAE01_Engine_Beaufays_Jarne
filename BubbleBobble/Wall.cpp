#include "Wall.h"
#include "RigidBody2D.h"
#include "BoxTrigger.h"
#include "TagComponent.h"
#include "SDL.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "SpriteComponent.h"
#include <memory>

Wall::Wall(std::shared_ptr<dae::Scene> pScene, const b2Vec2& position, const b2Vec2& size, bool ignoreColPlayerJump, bool invis)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	float ppm = float(pScene->GetPPM());

	// Adding components
	if (!invis) 
	{
		SpriteComponent* pSprite{ new SpriteComponent(this, "Idle", "BBSprites/wall.png") };
		AddComponent(pSprite);
	}

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	BoxTrigger* pBoxTrigger{ new BoxTrigger(this, position, 1.1f * size) };
	AddComponent(pBoxTrigger);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2_staticBody);
	if(!ignoreColPlayerJump) pRigidBody->SetCollisionIgnoreGroup(CollisionGroup::colGroup5);
	AddComponent(pRigidBody);

	TagComponent* pTagComponent{ new TagComponent(this, "Wall") };
	AddComponent(pTagComponent);
}

void Wall::Update()
{
	// Updating children
	GameObject::Update();
}

void Wall::Render() const
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
