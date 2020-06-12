#pragma once
#include "GameObject.h"
#include "Box2D.h"

class Player : public dae::GameObject
{
public:

	// Functions
	Player(dae::Scene* pScene, b2Vec2 position, b2Vec2 size);
	void Update() override;
	void Render() const override;

	void OnTriggerEnter() override;
	void OnTriggerCollision() override;
	void OnTriggerExit() override;

private:

	// Private functions
	void InitSprites();
	void InitStateMachine();

};
