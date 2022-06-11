#pragma once
#include "Headers/Precomp.h"
#include "Headers/GameObjects/Planet.h"
#include <unordered_map>
#include <string>

class Generator
{
public:
	Generator();
	~Generator();

	void Init(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets, char* seed, unsigned int arms, float armOffset, float rotateFac);

private:
	void Create(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets);
	glm::vec3 GetRandomGalaxyPosition();
	std::string GetRandomName();
	void NameDupeChecker(std::unordered_map<std::string, std::shared_ptr<Planet>>& planets, std::shared_ptr<Planet> planet);

	unsigned int m_Seed;
	int m_Arms;
	float m_ArmOffset;
	float m_RotateFactor;
};