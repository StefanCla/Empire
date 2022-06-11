#include "Headers/GameContext.h"
#include "Headers/Renderer/Renderer.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Models/ModelManager.h"
#include "Headers/Textures/TextureManager.h"
#include "Headers/Camera.h"
#include "Headers/Controllers/EmpireController.h"
#include "Headers/Controllers/PlayerController.h"
#include "Headers/Controllers/AIController.h"
#include "Headers/Bullet/BulletDebugDraw.h"
#include <glm/gtc/type_ptr.hpp>

GameContext* GameContext::Instance()
{
	static GameContext* s_Instance = new GameContext();
	return s_Instance;
}

GameContext::GameContext()
{}

GameContext::~GameContext()
{
	if (m_TurnThread.joinable())
		m_TurnThread.join();

	delete m_Camera;
}

void GameContext::Init(int width, int height)
{
	RenderWindow::Instance()->Init(width, height);

	Renderer::Instance()->Init(width, height);
	Renderer::Instance()->SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

#ifdef LINUX
	std::string svModelPath = "../../../Empire/Resources/Models/brian_03.obj";
	std::string dvModelPath = "../../../Empire/Resources/Models/brian_02.obj";
	std::string cvModelPath = "../../../Empire/Resources/Models/brian_01.obj";
	
	std::vector<std::string> skyboxFilepaths = {
		"../../../Empire/Resources/Textures/Skybox/Skybox_Right.png",
		"../../../Empire/Resources/Textures/Skybox/Skybox_Left.png",
		"../../../Empire/Resources/Textures/Skybox/Skybox_Top.png",
		"../../../Empire/Resources/Textures/Skybox/Skybox_Bottom.png",
		"../../../Empire/Resources/Textures/Skybox/Skybox_Front.png",
		"../../../Empire/Resources/Textures/Skybox/Skybox_Back.png"
	};
	TextureManager::Instance()->AddCubeMap("skybox_texture", skyboxFilepaths);
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "planet_texture", "../../../Empire/Resources/Textures/Planet_Medium_Res.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "sv_diffuse", "../../../Empire/Resources/Textures/brian_03.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "sv_emissive", "../../../Empire/Resources/Textures/brian_03_em.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "dv_diffuse", "../../../Empire/Resources/Textures/brian_02.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "dv_emissive", "../../../Empire/Resources/Textures/brian_02_em.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "cv_diffuse", "../../../Empire/Resources/Textures/brian_01.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "cv_emissive", "../../../Empire/Resources/Textures/brian_01_em.png");

	// Shaders
	ModelManager::Instance()->AddShader(ShaderType::Default, "../../../Empire/Resources/Shaders/Default.vert", "../../../Empire/Resources/Shaders/Default.frag");
	ModelManager::Instance()->AddShader(ShaderType::Phong, "../../../Empire/Resources/Shaders/Phong.vert", "../../../Empire/Resources/Shaders/Phong.frag");
	ModelManager::Instance()->AddShader(ShaderType::Skybox, "../../../Empire/Resources/Shaders/Skybox.vert", "../../../Empire/Resources/Shaders/Skybox.frag");
#elif WINDOWS
	std::string svModelPath = "../Empire/Resources/Models/brian_03.obj";
	std::string dvModelPath = "../Empire/Resources/Models/brian_02.obj";
	std::string cvModelPath = "../Empire/Resources/Models/brian_01.obj";

	std::vector<std::string> skyboxFilepaths = {
		"../Empire/Resources/Textures/Skybox/Skybox_Right.png",
		"../Empire/Resources/Textures/Skybox/Skybox_Left.png",
		"../Empire/Resources/Textures/Skybox/Skybox_Top.png",
		"../Empire/Resources/Textures/Skybox/Skybox_Bottom.png",
		"../Empire/Resources/Textures/Skybox/Skybox_Front.png",
		"../Empire/Resources/Textures/Skybox/Skybox_Back.png"
};
	TextureManager::Instance()->AddCubeMap("skybox_texture", skyboxFilepaths);
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "planet_texture", "../Empire/Resources/Textures/Planet_High_Res.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "sv_diffuse", "../Empire/Resources/Textures/brian_03.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "sv_emissive", "../Empire/Resources/Textures/brian_03_em.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "dv_diffuse", "../Empire/Resources/Textures/brian_02.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "dv_emissive", "../Empire/Resources/Textures/brian_02_em.png");
	TextureManager::Instance()->AddTexture(TextureType::Diffuse, "cv_diffuse", "../Empire/Resources/Textures/brian_01.png");
	TextureManager::Instance()->AddTexture(TextureType::Emission, "cv_emissive", "../Empire/Resources/Textures/brian_01_em.png");

	// Shaders
	ModelManager::Instance()->AddShader(ShaderType::Default, "../Empire/Resources/Shaders/Default.vert", "../Empire/Resources/Shaders/Default.frag");
	ModelManager::Instance()->AddShader(ShaderType::Phong, "../Empire/Resources/Shaders/Phong.vert", "../Empire/Resources/Shaders/Phong.frag");
	ModelManager::Instance()->AddShader(ShaderType::Skybox, "../Empire/Resources/Shaders/Skybox.vert", "../Empire/Resources/Shaders/Skybox.frag");
