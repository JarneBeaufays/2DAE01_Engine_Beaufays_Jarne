#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Transform.h"
#include "SDL_ttf.h"
#include "Font.h"
#include "Renderer.h"
#include "Texture2D.h"

TextComponent::TextComponent(dae::GameObject* pParent, const std::string& text, std::shared_ptr<dae::Font> spFont)
	: m_Text{ text }
	, m_spFont{ spFont }
	, m_spTexture{ nullptr }
	, m_NeedsUpdate{ true }
	, m_Color{ 255, 255, 255 }
	, ObjectComponent("TextComponent", pParent)
{
}

void TextComponent::Update()
{
	// Check if we changed the text
	if (m_NeedsUpdate) 
	{
		// Creating our surface
		const auto pSurface{ TTF_RenderText_Blended(m_spFont->GetFont(), m_Text.c_str(), m_Color) };
		if (!pSurface) return;

		// Creating our texture
		auto pTexture{ SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), pSurface) };
		if (!pTexture) return;

		// Finishing with some variable changes
		SDL_FreeSurface(pSurface);
		m_spTexture = std::make_shared<dae::Texture2D>(pTexture);
		m_NeedsUpdate = false;
	}
}

void TextComponent::Render() const
{
	// Returning if we don't have a texture
	if (!m_spTexture) return;

	// Getting our parent position with our local position
	b2Vec2 positon{ m_pParent->GetTransform().GetPosition() + m_LocalTransform.GetPosition() };
	dae::Renderer::GetInstance().RenderTexture(*m_spTexture, positon.x, positon.y);
}
