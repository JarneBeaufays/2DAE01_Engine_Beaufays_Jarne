#include "Game.h"
#include "Wall.h"
#include "BackgroundImage.h"
#include "Box.h"
#include "GameObject.h"
#include "Player.h"
#include "HighScore.h"
#include <memory>

bool Game::Initialize()
{
	// Creating our engine
	m_pEngine = new dae::Minigin{};
	m_pEngine->Initialize(800, 720);

	// Making some variables
	bool valid{ true };

	// Creating our scenes
	valid = CreateScenes() ? valid : false;

	// Returning if we are ready to go
	return valid;
}

void Game::StartGame()
{
	// Starting our gameloop!
	m_pEngine->Run();
}

void Game::Clean()
{
	// Getting rid of memory
	CleanPrivatePointers();
}

bool Game::CreateScenes()
{
	// Creating all the scenes for our game
	CreateSceneOne();

	return true;
}

void Game::CleanPrivatePointers()
{
	// Deleting our private pointers in this class
	delete m_pEngine;
	m_pEngine = nullptr;
}

void Game::CreateSceneOne()
{
	// --- https://nl.trend-top.com/8-games-like-bubble-bobble-on-steam ---
	// Creating the first scene
	auto& scene{ dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - Level 1") };

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("Background.jpg") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene.Add(image);

	// -- Making the wall objects
	// Bottom
	std::shared_ptr<Wall> wall{ std::make_shared<Wall>(&scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 250.f, 30.f }) };
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 550.f, 0.f }, b2Vec2{ 250.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 350.f, 0.f }, b2Vec2{ 100.f, 30.f });
	scene.Add(wall);

	// Side
	wall = std::make_shared<Wall>(&scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 770.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene.Add(wall);
	
	// Top
	wall = std::make_shared<Wall>(&scene, b2Vec2{ 0.f, 640.f }, b2Vec2{ 250.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 550.f, 640.f }, b2Vec2{ 250.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 350.f, 640.f }, b2Vec2{ 100.f, 30.f });
	scene.Add(wall);

	// In Level
	wall = std::make_shared<Wall>(&scene, b2Vec2{ 30.f, 130.f }, b2Vec2{ 170.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 250.f, 130.f }, b2Vec2{ 80.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 600.f, 130.f }, b2Vec2{ 170.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 470.f, 130.f }, b2Vec2{ 80.f, 30.f });
	scene.Add(wall);

	// Left E
	wall = std::make_shared<Wall>(&scene, b2Vec2{ 150.f, 500.f }, b2Vec2{ 180.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 150.f, 375.f }, b2Vec2{ 200.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 150.f, 250.f }, b2Vec2{ 220.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 150.f, 250.f }, b2Vec2{ 30.f, 280.f });
	scene.Add(wall);

	// Right E
	wall = std::make_shared<Wall>(&scene, b2Vec2{ 470.f, 500.f }, b2Vec2{ 180.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 450.f, 375.f }, b2Vec2{ 200.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 430.f, 250.f }, b2Vec2{ 220.f, 30.f });
	scene.Add(wall);

	wall = std::make_shared<Wall>(&scene, b2Vec2{ 620.f, 250.f }, b2Vec2{ 30.f, 280.f });
	scene.Add(wall);

	// Player
	std::shared_ptr<Player> spPlayer{ std::make_shared<Player>(&scene, b2Vec2{ 100.f, 350.f }, b2Vec2{ 40.f, 60.f }) };
	scene.Add(spPlayer);

	// Text
	std::shared_ptr<HighScore> spText{ std::make_shared<HighScore>() };
	scene.Add(spText);

	// --

	// Making some boxes for testing purposes
	const int amountOfBoxes{ 2 };

	for (int i{}; i < amountOfBoxes; i++)
	{
		std::shared_ptr<Box> box{ std::make_shared<Box>() };
		box->Initialize(&scene, b2Vec2{ (i+1) * 50.f, 600.0f }, b2Vec2{ float(rand() % 15 + 10), float(rand() % 15 + 10) });
		scene.Add(box);
	}
}
