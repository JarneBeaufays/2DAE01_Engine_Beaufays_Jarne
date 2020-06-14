#include "MiniginPCH.h"
#include "StateMachineComponent.h"

Transition::~Transition()
{
	delete m_pTargetState;
	m_pTargetState = nullptr;

	for (std::function<bool()>& pFunc : m_Conditions) 
	{
		delete &pFunc;
		pFunc = nullptr;
	}
}

bool Transition::Update()
{
	// Here we want to go over all of our conditions
	for (std::function<bool()>& function : m_Conditions)
	{
		// If our condition returns false, we return false too
		if (!function()) return false;
	}

	// Executing our exit actions
	for (std::function<void()>& function : m_ExitActions) function();

	// Returning our state
	return true;
}

State* State::Update()
{
	// Check if this is the first time we enter this state
	if (!m_Entered) 
	{
		// Loop over the entry functions
		for (std::function<void()>& function : m_EntryActions) function();

		// Setting some variables
		m_Entered = true;
	}

	// Here we want to go over all of our transitions and see if we have to follow them
	for (Transition* pTransition : m_Transitions) 
	{
		if (pTransition->Update()) 
		{
			// First change up some vars
			m_Entered = false;

			// Loop over our exit functions
			for (std::function<void()>& function : m_ExitActions) function();

			// We want to make a transition to another state!
			return pTransition->GetTargetState();
		}
	}

	// Loop over our action functions before returning
	for (std::function<void()>& function : m_Actions) function();

	// We want to stay in this state
	return this;
}

StateMachineComponent::~StateMachineComponent()
{
	for (Transition* pTran : m_AnyStateTransitions) 
	{
		delete pTran;
		pTran = nullptr;
	}
	m_AnyStateTransitions.clear();
}

void StateMachineComponent::Update()
{
	// First go over our any state transitions
	for (Transition* pTrans : m_AnyStateTransitions) 
	{
		if (pTrans->Update()) m_pCurrentState = pTrans->GetTargetState();
	}

	// Update our currentState
	m_pCurrentState = m_pCurrentState->Update();
}
