#include "MiniginPCH.h"
#include "TagComponent.h"

TagComponent::TagComponent(dae::GameObject* pParent, const std::string& tag)
	: m_Tag{ tag }
	, ObjectComponent{ "TagComponent", pParent }
{
}
