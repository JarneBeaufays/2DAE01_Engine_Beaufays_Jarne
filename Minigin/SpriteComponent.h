#pragma once
#include "ObjectComponent.h"
#include "ImageComponent.h"
#include "LocalTransform.h"
#include <memory>

class SpriteComponent : public ObjectComponent
{
public:

	// Functions
	SpriteComponent(dae::GameObject* pParent, const std::string& filePath, float frameTime, int frames, int cols, bool loop);
	SpriteComponent(dae::GameObject* pParent, const std::string& filePath);
	void Update() override;
	void Render() const override;

	// Getters and Setters
	float GetWidth() const { return float(m_spTexture->GetTextureWidth() / m_Rows); }
	float GetHeight() const { return float(m_spTexture->GetTextureHeight() / m_Cols); }
	
	void SetOffset(b2Vec2 offset) { m_LocalTransform.SetPosition(offset); }
	void SetOffset(float x, float y) { SetOffset(b2Vec2(x, y)); }
	void SetSize(b2Vec2 size) { m_LocalTransform.SetSize(size); }
	void SetSize(float x, float y) { SetSize(b2Vec2(x, y)); }

	void SetFlipped(bool value) { m_Flipped = value; }
	void Flip() { m_Flipped = !m_Flipped; }

private:

	// Functions
	bool EndOfSeq() const { return m_CurrentFrame == (m_Frames - 1); }

	// Variables
	std::shared_ptr<dae::Texture2D> m_spTexture;
	LocalTransform m_LocalTransform;

	float m_FrameTime;
	float m_AccumulatedTime;
	int m_CurrentFrame;
	int m_Frames;
	int m_Cols;
	int m_Rows;
	bool m_Loop;
	bool m_UsesCustomSize;
	bool m_Animated;
	bool m_Flipped;

};

