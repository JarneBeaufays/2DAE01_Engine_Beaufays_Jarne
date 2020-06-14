#pragma once
#include "GameObject.h"
#include "Box2D.h"
#include "Observer.h"
#include "Scene.h"
#include <memory>

class Player : public dae::GameObject, public Subject
{
public:

	// Functions
	Player(std::shared_ptr<dae::Scene> pScene, b2Vec2 position, b2Vec2 size);
	~Player();
	void Update() override;
	void Render() const override;

	void OnTriggerEnter() override;
	void OnTriggerExit() override;

private:

	// Private functions
	void InitSprites();
	void InitStateMachine();
	void InitControls();
	void InitSounds();
	void HandleShooting();
	void TeleportPlayer();

	// Variables
	std::shared_ptr<dae::Scene> m_spScene;
	const float m_TimeBetweenAttack{ 0.3f };
	float m_CurrentShootTimer{ 0.0f };
	int m_PPM;
	bool m_AllowedToShoot{ true };
	bool m_AllowedToJump{ false };

};
