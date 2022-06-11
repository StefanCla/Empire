#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Vessel.h"

class SupplyVessel : public Vessel
{
public:
	SupplyVessel();
	~SupplyVessel();

	void Update(float deltaTime) override;
	void ExecuteObjective() override;

private:
	unsigned int m_Credits;
};