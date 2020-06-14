#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "HighScore.h"

class GameInfo : public dae::Singleton<GameInfo>
{
public:

	// Functions
	void AddGameObject(std::shared_ptr<dae::GameObject> spObject) { m_Objects.push_back(spObject); }
	void Update() { for (auto pObject : m_Objects) pObject->Update(); }
	void Render() const { for (auto pObject : m_Objects) pObject->Render(); }
	void AddPoints(float score) { for (auto pObject : m_Objects) if (dynamic_cast<HighScore*>(pObject.get())) dynamic_cast<HighScore*>(pObject.get())->AddPoints(score); }

private:

	// Variables
	std::vector<std::shared_ptr<dae::GameObject>> m_Objects;

};

