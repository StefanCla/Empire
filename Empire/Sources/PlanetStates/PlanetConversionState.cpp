#include "Headers/PlanetStates/PlanetConversionState.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/Controllers/EmpireController.h"

PlanetConversionState::PlanetConversionState()
{
	m_Name = "PlanetConversionState";
}

PlanetConversionState::~PlanetConversionState()
{}

void PlanetConversionState::OnStart()
{
}

void PlanetConversionState::OnUpdate()
{
	if (m_TurnsUntilConversion == 0)
		m_Owner->ChangeState(m_Owner->GetState("PlanetActiveState"));
	else
		m_TurnsUntilConversion--;
}

void PlanetConversionState::OnExit()
{
	static_cast<Planet*>(m_Owner->GetOwner())->ConvertPlanet();
}
