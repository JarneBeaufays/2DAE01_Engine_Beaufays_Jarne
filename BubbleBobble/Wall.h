#pragma once
#include "Headers.h"
#include "Box2D.h"
#include "Scene.h"
#include <memory>

class Wall : public dae::GameObject
{
public:

	// Functions
	Wall(std::shared_ptr<dae::Scene> pScene, const b2Vec2& position, const b2Vec2& size, bool ignoreColPlayerJump = false, bool invis = false);
	void Update() override;
	void Render() const override;

};

