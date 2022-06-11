#include "Headers/GameObjects/GameObject.h"
#include "Headers/Renderer/VertexBuffer.h"

GameObject::GameObject() :
	m_IsActive(false),
	m_Owner(nullptr),
	m_BoxCollision(nullptr),
	m_MotionState(nullptr),
	m_RigidBody(nullptr)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Rotate(const glm::vec3& rotation)
{
	m_DegreeRotations += rotation;
	m_Rotations.x = glm::radians(m_DegreeRotations.x);
	m_Rotations.y = glm::radians(m_DegreeRotations.y);
	m_Rotations.z = glm::radians(m_DegreeRotations.z);

	MakeRotationMatrix();
	MakeModelMatrix();
}

void GameObject::SetPosition(const glm::vec3& pos)
{
	m_WorldPosition = pos;

	MakeTranslationMatrix();
	MakeModelMatrix();
}

void GameObject::Scale(const glm::vec3& scales)
{
	m_Scales = scales;
	m_ScaleMatrix = glm::mat4(1.0f);
	m_ScaleMatrix = glm::scale(m_ScaleMatrix, m_Scales);

	MakeScaleMatrix();
	MakeModelMatrix();
}

void GameObject::InitMatrices()
{
	m_TranslationMatrix = glm::mat4(1.0f);
	m_RotationMatrix = glm::mat4(1.0f);
	m_ScaleMatrix = glm::mat4(1.0f);

	MakeModelMatrix();
}

void GameObject::MakeTranslationMatrix()
{
	m_TranslationMatrix = glm::mat4(1.0f);
	m_TranslationMatrix = glm::translate(m_TranslationMatrix, m_WorldPosition);
}

void GameObject::MakeRotationMatrix()
{
	m_RotationMatrix = glm::mat4(1.0f);
	m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Rotations.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Rotations.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_RotationMatrix = glm::rotate(m_RotationMatrix, m_Rotations.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void GameObject::MakeScaleMatrix()
{
	m_ScaleMatrix = glm::mat4(1.0f);
	m_ScaleMatrix = glm::scale(m_ScaleMatrix, { m_Scales.x, m_Scales.y, m_Scales.z });
}

void GameObject::MakeModelMatrix()
{
	m_ModelMatrix = m_TranslationMatrix * m_RotationMatrix * m_ScaleMatrix;
}

void GameObject::SetActive(bool active)
{
	m_IsActive = active;
	if (m_IsActive)
		InitBullet();
}

void GameObject::InitBullet()
{
	//Bullet followed from: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z));

	m_MotionState = new btDefaultMotionState(groundTransform);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		0, //no mass
		m_MotionState,
		m_BoxCollision,
		btVector3(0, 0, 0)
	);

	m_RigidBody = new btRigidBody(rigidBodyCI);
	m_RigidBody->setUserPointer(this);
}