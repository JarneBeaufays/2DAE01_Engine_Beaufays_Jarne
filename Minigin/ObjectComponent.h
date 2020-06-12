#pragma once
#include "GameObject.h"
#include <string>

class ObjectComponent
{
public:

	// Functions
	ObjectComponent(std::string name = "", dae::GameObject* pParent = nullptr) : m_Name{ name }, m_pParent{ pParent } { };
	void virtual Update() = 0;
	void virtual Render() const { };

	// Getters and Setters
	const std::string& GetName() const { return m_Name; };

protected:

	// Datamembers
	dae::GameObject* m_pParent;
	std::string m_Name;
};

