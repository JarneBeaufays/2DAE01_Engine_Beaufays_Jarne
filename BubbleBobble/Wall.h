#pragma once
#include "Headers.h"
#include "Box2D.h"

class Wall : public dae::GameObject
{
public:

	// Functions
	Wall(dae::Scene* pScene, b2Vec2 position, b2Vec2 size);

	void Update() override;
	void Render() const override;

private:

};

