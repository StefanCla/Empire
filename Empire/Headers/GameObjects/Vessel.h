#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/GameObject.h"

class Planet;

class Vessel : public GameObject
{
public:
	Vessel();
	~Vessel();

	virtual void Update(float deltaTime) = 0;
	virtual void InitBullet() override;
	virtual void ExecuteObjective() = 0;
	void Move(float deltaTime);
	void SetMove(Planet* planet);
	void SetSmallDist();

protected:
	Planet* m_Planet;

	float m_Speed;
	float m_Distance;
	float m_DistPerJump;

	float m_TotalDist;
	float m_DistanceTraveld;

	glm::vec3 m_MidwayPoint;
	glm::vec3 m_StartPos;
	glm::vec3 m_Direction;
};