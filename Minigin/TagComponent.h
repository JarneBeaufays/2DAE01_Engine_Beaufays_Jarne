#pragma once
#include "ObjectComponent.h"

class TagComponent : public ObjectComponent
{
public:
	
	// Functions
	TagComponent(dae::GameObject* pParent, const std::string& tag);
	bool CompareTag(const std::string& otherTag) { return (otherTag == m_Tag); }
	void Update() { }

	// Getters and Setters
	std::string GetTag() const { return m_Tag; }
	void SetTag(const std::string& tag) { m_Tag = tag; }

private:

	// Private variables
	std::string m_Tag;

};