#endif

	// Skybox
	ModelManager::Instance()->AddModel(ModelType::Skybox, Geometry::GetModel(GeometryType::Cube));
	const std::shared_ptr<Model>& skyboxModel = ModelManager::Instance()->GetModel(ModelType::Skybox);
	skyboxModel->AddTexture(TextureManager::Instance()->GetTexture("skybox_texture"));
	skyboxModel->SetShader(ModelManager::Instance()->GetShader(ShaderType::Skybox));

	ModelManager::Instance()->AddModel(ModelType::PlanetPoint, Geometry::GetModel(GeometryType::Point));
	ModelManager::Instance()->GetModel(ModelType::PlanetPoint)->AddTexture(TextureManager::Instance()->GetTexture("planet_texture"));
	ModelManager::Instance()->GetModel(ModelType::PlanetPoint)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Default));

	ModelManager::Instance()->AddModel(ModelType::PlanetSOI, Geometry::GetModel(GeometryType::Circle));
	ModelManager::Instance()->GetModel(ModelType::PlanetSOI)->AddTexture(TextureManager::Instance()->GetTexture("planet_texture"));
	ModelManager::Instance()->GetModel(ModelType::PlanetSOI)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Default));

	ModelManager::Instance()->AddModel(ModelType::PlanetSphere, Geometry::GetModel(GeometryType::Sphere));
	ModelManager::Instance()->GetModel(ModelType::PlanetSphere)->AddTexture(TextureManager::Instance()->GetTexture("planet_texture"));
	ModelManager::Instance()->GetModel(ModelType::PlanetSphere)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Phong));

	ModelManager::Instance()->AddModel(ModelType::SV, svModelPath);
	ModelManager::Instance()->GetModel(ModelType::SV)->AddTexture(TextureManager::Instance()->GetTexture("sv_diffuse"));
	ModelManager::Instance()->GetModel(ModelType::SV)->AddTexture(TextureManager::Instance()->GetTexture("sv_emissive"));
	ModelManager::Instance()->GetModel(ModelType::SV)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Phong));

	ModelManager::Instance()->AddModel(ModelType::DV, dvModelPath);
	ModelManager::Instance()->GetModel(ModelType::DV)->AddTexture(TextureManager::Instance()->GetTexture("dv_diffuse"));
	ModelManager::Instance()->GetModel(ModelType::DV)->AddTexture(TextureManager::Instance()->GetTexture("dv_emissive"));
	ModelManager::Instance()->GetModel(ModelType::DV)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Phong));

	ModelManager::Instance()->AddModel(ModelType::CV, cvModelPath);
	ModelManager::Instance()->GetModel(ModelType::CV)->AddTexture(TextureManager::Instance()->GetTexture("cv_diffuse"));
	ModelManager::Instance()->GetModel(ModelType::CV)->AddTexture(TextureManager::Instance()->GetTexture("cv_emissive"));
	ModelManager::Instance()->GetModel(ModelType::CV)->SetShader(ModelManager::Instance()->GetShader(ShaderType::Phong));

#ifdef LINUX
	m_Menu = new Menu("#version 300 es", this);
#elif WINDOWS
	m_Menu = new Menu("#version 460", this); //I think this is the right version
#endif
	m_Input = new Input();
	m_Camera = new Camera({ 0.0f, 0.0f, -300.0f }, m_Input);

	m_Running = true;
}

