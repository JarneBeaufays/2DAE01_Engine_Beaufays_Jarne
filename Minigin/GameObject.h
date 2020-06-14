#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Scene.h"
#include <vector>
#include <string>

class ObjectComponent;

namespace dae
{
	class GameObject : public SceneObject
	{
	public:
		
		// Rule of 5
		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;

		// Main functions
		void Update() override;
		void Render() const override;
		
		// Functions
		template<class T>
		T* GetComponent() const;
		ObjectComponent* GetComponent(const std::string& componentName) const;
		std::vector<ObjectComponent*> GetComponents(const std::string& componentName) const;
		void SetName(const std::string & name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }
		void AddComponent(ObjectComponent* component);
		virtual void OnTriggerEnter() { };
		virtual void OnTriggerCollision() { };
		virtual void OnTriggerExit() { };

		// Getters and setters
		const Transform& GetTransform() const { return m_Transform; }
		Transform& GetTransform() { return m_Transform; }

	private:

		// Private variables
		Transform m_Transform;
		std::vector<ObjectComponent*> m_pComponents;
		std::string m_Name{ "" };
	
	};

	template<class T>
	inline T* GameObject::GetComponent() const
	{
		// Loop over all our components
		for (ObjectComponent* pComponent : m_pComponents) 
		{
			T* temp{ dynamic_cast<T*>(pComponent) };
			if (temp) return temp;
		}

		// This component hasn't been found
		return nullptr;
	}
}
