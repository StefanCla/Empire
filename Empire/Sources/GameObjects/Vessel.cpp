#include "Headers/GameObjects/Vessel.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/GameContext.h"

Vessel::Vessel() :
	m_Distance(0.0f),
	m_DistanceTraveld(0.0f),
	m_Planet(nullptr),
	m_Direction({ 0.0f, 0.0f, 0.0f }),
	m_Speed(0.05f),
	m_MidwayPoint({ 0, 0, 0 }),
	m_DistPerJump(0.0f),
	m_StartPos({ 0,0,0 }),
	m_TotalDist(0.0f)
{}

Vessel::~Vessel()
{}

void Vessel::InitBullet()
{
	m_BoxCollision = new btBoxShape(btVector3(2.0f, 2.0f, 2.0f));
	GameObject::InitBullet();
	GameContext::Instance()->GetPhysicsWorldDynamic()->addRigidBody(m_RigidBody);
}

void Vessel::SetMove(Planet* planet)
{
	if (m_Planet == nullptr)
	{
		m_Planet = planet;

		m_Direction = glm::normalize((planet->GetPosition() - m_WorldPosition));
		m_Distance = glm::distance(m_WorldPosition, (planet->GetPosition()));
		m_DistPerJump = m_Distance / MAXMOVEMENT;
		m_MidwayPoint = m_WorldPosition + m_Direction * MAXMOVEMENT;
		m_StartPos = m_WorldPosition;
		m_TotalDist = m_DistPerJump;
	}
}

void Vessel::SetSmallDist()
{
	m_DistanceTraveld = 0.0f;
}

void Vessel::Move(float deltaTime)
{
	//Thanks a lot to Wessel Frijters & Tim van der Leeden for
	//letting me know i did not use linear interpolation at first

	//I (Stefan) mixed up ray-cast with linear interpolation and thought i had it, which i didnt.

	if (m_Planet != nullptr)
	{
		if (m_TotalDist <= 0.0f) //Max distance has been reached
		{
			SetPosition(m_Planet->GetPosition());
			ExecuteObjective();

			m_Planet = nullptr;

			m_DistanceTraveld = 0.0f;
			m_TotalDist = 0.0f;
			m_Distance = 0.0f;
		}
		else if (m_DistanceTraveld < 1.0f)
		{
			float val = (m_Speed * deltaTime) * MAXMOVEMENT;
			m_DistanceTraveld += val;
			m_TotalDist -= val;

			SetPosition((1.0f - m_DistanceTraveld) * m_StartPos + m_DistanceTraveld * m_MidwayPoint);
		}
		else
		{
			m_StartPos = m_WorldPosition;
			m_MidwayPoint = m_WorldPosition + m_Direction * MAXMOVEMENT;
		}
	}
}