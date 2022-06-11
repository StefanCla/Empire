#pragma once
#include "Headers/Controllers/EmpireController.h"

class AIController : public EmpireController
{
public:
	AIController(const glm::vec3& color);
	~AIController() override;

	void StartTurn() override;
	void FirstTurn() override;

private:
	void UpgradePlanets();
};