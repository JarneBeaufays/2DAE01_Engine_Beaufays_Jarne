#pragma once
#include "ObjectComponent.h"
#include "Transform.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Box2D.h"
#include "RigidBody2D.h"
#include <memory>

class ImageComponent : public ObjectComponent
{
public:

	// Functions
	ImageComponent(dae::GameObject* pParent, std::string filePath, bool drawAroundCenter = false);
	void Update() override { };
	void Render() const override;
	void Resize(const b2Vec2& size);
	void Resize(float width, float height);

	// Setters and Getters
	void SetOffset(b2Vec2 offset) { m_Offset = offset; }
	void SetOffset(float x, float y) { m_Offset.x = x; m_Offset.y = y; }
	void SetKeepOriginalSize(bool value) { m_KeepOriginalSize = value; }
	void SetRigidBody2D(RigidBody2D* rb) { m_pRigidBody = rb; }

private:

	// Private variables
	RigidBody2D* m_pRigidBody;
	std::shared_ptr<dae::Texture2D> m_spTexture;
	
	b2Vec2 m_Offset;
	b2Vec2 m_Size;
	bool m_KeepOriginalSize;
	bool m_UsePosAsCenter;

};

