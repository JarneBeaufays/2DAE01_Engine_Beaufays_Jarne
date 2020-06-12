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
		bool ProcessInput();
		bool IsPressed(PhysicalButton button) const;
		void CreateButton(Command* pCommand, PhysicalButton physicalButton) { m_Buttons.push_back(new Button(pCommand, physicalButton)); }

	private:

		// Private functions
		void HandleControllerInput() const;

		// Variables
		std::vector<Button*> m_Buttons;
		XINPUT_STATE m_CurrentState;
	
	};
}

// Student 'Benjamin Decoene' helped me with the structure of this workflow