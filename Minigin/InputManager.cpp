#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>

dae::InputManager::~InputManager()
{
	// Deleting our buttons
	for (InputAction* pButton : m_InputActions) 
	{
		delete pButton;
		pButton = nullptr;
	}
}

bool dae::InputManager::InputActionPressed(const std::string& inputAction)
{
	// Check if we have pressed this action's button
	return IsPressed(GetInputAction(inputAction)->GetPhysicalButton());
}

InputAction* dae::InputManager::GetInputAction(const std::string& inputAction)
{
	// Return the corresponding input action
	for (InputAction* pInputAction : m_InputActions) 
	{
		if (pInputAction->GetActionName() == inputAction) return pInputAction;
	}

	// No actions found with the given name
	return nullptr;
}

bool dae::InputManager::ProcessInput()
{
	// Check if player wants to exit
	SDL_Event Events;
	SDL_PollEvent(&Events);
	if (Events.type == SDL_QUIT) return false;

	// Controller input via XInput
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(1, &m_CurrentState);

	// Check our Buttons for the controller
	HandleInput();

	return true;
}

bool dae::InputManager::IsPressed(PhysicalButton button) const
{
	// Getting our pressed buttons
	SDL_PumpEvents();
	const UINT8* state{ SDL_GetKeyboardState(NULL) };

	// Checking if our button is pressed
	switch (button)
	{
		// Controller input
	case PhysicalButton::ControllerButtonA:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case PhysicalButton::ControllerButtonB:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case PhysicalButton::ControllerButtonX:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case PhysicalButton::ControllerButtonY:
		return m_CurrentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;

		// Keyboard input
	case PhysicalButton::ButtonA:
		return state[SDL_SCANCODE_A];
	case PhysicalButton::ButtonZ:
		return state[SDL_SCANCODE_Z];
	case PhysicalButton::ButtonE:
		return state[SDL_SCANCODE_E];
	case PhysicalButton::ButtonR:
		return state[SDL_SCANCODE_R];
	case PhysicalButton::ButtonQ:
		return state[SDL_SCANCODE_Q];
	case PhysicalButton::ButtonS:
		return state[SDL_SCANCODE_S];
	case PhysicalButton::ButtonD:
		return state[SDL_SCANCODE_D];
	case PhysicalButton::ButtonF:
		return state[SDL_SCANCODE_F];
	}

	// Our button is not pressed
	return false;
}

void dae::InputManager::HandleInput() const
{
	// Check if our button is pressed
	for (InputAction* pButton : m_InputActions) 
	{
		// See if we used the key
		bool buttonDown{ IsPressed(pButton->GetPhysicalButton()) };
		//if (buttonDown) std::cout << "Down\n";
		pButton->ButtonDown(buttonDown);
	}

	// Executing our commands
	for (InputAction* pButton : m_InputActions)
	{
		// Check in what state we are and execute accordingly
		pButton->ExecuteCommand();
	}
}

