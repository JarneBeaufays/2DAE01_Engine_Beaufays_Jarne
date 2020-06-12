#pragma once
#include "ObjectComponent.h"
#include "GameObject.h"
#include "Box2D.h"
#include <vector>

struct Rectangle2V 
{
	b2Vec2 m_Position;
	b2Vec2 m_Size;
};

class BoxTrigger : public ObjectComponent
{
public:

	// Functions
	BoxTrigger(dae::GameObject* pParent, const b2Vec2& position, const b2Vec2& size);
	void Update() override;
	void Render() const override;

	// Getters and Setters
	const Rectangle2V& GetRectangle() const { return m_Rectangle; }
	dae::GameObject* GetParent() const { return m_pParent; }

private:

	// Variables
	Rectangle2V m_Rectangle;

};

