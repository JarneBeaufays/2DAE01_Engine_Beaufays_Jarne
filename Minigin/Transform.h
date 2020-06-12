#pragma once
#include "Box2D.h"

namespace dae
{
	class GameObject;

	class Transform final
	{
	public:

		const b2Vec2& GetPosition() const { return m_Position; }
		const b2Vec2& GetSize() const { return m_Size; }
		float GetAngle() const { return m_Angle; }

		void SetPosition(float x, float y);
		void SetPosition(const b2Vec2& position);
		void SetSize(float x, float y);
		void SetSize(const b2Vec2& size);
		void SetAngle(float angle) { m_Angle = angle; }

	private:
	
		b2Vec2 m_Position;
		b2Vec2 m_Size;
		float m_Angle;

	};
}
