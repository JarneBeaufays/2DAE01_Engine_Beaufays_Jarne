#include "MiniginPCH.h"
#include "Observer.h"

void Subject::Notify(dae::GameObject* pObject, ObserverEvent event)
{
	// Notify all observers!
	for (Observer* pObserver : m_Observers) pObserver->Notify(pObject, event);
}

void Subject::RemoveObserver(Observer* pObserver)
{
	// Loop over all our observers and see if it has to go
	for (unsigned int i{}; i < m_Observers.size(); i++)
	{
		if (m_Observers[i] == pObserver) 
		{
			// Delete this observer
			delete m_Observers[i];

			// Replace the open spot with our last observer
			m_Observers[i] = m_Observers.back();
			m_Observers.pop_back();
		}
	}
}
