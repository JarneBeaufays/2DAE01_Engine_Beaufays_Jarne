#pragma once
#include "GameObject.h"

enum class ObserverEvent 
{
	playerStartedRunning, playerJumped,
	playerDropped, killedEnemy
};

class Observer
{
public:

	// Functions
	virtual void Notify(dae::GameObject* pObject, ObserverEvent event) = 0;

};

class Subject 
{
public:

	// Functions
	void AddObserver(Observer* pObserver) { m_Observers.push_back(pObserver); }
	void RemoveObserver(Observer* pObserver);

protected:

	// Functions
	void Notify(dae::GameObject* pObject, ObserverEvent event);

private:

	// Variables
	std::vector<Observer*> m_Observers;

};