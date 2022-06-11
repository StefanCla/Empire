#include "Headers/ShipManager.h"
#include "Headers/Renderer/Renderer.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Models/ModelManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "Headers/GameContext.h"
#include "Headers/Controllers/EmpireController.h"

//Vessels
#include "Headers/GameObjects/SupplyVessel.h"
#include "Headers/GameObjects/DiscoveryVessel.h"
#include "Headers/GameObjects/CarrierVessel.h"

ShipManager* ShipManager::Instance()
{
	static ShipManager* s_Instance = new ShipManager;
	return s_Instance;
}

ShipManager::ShipManager()
{}

ShipManager::~ShipManager()
{}

void ShipManager::Init(btDiscreteDynamicsWorld* dynamicWorld)
{
	//Side vectors are for empire only-ships
	std::vector<std::shared_ptr<Vessel>> svVector;
	std::vector<std::shared_ptr<Vessel>> svVectorSide;
	std::vector<std::shared_ptr<Vessel>> dvVector;
	std::vector<std::shared_ptr<Vessel>> dvVectorSide;
	std::vector<std::shared_ptr<Vessel>> cvVector;
	std::vector<std::shared_ptr<Vessel>> cvVectorSide;

	//Init Supply Vessel
	for (int i = 0; i < (SHIPNUM_SV * 2); i++)
	{
		std::shared_ptr<SupplyVessel> SV = std::make_shared<SupplyVessel>();
		SV->SetPosition(glm::vec3((-30.0f - i * 5), 0.0f, -10.0f));
		svVector.push_back(SV);
		svVectorSide.push_back(SV);

		if (i == (SHIPNUM_SV - 1))
		{
			GameContext::Instance()->GetEmpire()[0]->AddShip(ObjectType::SV, svVectorSide);
			svVectorSide.clear();
		}
		else if (i == (SHIPNUM_SV * 2 - 1))
		{
			GameContext::Instance()->GetEmpire()[1]->AddShip(ObjectType::SV, svVectorSide);
			svVectorSide.clear();
		}
	}
	m_Ships.insert({ ObjectType::SV, svVector });

	//Init Discovery Vessel
	for (int i = (SHIPNUM_SV * 2); i < ((SHIPNUM_SV + SHIPNUM_DV ) * 2); i++)
	{
		std::shared_ptr<DiscoveryVessel> DV = std::make_shared<DiscoveryVessel>();
		DV->SetPosition(glm::vec3((-10.0f - i * 5), 0.0f, -10.0f));
		dvVector.push_back(DV);
		dvVectorSide.push_back(DV);

		if (i == (SHIPNUM_SV * 2 + SHIPNUM_DV - 1))
		{
			GameContext::Instance()->GetEmpire()[0]->AddShip(ObjectType::DV, dvVectorSide);
			dvVectorSide.clear();
		}
		else if (i == ((SHIPNUM_SV + SHIPNUM_DV) * 2 - 1))
		{
			GameContext::Instance()->GetEmpire()[1]->AddShip(ObjectType::DV, dvVectorSide);
			dvVectorSide.clear();
		}
	}
	m_Ships.insert({ ObjectType::DV, dvVector });

	//Init Carrier Vessel
	for (int i = ((SHIPNUM_SV + SHIPNUM_DV) * 2); i < ((SHIPNUM_SV + SHIPNUM_DV + SHIPNUM_CV) * 2); i++)
	{
		std::shared_ptr<CarrierVessel> CV = std::make_shared<CarrierVessel>();
		CV->SetPosition(glm::vec3((-5.0f + i * 5), 0.0f, -10.0f));
		cvVector.push_back(CV);
		cvVectorSide.push_back(CV);

		if (i == ((SHIPNUM_SV + SHIPNUM_DV) * 2 + SHIPNUM_CV - 1))
		{
			GameContext::Instance()->GetEmpire()[0]->AddShip(ObjectType::CV, cvVectorSide);
			cvVectorSide.clear();
		}
		else if (i == (((SHIPNUM_SV + SHIPNUM_DV + SHIPNUM_CV) * 2) - 1))
		{
			GameContext::Instance()->GetEmpire()[1]->AddShip(ObjectType::CV, cvVectorSide);
			cvVectorSide.clear();
		}
	}
	m_Ships.insert({ ObjectType::CV, cvVector });


	std::shared_ptr<VertexBuffer> shipColorVB = std::make_shared<VertexBuffer>(((SHIPNUM_SV + SHIPNUM_DV + SHIPNUM_CV)* 2) * sizeof(glm::vec3));
	shipColorVB->SetLayout({
		{ ShaderDataType::Float3, "in_instanceColor" }
		});

	std::shared_ptr<VertexBuffer> shipPositionVB = std::make_shared<VertexBuffer>(((SHIPNUM_SV + SHIPNUM_DV + SHIPNUM_CV) * 2) * sizeof(glm::vec3));
	shipPositionVB->SetLayout({
		{ ShaderDataType::Float3, "in_instancePosition" }
		});

	//SV
	std::shared_ptr<VertexBuffer> svShipMatrixVB = std::make_shared<VertexBuffer>((SHIPNUM_SV * 2) * sizeof(glm::mat4));
	svShipMatrixVB->SetLayout({
		{ ShaderDataType::Mat4, "in_instanceTransform" }
		});
	//DV
	std::shared_ptr<VertexBuffer> dvShipMatrixVB = std::make_shared<VertexBuffer>((SHIPNUM_DV * 2) * sizeof(glm::mat4));
	dvShipMatrixVB->SetLayout({
		{ ShaderDataType::Mat4, "in_instanceTransform" }
		});
	//CV
	std::shared_ptr<VertexBuffer> cvShipMatrixVB = std::make_shared<VertexBuffer>((SHIPNUM_CV * 2) * sizeof(glm::mat4));
	cvShipMatrixVB->SetLayout({
		{ ShaderDataType::Mat4, "in_instanceTransform"}
		});

	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>::const_iterator it;
	for (it = m_Ships.begin(); it != m_Ships.end(); it++)
	{
		for (std::shared_ptr<Vessel> ship : it->second)
		{
			m_ShipColor.push_back(ship->GetColor());
			m_ShipPosition.push_back(ship->GetPosition());

			switch (it->first)
			{
			case ObjectType::SV: m_SVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			case ObjectType::DV: m_DVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			case ObjectType::CV: m_CVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			default: printf("Error: Unknown ship\n");
				break;
			}
		}
	}

	shipColorVB->SetBufferData(glm::value_ptr(m_ShipColor[0]), m_ShipColor.size() * sizeof(glm::vec3));
	shipPositionVB->SetBufferData(glm::value_ptr(m_ShipPosition[0]), m_ShipPosition.size() * sizeof(glm::vec3));
	svShipMatrixVB->SetBufferData(glm::value_ptr(m_SVShipMatrices[0][0]), m_SVShipMatrices.size() * sizeof(glm::mat4));
	dvShipMatrixVB->SetBufferData(glm::value_ptr(m_DVShipMatrices[0][0]), m_DVShipMatrices.size() * sizeof(glm::mat4));
	cvShipMatrixVB->SetBufferData(glm::value_ptr(m_CVShipMatrices[0][0]), m_CVShipMatrices.size() * sizeof(glm::mat4));

	//Supply Vessel
	const std::shared_ptr<Model>& svModel = ModelManager::Instance()->GetModel(ModelType::SV);
	svModel->GetVAO()->AddVertexBuffer(VertexBufferType::ModelMatrixVB, svModel->GetShader()->GetRendererID(), svShipMatrixVB);

	//Discovery Vessel
	const std::shared_ptr<Model>& dvModel = ModelManager::Instance()->GetModel(ModelType::DV);
	dvModel->GetVAO()->AddVertexBuffer(VertexBufferType::ModelMatrixVB, dvModel->GetShader()->GetRendererID(), dvShipMatrixVB);
	
	//Carrier Vessel
	const std::shared_ptr<Model>& cvModel = ModelManager::Instance()->GetModel(ModelType::CV);
	cvModel->GetVAO()->AddVertexBuffer(VertexBufferType::ModelMatrixVB, cvModel->GetShader()->GetRendererID(), cvShipMatrixVB);
	
	m_Active = true;
}

