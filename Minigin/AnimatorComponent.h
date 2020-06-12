#pragma once
#include "GameObject.h"
#include "ObjectComponent.h"
#include "SpriteComponent.h"

class AnimatorComponent : public ObjectComponent
{
public:

	// Functions
	AnimatorComponent(dae::GameObject* pParent) : m_pCurrentSprite{ nullptr }, ObjectComponent { "AnimatorComponent", pParent } { }
	void Update() override;
	void Render() const override;
	void AddSprite(SpriteComponent* pSprite, bool isCurrent = false) { m_Sprites.push_back(pSprite); if (isCurrent || !m_pCurrentSprite) m_pCurrentSprite = pSprite; };

	// Getters and Setters
	bool SetActiveSprite(const std::string& spriteName);
	void SetFlipped(bool value) { for(SpriteComponent* pSprite : m_Sprites) pSprite->SetFlipped(value); }
	void Flip() { for (SpriteComponent* pSprite : m_Sprites) pSprite->Flip(); }

private:

	// Variables
	std::vector<SpriteComponent*> m_Sprites;
	SpriteComponent* m_pCurrentSprite;

};