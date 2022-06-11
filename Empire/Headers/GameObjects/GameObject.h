#pragma once
#include "Headers/Precomp.h"
#include <memory>
#include "Headers/Models/Model.h"
#include "Headers/Renderer/Shader.h"
#include "Headers/Textures/Texture.h"
#include <bullet/btBulletDynamicsCommon.h>

class EmpireController;

enum class ObjectType 
{
	DV, SV, CV, Planet
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Init();
	virtual void Update(float dt) = 0;
	virtual void InitBullet();

	const glm::vec3& GetPosition() const { return m_WorldPosition; }
	void SetPosition(const glm::vec3& pos);

	void Rotate(const glm::vec3& rotation);
	void Scale(const glm::vec3& scales);

	const glm::vec3& GetColor() const { return m_Color; }
	const glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
	const bool GetActive() const { return m_IsActive; }
	void SetActive(bool active);
	const std::shared_ptr<Model>& GetModel() const { return m_Model; }
	btRigidBody* GetRigidBody() const { return m_RigidBody; }
	ObjectType GetObjectType() const { return m_Type; }
	EmpireController* GetOwner() const { return m_Owner; }
	void SetOwner(EmpireController* owner) { m_Owner = owner; }

protected:
	void InitMatrices();
	void MakeTranslationMatrix();
	void MakeRotationMatrix();
	void MakeScaleMatrix();
	void MakeModelMatrix();

	EmpireController* m_Owner;
	ObjectType m_Type;
	bool m_IsActive;

	std::shared_ptr<Model> m_Model;

	glm::vec3 m_WorldPosition;

	glm::vec3 m_Rotations;
	glm::vec3 m_DegreeRotations;
	glm::vec3 m_Scales;

	glm::mat4 m_XRotationMatrix;
	glm::mat4 m_YRotationMatrix;
	glm::mat4 m_ZRotationMatrix;

	glm::mat4 m_TranslationMatrix;
	glm::mat4 m_RotationMatrix;
	glm::mat4 m_ScaleMatrix;

	glm::mat4 m_ModelMatrix;

	glm::vec3 m_Color;

	//Bullet
	btCollisionShape* m_BoxCollision;
	btDefaultMotionState* m_MotionState;
	btRigidBody* m_RigidBody;
};
