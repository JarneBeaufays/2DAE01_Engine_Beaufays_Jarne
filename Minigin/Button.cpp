#include "MiniginPCH.h"
#include "Button.h"

void InputAction::ButtonDown(bool value)
{
	// Check in what state we are and update accordingly
	switch (m_ButtonState)
	{
	case ButtonState::idle:
		if (value) m_ButtonState = ButtonState::onPress;
		break;

	case ButtonState::onPress:
		value ? m_ButtonState = ButtonState::down : m_ButtonState = ButtonState::onRelease;
		break;

	case ButtonState::down:
		if (!value) m_ButtonState = ButtonState::onRelease;
		break;

	case ButtonState::onRelease:
		value ? m_ButtonState = ButtonState::onPress : m_ButtonState = ButtonState::idle;
		break;
	}
}

void InputAction::ExecuteCommand()
{
	// Executing the correct action
	switch (m_ButtonState)
	{
	case ButtonState::onPress:
		m_pCommand->OnPress();
		break;
	case ButtonState::down:
		m_pCommand->Down();
		break;
	case ButtonState::onRelease:
		m_pCommand->OnRelease();
		break;
	}
}
