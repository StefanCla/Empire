#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/FSM/FSMBase.h"

class PlanetActiveState : public FSMBase
{
public:
	PlanetActiveState();
	~PlanetActiveState();

	void OnStart();
	void OnUpdate();

private:
	void UpdateCredits();
	void UpdateSOI();
};