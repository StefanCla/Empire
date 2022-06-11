#pragma once
#include "Headers/Precomp.h"
#include "Headers/PlanetManager.h"
#include "Headers/ShipManager.h"
#include "Headers/Menu.h"
#include "Headers/GameObjects/Planet.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#ifdef LINUX
#include "../Empire_Linux/Headers/Input.h"
#include "../Empire_Linux/Headers/RenderWindow.h"
#elif WINDOWS
#include "../Empire_Windows/Headers/Input.h"
#include "../Empire_Windows/Headers/RenderWindow.h"
#endif

class Camera;
class Menu;
class EmpireController;
class Vessel;

class GameContext
{
public:
	GameContext(const GameContext& g) = delete;
	GameContext& operator=(const GameContext& g) = delete;

	static GameContext* Instance();

	void Init(int width, int height);
	void InitGame();

	void Update(float deltaTime);
	void Render();
	void TurnLoop();

	bool IsRunning() const;

	bool IsWaitingForPlayerTurn() const;
	void WaitForPlayerTurnEnd();
	void NotifyTurnThread();

	Camera* GetCamera() { return m_Camera; }
	std::vector<EmpireController*>& GetEmpire() { return m_Empires; }
	EmpireController* GetPlayerEmpire() { return m_PlayerEmpire; }

	void SetCloseProgram(bool value) { m_CloseProgram = value; }
	bool GetCloseProgram() const { return m_CloseProgram; }

	btDiscreteDynamicsWorld* GetPhysicsWorldDynamic() const { return m_PhysicsWorldDynamic; }

	//Picking
	void SetSelectedShip(std::shared_ptr<Vessel> ship) { m_SelectedShip = ship; }
	void SetSelectedObject(GameObject* object) { m_SelectedObject = object; }
	void SetHoverObject(GameObject* hover) { m_HoverObject = hover; }
	std::shared_ptr<Vessel> GetSelectedShip() const { return m_SelectedShip; }
	GameObject* GetSelectedObject() const { return m_SelectedObject; }
	GameObject* GetHoverObject() const { return m_HoverObject; }

private:
	GameContext();
	~GameContext();

	// Turn based variables
	std::condition_variable m_TurnEndCondition;
	std::mutex m_TurnMutex;

	std::vector<EmpireController*> m_Empires;
	EmpireController* m_PlayerEmpire;
	std::thread m_TurnThread;
	unsigned int m_CurrentTurn = 0;

	// General variables
	Menu* m_Menu = nullptr;
	Camera* m_Camera = nullptr;
	Input* m_Input = nullptr;

	//Bullet Statc
	btBroadphaseInterface* m_BroadPhaseStatic;
	btDefaultCollisionConfiguration* m_CollisionConfigurationStatic;
	btCollisionDispatcher* m_DispatcherStatic;
	btSequentialImpulseConstraintSolver* m_ConstraintSolverStatic;
	btDiscreteDynamicsWorld* m_PhysicsWorldStatic;
	//Bullet Dynamic
	btBroadphaseInterface* m_BroadPhaseDynamic;
	btDefaultCollisionConfiguration* m_CollisionConfigurationDynamic;
	btCollisionDispatcher* m_DispatcherDynamic;
	btSequentialImpulseConstraintSolver* m_ConstraintSolverDynamic;
	btDiscreteDynamicsWorld* m_PhysicsWorldDynamic;
	
	//Picking
	std::shared_ptr<Vessel> m_SelectedShip;
	GameObject* m_SelectedObject;
	GameObject* m_HoverObject;

	bool m_WaitingForPlayerTurn = false;
	bool m_RenderEmpireSOI = true;
	bool m_BulletInit = false;
	bool m_Running = false;
	bool m_CloseProgram = false;
};
