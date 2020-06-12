#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Time.h"
#include "SDL.h"
#include "Transform.h"
#include "Renderer.h"
#include "ResourceManager.h"

SpriteComponent::SpriteComponent(dae::GameObject* pParent, const std::string& filePath, float frameTime, int frames, int cols, bool loop)
	: m_spTexture{ dae::ResourceManager::GetInstance().LoadTexture(filePath) }
	, m_FrameTime{ frameTime }
	, m_Frames{ frames }
	, m_Cols{ cols }
	, m_Rows{ int(ceilf(frames / float(cols))) }
	, m_AccumulatedTime{ 0.f }
	, m_CurrentFrame{ 0 }
	, m_Loop{ loop }
	, m_UsesCustomSize{ false }
	, m_Animated{ true }
	, m_Flipped{ false }
	, ObjectComponent("SpriteComponent", pParent)
{
}

SpriteComponent::SpriteComponent(dae::GameObject* pParent, const std::string& filePath)
	: m_spTexture{ dae::ResourceManager::GetInstance().LoadTexture(filePath) }
	, m_FrameTime{ 0.f }
	, m_Frames{ 1 }
	, m_Cols{ 1 }
	, m_Rows{ 1 }
	, m_AccumulatedTime{ 0.f }
	, m_CurrentFrame{ 0 }
	, m_Loop{ false }
	, m_UsesCustomSize{ false }
	, m_Animated{ false }
	, m_Flipped{ false }
	, ObjectComponent("SpriteComponent", pParent)
{
}

void SpriteComponent::Update()
{
	// Check if we have to animate
	if (!m_Animated) return;

	// Check if we don't want to loop and are at the end of our sequence
	if (!m_Loop && EndOfSeq()) return;

	// Add our delta time
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();

	// Check if we've reached the time of our frame
	if (m_AccumulatedTime >= m_FrameTime) 
	{
		++m_CurrentFrame %= m_Frames;
		m_AccumulatedTime = 0.f;
	}

	// Check if we want to reset our frames
	if (m_CurrentFrame >= m_Frames) m_CurrentFrame = 0;
}

void SpriteComponent::Render() const
{
	// Getting our variables
	b2Vec2 position{ m_LocalTransform.GetPosition() + m_pParent->GetTransform().GetPosition() };
	b2Vec2 size{ b2Vec2{ m_pParent->GetTransform().GetSize().x * m_LocalTransform.GetSize().x, m_pParent->GetTransform().GetSize().y * m_LocalTransform.GetSize().y } };

	// Getting our coordinates
	float x, y, sx, sy;
	x = position.x - (size.x / 2.f);
	y = position.y - (size.y / 2.f);
	sx = size.x;
	sy = size.y;

	// Check if we have to animate
	if (m_Animated) 
	{
		// Get our column and row
		const int col{ m_CurrentFrame % m_Cols };
		const int row{ m_CurrentFrame / m_Cols };
		
		// Making our source rectangle
		SDL_Rect srcRect;
		srcRect.h = m_spTexture->GetTextureHeight() / m_Rows;
		srcRect.w = m_spTexture->GetTextureWidth() / m_Cols;
		srcRect.y = srcRect.h * row;
		srcRect.x = col * srcRect.w;

		// Drawing our texture
		dae::Renderer::GetInstance().RenderTexture(*m_spTexture, srcRect, x, y, sx, sy, m_pParent->GetTransform().GetAngle() + m_LocalTransform.GetAngle(), m_Flipped);
	}
	else dae::Renderer::GetInstance().RenderTexture(*m_spTexture, x, y, sx, sy, m_pParent->GetTransform().GetAngle() + m_LocalTransform.GetAngle(), m_Flipped);
}
