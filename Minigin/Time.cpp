#include "MiniginPCH.h"
#include "Time.h"

void Time::Update()
{
	// Update the time if the game isn't paused
	if (!m_Paused) 
	{
		// Getting the end time for time calculation
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> deltaTime = endTime - m_PreviousFrameTime;

		// Setting the delta time
		m_DeltaTime = (float)deltaTime.count();
		m_TotalTimePlaying += m_DeltaTime;

		// Setting the currentTime used for the next frame
		m_PreviousFrameTime = std::chrono::high_resolution_clock::now();
	}

	// Adding the time to our total time
	m_TotalTimeRunning += m_DeltaTime;

	// Calculating our Frames Per Second
	CalculateFPS();
}

void Time::CalculateFPS()
{
	// Calculating the frames per second
	m_FrameTimeCounter += m_DeltaTime;

	if (m_FrameTimeCounter > 1)
	{
		// Calculating the frames we got in one second
		m_FramesPerSecond = int(1.f / m_DeltaTime);
		m_FrameTimeCounter = 0.f;
	}
}

#pragma region Setters
	void Time::SetPaused(bool value = true)
	{
		// This function allows you to pause the timer
		m_Paused = value;
	}
#pragma endregion

#pragma region Getters
	float Time::GetDeltaTime() const
	{
		// Returns the time between frames
		return m_DeltaTime;
	}
	
	float Time::GetTotalTimeRunning() const
	{
		// Returns the total time this project has been running
		return m_TotalTimeRunning;
	}
	
	float Time::GetTotalTimePlaying() const
	{
		// Returns the total time the game has been playing (keeps pause into account)
		return m_TotalTimePlaying;
	}
	
	int Time::GetFPS() const
	{
		// Returns the frames per second
		return m_FramesPerSecond;
	}
	bool Time::IsPaused() const
	{
		// Returns true if the timer is on pause
		return m_Paused;
	}
#pragma endregion
