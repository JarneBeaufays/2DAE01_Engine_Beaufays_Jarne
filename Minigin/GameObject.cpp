#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "ObjectComponent.h"
#include "Renderer.h"

dae::GameObject::~GameObject() 
{
	// Deleting all the game objects
	for (ObjectComponent* pObject : m_pComponents) 
	{
		delete pObject;
		pObject = nullptr;
	}
	m_pComponents.clear();
};

void dae::GameObject::Update()
{
	// First run collision functions
	OnTriggerEnter();
	OnTriggerCollision();
	OnTriggerExit();

	// Update child objects
	for (ObjectComponent* pComponent : m_pComponents) pComponent->Update();
}

void dae::GameObject::Render() const
{
	// Render child objects
	for (ObjectComponent* pComponent : m_pComponents) pComponent->Render();
}

void dae::GameObject::AddComponent(ObjectComponent* component)
{
	// Add component to game object
	m_pComponents.push_back(component);
}

ObjectComponent* dae::GameObject::GetComponent(const std::string& componentName) const
{
	// Check if we find the component
	for (ObjectComponent* pComponent : m_pComponents) 
	{
		if (pComponent->GetName() == componentName) return pComponent;
	}

	// Return nullptr, bcs no component found
	return nullptr;
}

std::vector<ObjectComponent*> dae::GameObject::GetComponents(const std::string& componentName) const
{
	// Create the vector we'll store our components in
	std::vector<ObjectComponent*> components;

	// Add all the components
	for (ObjectComponent* pComponent : m_pComponents)
	{
		if (pComponent->GetName() == componentName) components.push_back(pComponent);
	}

	// Return our components
	return components;
}
