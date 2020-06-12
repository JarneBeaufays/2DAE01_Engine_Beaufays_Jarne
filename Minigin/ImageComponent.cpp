#include "MiniginPCH.h"
#include "ImageComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SDL.h"

ImageComponent::ImageComponent(dae::GameObject* pParent, std::string filePath, bool drawAroundCenter)
	: m_spTexture{ dae::ResourceManager::GetInstance().LoadTexture(filePath) }
	, m_Size{ 10.f, 10.f }
	, m_KeepOriginalSize{ true }
	, m_UsePosAsCenter{ drawAroundCenter }
	, m_Offset{ }
	, ObjectComponent{ "ImageComponent", pParent }
{
}

void ImageComponent::Render() const
{
	// Getting the parent position
	const b2Vec2 parentPosition{ m_pParent->GetTransform().GetPosition() };

	// Creating our size + pos variables
	float x,y,sx,sy;
	x = m_Offset.x + parentPosition.x;
	y = m_Offset.y + parentPosition.y;
	sx = 1.f;
	sy = 1.f;

	// Doing some checks
	if (!m_KeepOriginalSize) { sx = m_Size.x; sy = m_Size.y; }
	if (m_UsePosAsCenter) { x -= (m_Size.x / 2.f); y -= (m_Size.y / 2.f); }

	// Rendering our texture
	if(m_pRigidBody) dae::Renderer::GetInstance().RenderTexture(*m_spTexture, x, y, sx, sy, -m_pRigidBody->GetAngleDeg(), false);
	else dae::Renderer::GetInstance().RenderTexture(*m_spTexture, x, y, sx, sy);
}

void ImageComponent::Resize(const b2Vec2& size)
{
	// Give the variables to the functions below
	Resize(size.x, size.y);
}

void ImageComponent::Resize(float width, float height)
{
	// Changing the size of our image
	m_KeepOriginalSize = false;
	m_Size.x = width;
	m_Size.y = height;
}
