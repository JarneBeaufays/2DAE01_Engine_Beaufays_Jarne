#pragma once
#include "Singleton.h"
#include <chrono>

class Time final : public dae::Singleton<Time>
{
public:
	// -- Functions Public
	void Update();

	// -- Setters
	void SetPaused(bool value);

	// -- Getters
	float GetDeltaTime() const;
	float GetTotalTimeRunning() const;
	float GetTotalTimePlaying() const;
	int GetFPS() const;
	bool IsPaused() const;

private:

	// -- Functions Private
	void CalculateFPS();

	// -- Variables
	std::chrono::steady_clock::time_point m_PreviousFrameTime = std::chrono::high_resolution_clock::now(); 	// Total time running = since start, total time playing keeps the pausing of the game in mind

	float m_TotalTimeRunning = 0.f;
	float m_TotalTimePlaying = 0.f;
	float m_DeltaTime = 0.f;
	float m_FrameTimeCounter = 0.f;

	int m_FramesPerSecond = 0;

	bool m_Paused = false;

};