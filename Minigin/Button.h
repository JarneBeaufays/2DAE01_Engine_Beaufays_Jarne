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
	ButtonE, ButtonR,

	ButtonQ, ButtonS,
	ButtonD, ButtonF
};

class InputAction
{
public:

	// Functions
	InputAction(const std::string& actionName, Command* pCommand, PhysicalButton physicalButton) : m_ActionName{ actionName }, m_pCommand { pCommand }, m_PhysicalButton{ physicalButton }, m_ButtonState{ ButtonState::idle } { };
	~InputAction() { delete m_pCommand; m_pCommand = nullptr; };
	void ButtonDown(bool value);
	void ExecuteCommand();

	// Getters and Setters
	const std::string& GetActionName() const { return m_ActionName; }
	Command* GetCommand() const { return m_pCommand; }
	PhysicalButton GetPhysicalButton() { return m_PhysicalButton; }
	ButtonState GetState() { return m_ButtonState; }
	void SetState(ButtonState state) { m_ButtonState = state; }

private:

	// Variables
	std::string m_ActionName;
	Command* m_pCommand;
	ButtonState m_ButtonState;
	PhysicalButton m_PhysicalButton;

};
