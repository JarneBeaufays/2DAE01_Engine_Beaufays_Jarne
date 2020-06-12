#include "Wall.h"
#include "RigidBody2D.h"
#include "TagComponent.h"
#include "SDL.h"
#include "Renderer.h"
#include "BoxCollider.h"
#include "SpriteComponent.h"

Wall::Wall(dae::Scene* pScene, b2Vec2 position, b2Vec2 size)
{
	// Setting variables
	GetTransform().SetPosition(position);
	GetTransform().SetSize(size);
	float ppm = pScene->GetPPM();

	// Adding components
	SpriteComponent* pSprite{ new SpriteComponent(this, "BBSprites/wall.png") };
	AddComponent(pSprite);

	BoxCollider* pBoxCollider{ new BoxCollider(this, 0.0f, 0.0f, size.x, size.y, ppm) };
	AddComponent(pBoxCollider);

	RigidBody2D* pRigidBody{ new RigidBody2D(this) };
	pRigidBody->Initialize(pScene, size, position, b2_staticBody);
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
		box.x = position.x - (size.x / 2.0f);
		box.y = position.y - (size.y / 2.0f);
		box.w = size.x;
		box.h = size.y;

		// Using SDL to create magenta color and render rect
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 234, 10, 142, 255);
		SDL_RenderFillRect(renderer.GetSDLRenderer(), &box);
	}
}
