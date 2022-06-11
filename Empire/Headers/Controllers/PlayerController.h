#pragma once
#include "Headers/Controllers/EmpireController.h"
#include "Headers/GameObjects/DiscoveryVessel.h"

class Planet;

class PlayerController : public EmpireController
{
public:
	PlayerController(const glm::vec3& color);
	~PlayerController() override;

	void StartTurn() override;
	void FirstTurn() override;

};
