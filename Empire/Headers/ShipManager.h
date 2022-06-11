#pragma once
#include "Headers/Precomp.h"
#include <unordered_map>
#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
//#include "Headers/GameObjects/GameObject.h"
#include "Headers/GameObjects/Vessel.h"

class ShipManager
{
public:
	ShipManager(const ShipManager& sm) = delete;
	ShipManager& operator=(const ShipManager& sm) = delete;

	static ShipManager* Instance();

	void Init(btDiscreteDynamicsWorld* dynamicWorld);
	void Update(float deltaTime);
	void Render();

	bool IsActive() const { return m_Active; }

	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>& GetShips();

private:
	ShipManager();
	~ShipManager();
		
	void UpdateTransformInstanceData();
	bool m_Active = false;

	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>> m_Ships;

	std::vector<glm::vec3> m_ShipColor;
	std::vector<glm::vec3> m_ShipPosition;
	std::vector<glm::mat4> m_SVShipMatrices;
	std::vector<glm::mat4> m_DVShipMatrices;
	std::vector<glm::mat4> m_CVShipMatrices;

};