#pragma once
#include "Headers/Precomp.h"
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>
#include "Headers/GameObjects/GameObject.h"

class Planet;
class Vessel;

class EmpireController
{
public:
	EmpireController(const glm::vec3& color);
	virtual ~EmpireController();

	virtual void StartTurn() = 0;
	virtual void FirstTurn() = 0;
	virtual void RenderSOI();

	unsigned int GetShipTypeCount(ObjectType type) const;
	void BuyShip(ObjectType type, unsigned int cost, unsigned int size = 0);

	void AddPlanet(std::shared_ptr<Planet> planet);
	void AddPlanetByName(const std::string& planetName);
	void AddShip(ObjectType type, std::vector<std::shared_ptr<Vessel>> ship);
	const glm::vec3& GetColor() const { return m_Color; }
	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>> GetShips() { return m_Ships; }
	unsigned int GetCredits() const { return m_Credits; }
	void AddCredits(int credits) { m_Credits += credits; }
	void RemoveCredits(int credits) { m_Credits -= credits; }

protected:
	std::unordered_map<std::string, std::shared_ptr<Planet>> m_Planets;
	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>> m_Ships;

	glm::vec3 m_Color;
	int m_Credits = 0;
};
