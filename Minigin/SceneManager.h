#pragma once
#include "Singleton.h"
#include "SceneObject.h"
#include <string>
#include <memory>
#include <vector>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:

		// Functions
		std::shared_ptr<dae::Scene> CreateScene(const std::string& name);
		void Destroy();
		void Update();
		void Render();
		bool NextScene();
		void InstantiateObject(std::shared_ptr<dae::SceneObject> spObject);

		// Getters and Setters
		std::shared_ptr<Scene> GetCurrentScene() { return m_spScenes[m_ActiveSceneID]; }
		bool SetActiveScene(const std::string& name);
		bool SetActiveScene(int id);

	private:

		// Functions
		SceneManager() = default;

		// Variables
		friend class Singleton<SceneManager>;
		std::vector<std::shared_ptr<Scene>> m_spScenes;
		int m_ActiveSceneID{ 0 };

	};
}
