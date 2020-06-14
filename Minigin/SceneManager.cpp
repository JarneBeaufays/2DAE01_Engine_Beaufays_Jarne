#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "..\BubbleBobble\Player.h"

std::shared_ptr<dae::Scene> dae::SceneManager::CreateScene(const std::string& name)
{
	// Creating the scene
	const auto scene = std::shared_ptr<Scene>(new Scene(name));

	// Pushing back our scene
	m_spScenes.push_back(scene);
	return scene;
}

void dae::SceneManager::Destroy()
{
	m_spScenes.clear();
}

void dae::SceneManager::Update()
{
	// Update the current scene
	m_spScenes[m_ActiveSceneID]->Update();
}

void dae::SceneManager::Render()
{
	// Render the current scene
	m_spScenes[m_ActiveSceneID]->Render();
}

bool dae::SceneManager::NextScene()
{
	// Check if we don't go over out maximum scenes
	if (unsigned int(m_ActiveSceneID + 1) < m_spScenes.size())
	{
		m_ActiveSceneID++;
		return true;
	}

	// Avoid vector error ID >= amount of scenes
	return false;
}

void dae::SceneManager::InstantiateObject(std::shared_ptr<dae::SceneObject> spObject)
{
	GetCurrentScene()->Add(spObject);
}

bool dae::SceneManager::SetActiveScene(const std::string& name)
{
	// Go over all the scenes and compare the names
	for (unsigned int i{}; i < m_spScenes.size(); i++) 
	{
		if (m_spScenes[i]->GetName() == name)
		{
			m_ActiveSceneID = i;
			return true;
		}
	}

	// No scene found with this name
	return false;
}

bool dae::SceneManager::SetActiveScene(int id)
{
	// Check if the given id fits in our vector
	if (unsigned int(id) < m_spScenes.size()) 
	{
		m_ActiveSceneID = id;
		return true;
	}

	// Avoid vector error ID >= amount of scenes
	return false;
}
