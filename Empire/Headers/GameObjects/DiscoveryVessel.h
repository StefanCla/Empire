#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Vessel.h"
#include "Headers/GameObjects/Planet.h"

class DiscoveryVessel : public Vessel
{
public:
	DiscoveryVessel();
	~DiscoveryVessel();

	void Update(float deltaTime) override;
	void ExecuteObjective() override;

};