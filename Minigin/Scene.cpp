#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Time.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

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

	// Update our physics world
	m_pPhysicsWorld->Step(Time::GetInstance().GetDeltaTime(), 6, 2);
}

void Scene::Render() const
{
	// Render all our objects in our scene
	for (const auto& object : m_Objects) object->Render();
}
