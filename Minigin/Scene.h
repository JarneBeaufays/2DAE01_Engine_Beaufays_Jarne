#pragma once
#include "SceneManager.h"
#include "TextObject.h"
#include "ResourceManager.h"
#include "Box2D.h"

class CollisionManager;
class Player;

namespace dae
{
	class SceneObject;
	class Scene
	{
		friend std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name);
	public:

		// Functions
		void Update();
		void Render() const;
		void Add(std::shared_ptr<SceneObject> object);

		// Rule of 5
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		//Getters and Setters
		b2Body* CreateBody(b2BodyDef* bodyDef) { return m_pPhysicsWorld->CreateBody(bodyDef); };
		b2World* GetWorld() const { return m_pPhysicsWorld; }
		const std::string& GetName() const { return m_Name; }
		const std::vector <std::shared_ptr<SceneObject>> GetObjects() const { return m_Objects; }
		CollisionManager* GetCollisionManager() const;
		Player* GetPlayer() const;
		const int GetPPM() const { return m_PPM; }

	private: 

		// Functions
		explicit Scene(const std::string& name);

		// Variables
		b2World* m_pPhysicsWorld;
		std::string m_Name;
		std::vector <std::shared_ptr<SceneObject>> m_Objects{};
		static unsigned int m_IdCounter;
		int m_PPM = 32;
	};

}
