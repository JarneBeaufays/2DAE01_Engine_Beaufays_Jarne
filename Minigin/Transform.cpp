#include "MiniginPCH.h"
#include "Transform.h"
#include "Renderer.h"
#include "GameObject.h"

void dae::Transform::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = Renderer::GetInstance().GetWindowHeight() - y;
}

void dae::Transform::SetPosition(const b2Vec2& position)
{
	SetPosition(position.x, position.y);
}

void dae::Transform::SetSize(float x, float y)
{
	m_Size.x = x;
	m_Size.y = y;
}

void dae::Transform::SetSize(const b2Vec2& size)
{
	SetSize(size.x, size.y);
}