void ShipManager::Update(float deltaTime)
{
	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>::const_iterator it;

	for (it = m_Ships.begin(); it != m_Ships.end(); it++)
		for(std::shared_ptr<GameObject> ship : it->second)
			ship->Update(deltaTime);
}

void ShipManager::Render()
{
	UpdateTransformInstanceData();

	//Supply Vessel
	if (m_SVShipMatrices.size() > 0)
	{
		const std::shared_ptr<Model> svModel = ModelManager::Instance()->GetModel(ModelType::SV);
		svModel->Bind();
		svModel->GetVAO()->GetVertexBuffer(VertexBufferType::ModelMatrixVB)->SetBufferData(glm::value_ptr(m_SVShipMatrices[0][0]), m_SVShipMatrices.size() * sizeof(glm::mat4));
		Renderer::Instance()->Submit(svModel->GetShader());
		Renderer::Instance()->RenderElementsInstanced(svModel->GetShader(), svModel->GetIBO()->GetCount(), m_SVShipMatrices.size());
	}

	//Discovery Vessel
	if (m_DVShipMatrices.size() > 0)
	{
		const std::shared_ptr<Model> dvModel = ModelManager::Instance()->GetModel(ModelType::DV);
		dvModel->Bind();
		dvModel->GetVAO()->GetVertexBuffer(VertexBufferType::ModelMatrixVB)->SetBufferData(glm::value_ptr(m_DVShipMatrices[0][0]), m_DVShipMatrices.size() * sizeof(glm::mat4));
		Renderer::Instance()->Submit(dvModel->GetShader());
		Renderer::Instance()->RenderElementsInstanced(dvModel->GetShader(), dvModel->GetIBO()->GetCount(), m_DVShipMatrices.size());
	}

	//Carrier Vessel
	if (m_CVShipMatrices.size() > 0)
	{
		const std::shared_ptr<Model> cvModel = ModelManager::Instance()->GetModel(ModelType::CV);
		cvModel->Bind();
		cvModel->GetVAO()->GetVertexBuffer(VertexBufferType::ModelMatrixVB)->SetBufferData(glm::value_ptr(m_CVShipMatrices[0][0]), m_CVShipMatrices.size() * sizeof(glm::mat4));
		Renderer::Instance()->Submit(cvModel->GetShader());
		Renderer::Instance()->RenderElementsInstanced(cvModel->GetShader(), cvModel->GetIBO()->GetCount(), m_CVShipMatrices.size());
	}
}

void ShipManager::UpdateTransformInstanceData()
{
	m_SVShipMatrices.clear();
	m_DVShipMatrices.clear();
	m_CVShipMatrices.clear();

	std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>::const_iterator it;
	for (it = m_Ships.begin(); it != m_Ships.end(); it++)
	{
		for (std::shared_ptr<GameObject> ship : it->second)
		{
			switch (it->first)
			{
			case ObjectType::SV: if(ship->GetActive())	m_SVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			case ObjectType::DV: if(ship->GetActive())	m_DVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			case ObjectType::CV: if(ship->GetActive())	m_CVShipMatrices.push_back(ship->GetModelMatrix());
				break;
			default: printf("Error: Unknown ship\n");
				break;
			}
		}
	}
}

std::unordered_map<ObjectType, std::vector<std::shared_ptr<Vessel>>>& ShipManager::GetShips()
{
	return m_Ships;
}