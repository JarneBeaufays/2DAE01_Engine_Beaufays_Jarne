#include "MiniginPCH.h"
#include "BoxTrigger.h"
#include "CollisionManager.h"
#include "SDL.h"
#include "Renderer.h"

BoxTrigger::BoxTrigger(dae::GameObject* pParent, const b2Vec2& position, const b2Vec2& size)
	: m_Rectangle{ }
	, ObjectComponent{ "BoxTrigger", pParent }
{
	// Setting some variables
	m_Rectangle.m_Size = size;
	m_Rectangle.m_Position = b2Vec2(position.x - m_Rectangle.m_Size.x / 2.f, position.y - m_Rectangle.m_Size.y / 2.f);

	// Adding our box in our manager
	CollisionManager::GetInstance().AddBox(this);
}

void BoxTrigger::Update()
{
	// Updating our position
	m_Rectangle.m_Position.x = m_pParent->GetTransform().GetPosition().x - m_Rectangle.m_Size.x / 2.f;
	m_Rectangle.m_Position.y = m_pParent->GetTransform().GetPosition().y - m_Rectangle.m_Size.y / 2.f;
}

void BoxTrigger::Render() const 
{
	// Debug
	if (false) 
	{
		// Getting our position and renderer
		auto& renderer{ dae::Renderer::GetInstance() };
		const b2Vec2 position{ m_Rectangle.m_Position };
		const b2Vec2 size{ m_Rectangle.m_Size };

		// Creating a box with correct values
		SDL_Rect box;
		box.x = int(position.x);
		box.y = int(position.y);
		box.w = int(size.x);
		box.h = int(size.y);

		// Using SDL to create magenta color and render rect
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 234, 10, 142, 255);
		SDL_RenderFillRect(renderer.GetSDLRenderer(), &box);
	}
}