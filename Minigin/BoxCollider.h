#pragma once
#include "ObjectComponent.h"
#include "Box2D.h"
#include "GameObject.h"

class BoxCollider : public ObjectComponent
{
public:

	// Functions
	BoxCollider(dae::GameObject* pParent, const b2Vec2& offset, const b2Vec2& size, float ppm);
	BoxCollider(dae::GameObject* pParent, float xOffset, float yOffset, float width, float height, float ppm);
	void Update() {};
	void Render() const {};

	// Getters and Setters
	b2PolygonShape& GetShape() { return m_Shape; }

private:

	// Variables
	b2PolygonShape m_Shape;
	b2Vec2 m_Offset;
	b2Vec2 m_Size;

};

