#pragma once
#include "Headers.h"
#include "Box2D.h"

class Ground : public dae::GameObject
{
public:

	// Functions
	Ground(dae::Scene* pScene, const b2Vec2& position, const b2Vec2& size);
	void Update() override;
	void Render() const override;

};

