#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/FSM/FSMBase.h"

class PlanetInactiveState : public FSMBase
{
public:
	PlanetInactiveState();
	~PlanetInactiveState();
	void OnStart();
	void OnUpdate();
};