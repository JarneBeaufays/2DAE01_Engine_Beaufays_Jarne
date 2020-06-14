#include "Game.h"
#include "Wall.h"
#include "Ground.h"
#include "BackgroundImage.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "EnemyManager.h"
#include "Box.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "HighScore.h"
#include "GameInfo.h"
#include "SceneTimer.h"
#include <memory>

bool Game::Initialize()
{
	// Creating our engine
	m_pEngine = new dae::Minigin{};
	m_pEngine->Initialize(800, 720);

	// Making some variables
	bool valid{ true };

	// Text
	std::shared_ptr<HighScore> spText{ std::make_shared<HighScore>() };
	GameInfo::GetInstance().AddGameObject(spText);

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
	CreateStartScene();
	CreateSceneOne();
	CreateSceneTwo();
	CreateSceneThree();
	CreateWinScene();
	CreateEndScene();

	return true;
}

void Game::CleanPrivatePointers()
{
	// Deleting our private pointers in this class
	delete m_pEngine;
	m_pEngine = nullptr;
}

void Game::CreateStartScene()
{
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - BeginScreen");

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("StartScreen.png") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);

	std::shared_ptr<SceneTimer> timer{ std::make_shared<SceneTimer>() };
	scene->Add(timer);
}

void Game::CreateWinScene()
{
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - Win");
	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("WinScreen.png") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);
}

