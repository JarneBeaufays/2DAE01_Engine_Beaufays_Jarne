#pragma once
#include "ObjectComponent.h"
#include "TextObject.h"
#include "LocalTransform.h"
#include "SDL.h"
#include <string>

class TextComponent : public ObjectComponent
{
public:

	// Functions
	TextComponent(dae::GameObject* pParent, const std::string& text, std::shared_ptr<dae::Font> pFont);
	void Update() override;
	void Render() const override;

	// Getters and Setters
	void SetText(const std::string& text) { m_Text = text; m_NeedsUpdate = true; }
	void SetOffset(b2Vec2 offset) { m_LocalTransform.SetPosition(offset); }
	void SetOffset(float x, float y) { SetOffset(b2Vec2(x, y)); }
	void SetSize(b2Vec2 size) { m_LocalTransform.SetSize(size); }
	void SetSize(float x, float y) { SetSize(b2Vec2(x, y)); }

private:

	// Variables
	std::string m_Text;
	std::shared_ptr<dae::Font> m_spFont;
	std::shared_ptr<dae::Texture2D> m_spTexture;
	LocalTransform m_LocalTransform;
	SDL_Color m_Color;

	bool m_NeedsUpdate;

};