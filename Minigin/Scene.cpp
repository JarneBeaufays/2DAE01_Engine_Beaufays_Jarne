#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "Time.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

CollisionManager* dae::Scene::GetCollisionManager() const
{
	for (auto ob : m_Objects) 
	{
		dae::GameObject* obj{ dynamic_cast<dae::GameObject*>(ob.get()) };
		if (obj) 
		{
			if (obj->GetName() == "CollisionManager")
				return dynamic_cast<CollisionManager*>(obj);
		}
	}
	return nullptr;
}

Scene::Scene(const std::string& name)
	: m_Name{ name }
{
	// Creating our physics world
	b2Vec2 gravity{ 0.0f, -9.81f };
	m_pPhysicsWorld = new b2World(gravity);

	// Tell Box2D that we want to listen for
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<SceneObject> object)
{
	// Adding an object to our scene
	m_Objects.push_back(object);
}

void Scene::Update()
{
	// Update each object in our scene
	for(auto& object : m_Objects) object->Update();

	// Delete objects that must me deleted
	for (int i{}; i < m_Objects.size(); i++)
	{
		if (m_Objects[i]->GetDelete())
		{
			// Delete object
			// Remove the collision data
			GameObject* pTemp{ dynamic_cast<GameObject*>(m_Objects[i].get()) };
			dae::SceneManager::GetInstance().GetCurrentScene()->GetCollisionManager()->DeleteBox(pTemp->GetComponent<BoxTrigger>());

			// Remove the object from the list -> Smart Pointers
			m_Objects[i] = m_Objects.back();
			m_Objects.pop_back();
			return;
		}
	}

	// Update our physics world
	m_pPhysicsWorld->Step(Time::GetInstance().GetDeltaTime(), 6, 2);
}

void Scene::Render() const
{
	// Render all our objects in our scene
	for (const auto& object : m_Objects) object->Render();
}
