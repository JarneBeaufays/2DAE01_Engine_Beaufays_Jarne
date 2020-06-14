#pragma once
#include "Headers.h"
#include "Box2D.h"

class Wall : public dae::GameObject
{
public:

	// Functions
	Wall(dae::Scene* pScene, const b2Vec2& position, const b2Vec2& size, bool ignoreColPlayerJump = false, bool invis = false);
	void Update() override;
	void Render() const override;

};

