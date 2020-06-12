#pragma once
#include "Box2D.h"

// This is not a component! This can be used in components as a local bonus transform
class LocalTransform
{
public:

	// Getters and Setters
	const b2Vec2& GetPosition() const { return m_Position; }
	const b2Vec2& GetSize() const { return m_Size; }
	float GetAngle() const { return m_Angle; }
	void SetPosition(b2Vec2 position) { m_Position.x = position.x; m_Position.y = -position.y; }
	void SetSize(b2Vec2 size) { m_Size = size; }
	void SetAngle(float angle) { m_Angle = angle; }

private:

	// Variables
	b2Vec2 m_Position{ 0.f, 0.f };
	b2Vec2 m_Size{ 1.f, 1.f };
	float m_Angle{ 0.0f };

};