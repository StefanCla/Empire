#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/GameObject.h"
#include <bullet/btBulletDynamicsCommon.h>

//FSM
#include "Headers/FSM/FSM.h"
#include "Headers/FSM/FSMObjectBase.h"
#include "Headers/PlanetStates/PlanetActiveState.h"
#include "Headers/PlanetStates/PlanetConversionState.h"
#include "Headers/PlanetStates/PlanetInactiveState.h"

class PlanetActiveState;
class PlanetConversionState;
class PlanetInactiveState;

class Planet : public GameObject, public FSMObjectBase
{
public:
	Planet();
	~Planet();

	void Update(float dt) override;
	void OnTurnStart();
	void InitBullet() override;

	void ConvertPlanet();
	void GenerateCredits();

	void SetActiveState();

	bool IsNeutral() const { return m_Owner == nullptr ? true : false; }
	float GetSOIRadius() const;
	float GetMaxSOIRadius() const;
	const std::string& GetName() const { return m_Name; }
	void SetName(std::string name) { m_Name = name; }
	unsigned int GetTechLevel() const { return m_TechLevel; }
	unsigned int GetTechLevelIncreaseCost() const;
	bool IsMaxTechLevel() const;
	void IncreaseTechLevel();
	unsigned int GetResourceLevel() const { return m_ResourceLevel; }
	std::vector<std::shared_ptr<Planet>>& GetSphereList() { return m_SphereList; }
	void SetSphereList(std::vector<std::shared_ptr<Planet>> planetList) { m_SphereList = planetList; }
	PlanetConversionState* GetConversionState() const { return m_ConversionState; }
	void SetColor(const glm::vec3& color) { m_Color = color; }
	unsigned int GetCredits() const { return m_Credits; }
	unsigned int GetFighters() const { return m_Fighters; }

private:
	std::string m_Name;
	unsigned int m_TechLevel, m_ResourceLevel, m_Credits, m_Fighters;
	std::vector<std::shared_ptr<Planet>> m_SphereList; //SoI

	//FSM
	PlanetActiveState* m_ActiveState;
	PlanetConversionState* m_ConversionState;
	PlanetInactiveState* m_InactiveState;
};
