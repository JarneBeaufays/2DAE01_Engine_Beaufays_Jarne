#pragma once
#include "Box2d.h"
#include "GameObject.h"

class Box : public dae::GameObject
{
public:

	Box() = default;

	void Initialize(dae::Scene* pScene, const b2Vec2& position, const b2Vec2& size);
	void Update() override;
	void Render() const;

};

