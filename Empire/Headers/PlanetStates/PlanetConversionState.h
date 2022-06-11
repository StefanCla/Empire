#pragma once
#include "Headers/Precomp.h"
#include "Headers/FSM/FSMBase.h"

class EmpireController;

class PlanetConversionState : public FSMBase
{
public:
	PlanetConversionState();
	~PlanetConversionState();

	void OnStart();
	void OnUpdate();
	void OnExit();

	void SetTurns(unsigned int turns) { m_TurnsUntilConversion = turns; }

private:
	std::shared_ptr<EmpireController> m_Empire;
	unsigned int m_TurnsUntilConversion;
};