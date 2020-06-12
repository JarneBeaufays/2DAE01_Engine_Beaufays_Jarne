#pragma once
#include "GameObject.h"
#include "ObjectComponent.h"

class State 
{

};

class  StateMachineComponent : public ObjectComponent
{
public:

	// Functions
	StateMachineComponent(dae::GameObject* pParent);
	void Update() override;
	void Render() const override;

private:

	// Variables


};
