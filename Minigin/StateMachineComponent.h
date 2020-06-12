#pragma once
#include "GameObject.h"
#include "ObjectComponent.h"
#include <functional>
#include <vector>

class Transition 
{
public:

	// Functions
	Transition(State* pTargetState) : m_pTargetState{ m_pTargetState } { };
	bool Update();
	void AddCondition(std::function<bool()>& function) { m_Conditions.push_back(function); }
	void AddExitAction(std::function<void()>& function) { m_ExitActions.push_back(function); }

	// Getters and Setters
	State* GetTargetState() const { return m_pTargetState; }

private:

	// Variables
	std::vector<std::function<bool()>> m_Conditions;
	std::vector<std::function<void()>> m_ExitActions;
	State* m_pTargetState;

};

class State 
{
public:

	// Functions
	State(const std::string& name) : m_StateName{ name }, m_Entered{ false } { };
	State* Update();

	void AddEntryAction(std::function<void()>& function) { m_EntryActions.push_back(function); }
	void AddAction(std::function<void()>& function) { m_Actions.push_back(function); }
	void AddExitAction(std::function<void()>& function) { m_ExitActions.push_back(function); }
	void AddTransition(Transition* pTransition) { m_Transitions.push_back(pTransition); }

private:

	// Variables
	std::vector<std::function<void()>> m_EntryActions;
	std::vector<std::function<void()>> m_Actions;
	std::vector<std::function<void()>> m_ExitActions;
	std::vector<Transition*> m_Transitions;
	std::string m_StateName;

	bool m_Entered;

};

class  StateMachineComponent : public ObjectComponent
{
public:

	// Functions
	StateMachineComponent(dae::GameObject* pParent, State* pState) : m_pCurrentState{ pState }, ObjectComponent { "StateMachineComponent", pParent } { };
	void Update() override;
	void Render() const override {};

private:

	// Variables
	State* m_pCurrentState;

};
