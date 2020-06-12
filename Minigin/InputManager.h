#pragma once
#include "Singleton.h"
#include "Button.h"
#include "MiniginPCH.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:

		// Functions
		~InputManager();
		bool InputActionPressed(const std::string& inputAction);
		bool ProcessInput();
		void CreateInputAction(const std::string& actionName, Command* pCommand, PhysicalButton physicalButton) { m_InputActions.push_back(new InputAction(actionName, pCommand, physicalButton)); }

	private:

		// Private functions
		InputAction* GetInputAction(const std::string& inputAction);
		bool IsPressed(PhysicalButton button) const;
		void HandleInput() const;

		// Variables
		std::vector<InputAction*> m_InputActions;
		XINPUT_STATE m_CurrentState;
	
	};
}

// Student 'Benjamin Decoene' helped me with the structure of this workflow