void GameContext::InitGame()
{
	// Static World
	m_BroadPhaseStatic = new btDbvtBroadphase();
	m_CollisionConfigurationStatic = new btDefaultCollisionConfiguration();
	m_DispatcherStatic = new btCollisionDispatcher(m_CollisionConfigurationStatic);
	m_ConstraintSolverStatic = new btSequentialImpulseConstraintSolver();
	m_PhysicsWorldStatic = new btDiscreteDynamicsWorld(m_DispatcherStatic, m_BroadPhaseStatic, m_ConstraintSolverStatic, m_CollisionConfigurationStatic);

	// Dynamic World
	m_BroadPhaseDynamic = new btDbvtBroadphase();
	m_CollisionConfigurationDynamic = new btDefaultCollisionConfiguration();
	m_DispatcherDynamic = new btCollisionDispatcher(m_CollisionConfigurationDynamic);
	m_ConstraintSolverDynamic = new btSequentialImpulseConstraintSolver();
	m_PhysicsWorldDynamic = new btDiscreteDynamicsWorld(m_DispatcherDynamic, m_BroadPhaseDynamic, m_ConstraintSolverDynamic, m_CollisionConfigurationDynamic);

	m_PhysicsWorldDynamic->setGravity(btVector3(0, 0, 0)); //9.81 gravity
	
	BulletDebugDraw::Instance()->Init();
	BulletDebugDraw::Instance()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	m_PhysicsWorldStatic->setDebugDrawer(BulletDebugDraw::Instance());
	m_PhysicsWorldDynamic->setDebugDrawer(BulletDebugDraw::Instance());

	PlanetManager::Instance()->Init(m_PhysicsWorldStatic);

	m_BulletInit = true;

	// Setup turn-based
	glm::vec3 aiColor = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 playerColor = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Empires.push_back(new AIController(aiColor));
	m_Empires.push_back(new PlayerController(playerColor));

	m_PlayerEmpire = m_Empires[1];

	ShipManager::Instance()->Init(m_PhysicsWorldDynamic);

	m_TurnThread = std::thread(&GameContext::TurnLoop, this);
}

void GameContext::Update(float deltaTime)
{
	m_Input->GetInput();
	m_Camera->Update(deltaTime);

	if (PlanetManager::Instance()->IsActive())
	{
		PlanetManager::Instance()->Update(deltaTime);
	}

	if (ShipManager::Instance()->IsActive())
		ShipManager::Instance()->Update(deltaTime);

	if (m_BulletInit)
	{
		m_PhysicsWorldDynamic->stepSimulation(1 / 60.0f, 5);

		//Needed to avoid click on planets under menus
		ImGuiIO io = ImGui::GetIO();
		if(!io.WantCaptureMouse)
			m_Camera->Picking(m_PhysicsWorldDynamic, m_PhysicsWorldStatic);
	}
}

void GameContext::Render()
{
	Renderer::Instance()->Clear();
	Renderer::Instance()->BeginScene(m_Camera);

	if (PlanetManager::Instance()->IsActive())
	{
		PlanetManager::Instance()->Render();
	}

	if (ShipManager::Instance()->IsActive())
	{
		ShipManager::Instance()->Render();

		if (BULLET_DEBUG_DRAWING && m_BulletInit)
		{
			m_PhysicsWorldStatic->debugDrawWorld();
			m_PhysicsWorldDynamic->debugDrawWorld();
		}

		const std::shared_ptr<Model>& skyboxModel = ModelManager::Instance()->GetModel(ModelType::Skybox);
		skyboxModel->Bind();

		Renderer::Instance()->Submit(skyboxModel->GetShader());
		Renderer::Instance()->RenderSkybox(skyboxModel->GetShader(), skyboxModel->GetVAO()->GetIndexBuffer()->GetCount());

		if (m_RenderEmpireSOI)
		{
			for (EmpireController* empire : m_Empires)
				empire->RenderSOI();
		}
	}

	m_Menu->Render();
}

void GameContext::TurnLoop()
{
	/* 
		The turn will be advanced to the next empire
		- Player ends his turn by pressing a button in a menu or by pressing a button
		- AI empire will advance turn once it has taken all its actions that it had planned for that turn
	*/
	m_Empires[0]->FirstTurn();
	m_Empires[1]->FirstTurn();

	while (m_Running)
	{
		m_CurrentTurn++;
		if (m_CurrentTurn == m_Empires.size())
			m_CurrentTurn = 0;
		m_Empires[m_CurrentTurn]->StartTurn();
	}
}

bool GameContext::IsRunning() const
{
	return m_Running;
}

bool GameContext::IsWaitingForPlayerTurn() const
{
	return m_WaitingForPlayerTurn;
}

void GameContext::WaitForPlayerTurnEnd()
{
	m_WaitingForPlayerTurn = true;

	std::unique_lock<std::mutex> lock(m_TurnMutex);
	m_TurnEndCondition.wait(lock);

	m_WaitingForPlayerTurn = false;
}

void GameContext::NotifyTurnThread()
{
	m_TurnEndCondition.notify_one();
}
