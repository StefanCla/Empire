#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Vessel.h"

class CarrierVessel : public Vessel
{
public:
	CarrierVessel();
	~CarrierVessel();

	void Update(float deltaTime) override;
	void ExecuteObjective() override;

	void SetSize(unsigned int size) { m_Size = size; }
	unsigned int GetSize() { return m_Size; }

private:
	unsigned int m_Size;

};