void Game::CreateSceneOne()
{
	// --- https://nl.trend-top.com/8-games-like-bubble-bobble-on-steam ---
	// Creating the first scene
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - Level 1");

	std::shared_ptr<CollisionManager> manager{ std::make_shared<CollisionManager>() };
	scene->Add(manager);

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("Background.jpg") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);

	// -- Making the wall objects
	// Bottom
	std::shared_ptr<Ground> ground{ std::make_shared<Ground>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 250.f, 30.f }) };
	std::shared_ptr<Wall> wall{  };
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 550.f, 0.f }, b2Vec2{ 250.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 350.f, 0.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	// Side
	wall = std::make_shared<Wall>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 770.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene->Add(wall);
	
	// Top
	wall = std::make_shared<Wall>(scene, b2Vec2{ 0.f, 640.f }, b2Vec2{ 250.f, 30.f });
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 550.f, 640.f }, b2Vec2{ 250.f, 30.f });
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 350.f, 640.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(wall);

	// In Level
	ground = std::make_shared<Ground>(scene, b2Vec2{ 30.f, 130.f }, b2Vec2{ 170.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 250.f, 130.f }, b2Vec2{ 80.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 600.f, 130.f }, b2Vec2{ 170.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 470.f, 130.f }, b2Vec2{ 80.f, 30.f });
	scene->Add(ground);

	// Left E
	ground = std::make_shared<Ground>(scene, b2Vec2{ 150.f, 500.f }, b2Vec2{ 180.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 150.f, 375.f }, b2Vec2{ 200.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 150.f, 250.f }, b2Vec2{ 220.f, 30.f });
	scene->Add(ground);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 150.f, 250.f }, b2Vec2{ 30.f, 280.f }, true);
	scene->Add(wall);

	// Right E
	ground = std::make_shared<Ground>(scene, b2Vec2{ 470.f, 500.f }, b2Vec2{ 180.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 450.f, 375.f }, b2Vec2{ 200.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 430.f, 250.f }, b2Vec2{ 220.f, 30.f });
	scene->Add(ground);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 620.f, 250.f }, b2Vec2{ 30.f, 280.f }, true);
	scene->Add(wall);

	// Player
	std::shared_ptr<Player> spPlayer{ std::make_shared<Player>(scene, b2Vec2{ 100.f, 350.f }, b2Vec2{ 40.f, 60.f }) };
	scene->Add(spPlayer);

	// Enemy Manager
	std::shared_ptr<EnemyManager> spEnemyManager{ std::make_shared<EnemyManager>(scene, 0) };
	scene->Add(spEnemyManager);

	// Enemy
	std::shared_ptr<Enemy> spEnemy{ std::make_shared<Enemy>(scene, b2Vec2{ 270.f, 350.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager) };
	scene->Add(spEnemy);
	spEnemy = std::make_shared<Enemy>(scene, b2Vec2{ 490.f, 350.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager);
	scene->Add(spEnemy);

	// --
}

void Game::CreateSceneTwo()
{
	// Level: http://belgiquesets.servemp3.com:8000/bublbobl/bublbobl/www.adamdawes.com/retrogaming/bbguide/images/round02.gif
	// Creating the Second scene
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - Level 2");

	std::shared_ptr<CollisionManager> manager{ std::make_shared<CollisionManager>() };
	scene->Add(manager);

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("Background2.jpg") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);

	std::shared_ptr<Ground> ground{ std::make_shared<Ground>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 800.f, 30.f }) };
	scene->Add(ground);

	std::shared_ptr<Wall> wall{ std::make_shared<Wall>(scene, b2Vec2{ 0.f, 640.f }, b2Vec2{ 800.f, 30.f }) };
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 30.f, 640.f });
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 770.f, 0.f }, b2Vec2{ 30.f, 640.f });
	scene->Add(wall);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 100.f, 100.f }, b2Vec2{ 150.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 350.f, 100.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 550.f, 100.f }, b2Vec2{ 150.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 150.f, 225.f }, b2Vec2{ 500.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 200.f, 350.f }, b2Vec2{ 150.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 450.f, 350.f }, b2Vec2{ 150.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 300.f, 475.f }, b2Vec2{ 200.f, 30.f });
	scene->Add(ground);

	// Player
	std::shared_ptr<Player> spPlayer{ std::make_shared<Player>(scene, b2Vec2{ 30.f, 30.f }, b2Vec2{ 40.f, 60.f }) };
	scene->Add(spPlayer);

	// Enemy Manager
	std::shared_ptr<EnemyManager> spEnemyManager{ std::make_shared<EnemyManager>(scene, 0) };
	scene->Add(spEnemyManager);

	// Enemy
	std::shared_ptr<Enemy> spEnemy{ std::make_shared<Enemy>(scene, b2Vec2{ 380.f, 300.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager) };
	scene->Add(spEnemy);
	spEnemy = std::make_shared<Enemy>(scene, b2Vec2{ 150.f, 150.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager);
	scene->Add(spEnemy);
	spEnemy = std::make_shared<Enemy>(scene, b2Vec2{ 610.f, 150.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager);
	scene->Add(spEnemy);
}

void Game::CreateSceneThree()
{
	// Level: https://lh3.googleusercontent.com/proxy/SOkFLHWP-vyuJuKaS7PJLjZGZLgTf_fOxAjnxya1CIJMbUGagtCuYUaBNEp8ipAlDSBAeHYdfqER2HZIbX7DmS9eW-jvSjegu7XJ9zbdG5PebZOgqrHDa5n6sDKdQv57PDQoiuzTU63TdO-2whLOA3rHpyt_6OWlEjd4BYrtW67KZtEqbAF-qA
	// Creating the third scene
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - Level 3");

	std::shared_ptr<CollisionManager> manager{ std::make_shared<CollisionManager>() };
	scene->Add(manager);

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("Background3.jpg") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);

	std::shared_ptr<Ground> ground{ std::make_shared<Ground>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 800.f, 30.f }) };
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 30.f, 120.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 30.f, 280.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 30.f, 440.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 670.f, 120.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 670.f, 280.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 670.f, 440.f }, b2Vec2{ 100.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 250.f, 120.f }, b2Vec2{ 300.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 250.f, 280.f }, b2Vec2{ 300.f, 30.f });
	scene->Add(ground);

	ground = std::make_shared<Ground>(scene, b2Vec2{ 250.f, 440.f }, b2Vec2{ 300.f, 30.f });
	scene->Add(ground);

	std::shared_ptr<Wall> wall{ std::make_shared<Wall>(scene, b2Vec2{ 0.f, 640.f }, b2Vec2{ 800.f, 30.f }) };
	scene->Add(wall);
	
	wall = std::make_shared<Wall>(scene, b2Vec2{ 0.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene->Add(wall);

	wall = std::make_shared<Wall>(scene, b2Vec2{ 770.f, 0.f }, b2Vec2{ 30.f, 670.f });
	scene->Add(wall);

	// Player
	std::shared_ptr<Player> spPlayer{ std::make_shared<Player>(scene, b2Vec2{ 30.f, 30.f }, b2Vec2{ 40.f, 60.f }) };
	scene->Add(spPlayer);

	// Enemy Manager
	std::shared_ptr<EnemyManager> spEnemyManager{ std::make_shared<EnemyManager>(scene, 0) };
	scene->Add(spEnemyManager);

	// Enemy
	std::shared_ptr<Enemy> spEnemy{ std::make_shared<Enemy>(scene, b2Vec2{ 380.f, 300.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager) };
	scene->Add(spEnemy);
	spEnemy = std::make_shared<Enemy>(scene, b2Vec2{ 50.f, 150.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager);
	scene->Add(spEnemy);
	spEnemy = std::make_shared<Enemy>(scene, b2Vec2{ 720.f, 150.f }, b2Vec2{ 40.f, 60.f }, EnemyType::ZenChan, spEnemyManager);
	scene->Add(spEnemy);
}

void Game::CreateEndScene()
{
	std::shared_ptr<dae::Scene> scene = dae::SceneManager::GetInstance().CreateScene("Bubble Bobble - EndScreen");

	std::shared_ptr<BackgroundImage> image{ std::make_shared<BackgroundImage>("GameOver.png") };
	image->GetTransform().SetPosition(0.f, 0.f);
	scene->Add(image);
}
