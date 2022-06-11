#include "Headers/Camera.h"
#include "Headers/GameObjects/Planet.h"
#include "Headers/GameContext.h"
#include <iostream>

Camera::Camera(const glm::vec3& pos, Input* input)
{
	m_Input = input;
	m_Position = pos;
	m_Direction = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - m_Position);
	m_ScrollPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_TargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_IsSelected = false;

	m_ProjectionMatrix = glm::perspective(
		45.0f,			//FOV
		4.0f / 3.0f,	//Aspect Ratio
		0.1f,			//Min render distance
		1000.0f			//Max render distance
	);

	m_TargetPosition = m_Position + m_Direction;
	MakeViewProjectionMatrix();
}

void Camera::Update(float dt)
{
	glm::vec3 xDir = glm::cross(m_Direction, m_Up);
	glm::vec3 zDir = glm::cross(m_Direction, xDir);
	glm::vec3 cameraScroll = glm::vec3(0.0f, 0.0f, 0.0f);

	//Key input
	if (m_Input->m_Keys & (1 << 7))							//Scroll Up
	{
		cameraScroll += (m_Direction * m_Speed * 7.0f) * dt;
		m_Position += cameraScroll;
	}
	if (m_Input->m_Keys & (1 << 8))							//Scroll Down
	{
		cameraScroll -= (m_Direction * m_Speed * 7.0f) * dt;
		m_Position += cameraScroll;
	}

	if (m_Input->m_Keys & (1 << 1))							//W
	{
		m_Position -= zDir * m_Speed * dt;
		m_IsSelected = false;
	}
	if (m_Input->m_Keys & (1 << 2))							//S
	{
		m_Position += zDir * m_Speed * dt;
		m_IsSelected = false;
	}
	if (m_Input->m_Keys & (1 << 3))							//D
	{
		m_Position += xDir * m_Speed * dt;
		m_IsSelected = false;
	}
	if (m_Input->m_Keys & (1 << 4))							//A
	{
		m_Position -= xDir * m_Speed * dt;
		m_IsSelected = false;
	}

	//Mouse drag
	if (m_Input->m_Keys & (1 << 6) && !m_InDrag)
	{
		m_DragCenter = { m_Input->m_MouseX, m_Input->m_MouseY };
		m_InDrag = true;
	}

	if (m_InDrag)
	{
		if (!(m_Input->m_Keys & (1 << 6)))
			m_InDrag = false;
		else
		{
			float x = m_Input->m_MouseX - m_DragCenter.x;
			float y = m_Input->m_MouseY - m_DragCenter.y;
			if (x < 150 && x > -150 || y < 150 && y > -150)
			{
				glm::vec2 delta = glm::vec2(x, y);

				if (x < 150 && x > -150)
				{
					if (delta.y > 5)
						delta.y = 5;
					else if (delta.y < -5)
						delta.y = -5;

					delta.x = 0.0f;
				}
				else if (y < 150 && y > -150)
				{

					if(delta.x > 5)
						delta.x = 5;
					else if (delta.x < -5)
						delta.x = -5;

					delta.y = 0.0f;
				}
				delta *= 0.20f * dt;

				glm::mat4 rotX = glm::rotate(glm::mat4(1), delta.y, (glm::vec3(1.0f, 0.0f, 0.0f) * 15.0f));
				glm::mat4 rotY = glm::rotate(rotX, delta.x, (glm::vec3(0.0f, -1.0f, 0.0f) * 15.0f));
				m_Direction = static_cast<glm::mat3>(rotY) * m_Direction;
				m_Up = static_cast<glm::mat3>(rotX) * m_Up;
			}
		}
	}

	if (m_IsSelected)
	{
		m_ScrollPos += cameraScroll;
		m_Position = GameContext::Instance()->GetSelectedObject()->GetPosition() - (m_Direction * 15.0f);
		m_Position += m_ScrollPos;
	}
	else if (!m_IsSelected)
	{
		GameContext::Instance()->SetSelectedObject(nullptr);
	}

	m_TargetPosition = m_Position + m_Direction;
	MakeViewProjectionMatrix();

}

void Camera::RayNDC()
{
	//Allows for window resize
	glm::vec2 windowSize = RenderWindow::Instance()->GetWindowSize();

	m_RayStartNDC = glm::vec4(
		(m_Input->m_MouseX / static_cast<float>(windowSize.x) - 0.5f) * 2.0f,
		(m_Input->m_MouseY / static_cast<float>(windowSize.y) - 0.5f) * -2.0f,
		-1.0,
		1.0f
	);

	m_RayEndNDC = glm::vec4(
		(m_Input->m_MouseX / static_cast<float>(windowSize.x) - 0.5f) * 2.0f,
		(m_Input->m_MouseY / static_cast<float>(windowSize.y) - 0.5f) * -2.0f,
		0.0,
		1.0f
	);
}

Camera::~Camera()
{}

void Camera::MakeViewProjectionMatrix()
{
	m_ViewMatrix = glm::lookAt(
		m_Position,					//Camera world Pos
		m_TargetPosition,			//Target it looks at
		m_Up						//Camera side
	);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;// *m_MaxRot;
}

void Camera::Picking(btDiscreteDynamicsWorld* dynamicWorld, btDiscreteDynamicsWorld* staticWorld)
{
	//Thanks to Jan Vijfhuizen for helping me get the raycasting to work
	//Raycasting tutorial: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
	
	RayNDC();

	glm::mat4 matrix = glm::inverse(m_ProjectionMatrix * m_ViewMatrix);
	glm::vec4 rayStartWorld = matrix * m_RayStartNDC; rayStartWorld /= rayStartWorld.w;
	glm::vec4 rayEndWorld = matrix * m_RayEndNDC; rayEndWorld /= rayEndWorld.w;

	glm::vec3 rayDir(rayEndWorld - rayStartWorld);
	rayDir = glm::normalize(rayDir);

	glm::vec3 rayStart = static_cast<glm::vec3>(rayStartWorld);
	glm::vec3 rayEnd = rayStart + rayDir * 1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(rayStart.x, rayStart.y, rayStart.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z)
	);

	dynamicWorld->rayTest(
		btVector3(rayStart.x, rayStart.y, rayStart.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
		RayCallback
	);

	staticWorld->rayTest(
		btVector3(rayStart.x, rayStart.y, rayStart.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
		RayCallback
	);

	if (RayCallback.hasHit())
	{
		if (m_Input->m_Keys & (1 << 5)) //Left click
		{
			GameContext::Instance()->SetSelectedObject(static_cast<GameObject*>(RayCallback.m_collisionObject->getUserPointer()));
			m_Position = GameContext::Instance()->GetSelectedObject()->GetPosition() - m_Direction;
			m_ScrollPos = glm::vec3(0.0f, 0.0f, 0.0f);
			m_IsSelected = true;
		}

		GameContext::Instance()->SetHoverObject(static_cast<GameObject*>(RayCallback.m_collisionObject->getUserPointer()));
	}
	else
		GameContext::Instance()->SetHoverObject(nullptr);
}