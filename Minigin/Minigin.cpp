#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "CollisionManager.h"
#include <SDL.h>
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"

using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize(unsigned int width, unsigned int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window, width, height);

	// Tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	AudioManager::GetInstance().Init("../Data/");
}

void dae::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	auto& collision = CollisionManager::GetInstance();
	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	while (doContinue)
	{
		const auto start = std::chrono::high_resolution_clock::now();

		// Resetting our colors
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 0, 0, 0, 255);

		doContinue = input.ProcessInput();

		// Updating our singletons

		Time::GetInstance().Update();
		collision.Update();
		sceneManager.Update();
		renderer.Render();

		const auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;

		auto sleepTime = duration_cast<duration<float>>(start + milliseconds(MsPerFrame) - high_resolution_clock::now());
		this_thread::sleep_for(sleepTime);
	}

	Cleanup();
}
