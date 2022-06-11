#pragma once
#include "Headers/Precomp.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "Headers/GameObjects/GameObject.h"

#ifdef LINUX
#include "../Empire_Linux/Headers/RenderWindow.h"
#include "../Empire_Linux/Headers/Input.h"
#elif WINDOWS
#include "../Empire_Windows/Headers/Input.h"
#include "../Empire_Windows/Headers/RenderWindow.h"
#endif

class Camera
{
public:

	Camera(const glm::vec3& pos, Input* input);
	~Camera();

	void Update(float dt);

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(glm::vec3 pos) { m_Position = pos; }
	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	void Picking(btDiscreteDynamicsWorld* dynamicWorld, btDiscreteDynamicsWorld* staticWorld);

private:
	void MakeViewProjectionMatrix();
	void RayNDC();

	glm::vec4 m_RayStartNDC;
	glm::vec4 m_RayEndNDC;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ViewProjectionMatrix;

	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_TargetPosition;
	glm::vec3 m_Up;
	glm::vec3 m_ScrollPos;

	glm::vec2 m_DragCenter;

	float m_Rotation = 0.0f;
	float m_Speed = 30.0f;
	bool m_InDrag = false;

	Input* m_Input;

	//Selecting
	bool m_IsSelected;
};
