#include "MiniginPCH.h"
#include "AnimatorComponent.h"

void AnimatorComponent::Update()
{
	// Update the sprite we are showing right now
	m_pCurrentSprite->Update();
}

void AnimatorComponent::Render() const
{
	// Update the sprite we are showing right now
	m_pCurrentSprite->Render();
}

bool AnimatorComponent::SetActiveSprite(const std::string& spriteName)
{
	// Look for the given sprite
	for (SpriteComponent* pSprite : m_Sprites) 
	{
		// Compare the given name with our sprite names
		if (pSprite->GetName() == spriteName) { m_pCurrentSprite = pSprite; return true; }
	}

	// Sprite not found
	return false;
}
