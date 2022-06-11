#pragma once
#include "Headers/Random.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>

class Planet;
class VertexBuffer;

class PlanetManager
{
public:
	PlanetManager(const PlanetManager& pm) = delete;
	PlanetManager& operator=(const PlanetManager& pm) = delete;

	static PlanetManager* Instance();

	void Init(btDiscreteDynamicsWorld* staticWorld);
	void Update(float deltaTime);
	void Render();

	bool IsActive() const { return m_Active; }

	std::unordered_map<std::string, std::shared_ptr<Planet>>& GetPlanets();
	std::shared_ptr<Planet> GetRandomHomePlanet();

private:
	PlanetManager();
	~PlanetManager();

	void UpdateColorInstanceData();
	void UpdateSOIRaddiInstanceData();
	void UpdateTransformInstanceData();

	std::unordered_map<std::string, std::shared_ptr<Planet>> m_Planets;

	std::vector<glm::vec3> m_PlanetColors;
	std::vector<glm::vec3> m_PlanetPositions;
	std::vector<float> m_PlanetSOIRadii;
	std::vector<glm::mat4> m_PlanetMatrices;

	bool m_Active = false;
};
