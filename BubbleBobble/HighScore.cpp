#include "HighScore.h"
#include "TagComponent.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "..\Minigin\Time.h"

HighScore::HighScore()
{
	// Setting variables
	GetTransform().SetPosition(10.f, 708.f);

	// Adding components	
	TagComponent* pTagComponent{ new TagComponent(this, "HighScoreCounter") };
	AddComponent(pTagComponent);

	const int fontSize{ 30 };
	TextComponent* pTimer{ new TextComponent(this, "0", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize)) };
	pTimer->SetOffset(410.f, 0.f);
	AddComponent(pTimer);

	TextComponent* pText{ new TextComponent(this, "Time: ", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize)) };
	pText->SetOffset(330.f, 0.f);
	AddComponent(pText);

	TextComponent* pScore{ new TextComponent(this, "100000", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", fontSize)) };
	AddComponent(pScore);
}

void HighScore::Update()
{
	// Update children
	dae::GameObject::Update();

	// Getting our components
	std::vector<TextComponent*> textComponents;
	for (ObjectComponent* pObj : GetComponents("TextComponent")) textComponents.push_back(dynamic_cast<TextComponent*>(pObj));

	// Updating time
	m_CurrentTimer += Time::GetInstance().GetDeltaTime();
	if (m_CurrentTimer >= 1)
	{
		m_CurrentTimer = 0;
		m_SecondsInGame++;
		textComponents[0]->SetText(std::to_string(m_SecondsInGame));
	}

	// Check if we have to update our score
	textComponents[2]->SetText(std::to_string((int)m_Points));
}
