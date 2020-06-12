#pragma once
#include "Command.h"

enum class ButtonState 
{
	idle = 0,
	onPress = 1,
	down = 2,
	onRelease = 3
};

enum class PhysicalButton
{
	// Controller buttons
	ControllerButtonA,
	ControllerButtonB,
	ControllerButtonX,
	ControllerButtonY,

	// Keyboard buttons
	ButtonA, ButtonZ,
	ButtonE, ButtonR
};

class Button
{
public:

	// Functions
	Button(Command* pCommand, PhysicalButton physicalButton) : m_pCommand{ pCommand }, m_PhysicalButton{ physicalButton }, m_ButtonState{ ButtonState::idle } { };
	~Button() { delete m_pCommand; m_pCommand = nullptr; };
	void ButtonDown(bool value);
	void ExecuteCommand();

	// Getters and Setters
	Command* GetCommand() const { return m_pCommand; }
	PhysicalButton GetPhysicalButton() { return m_PhysicalButton; }
	ButtonState GetState() { return m_ButtonState; }
	void SetState(ButtonState state) { m_ButtonState = state; }

private:

	// Variables
	Command* m_pCommand;
	ButtonState m_ButtonState;
	PhysicalButton m_PhysicalButton;

};
