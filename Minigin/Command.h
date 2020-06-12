#pragma once
#include <Functional>

class Command
{
public:

	// Functions
	Command(std::function<void()> onPress, std::function<void()> onRelease, std::function<void()> down) : m_OnPressAction{ onPress }, m_OnReleaseAction{ onRelease }, m_DownAction{ down } { }
	void OnPress() { m_OnPressAction(); }
	void OnRelease() { m_OnReleaseAction(); }
	void Down() { m_DownAction(); }

private:

	// Privates
	std::function<void()> m_OnPressAction;
	std::function<void()> m_OnReleaseAction;
	std::function<void()> m_DownAction;

};