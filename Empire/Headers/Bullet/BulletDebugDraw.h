#pragma once
#include "Headers/Precomp.h"
#include <bullet/btBulletCollisionCommon.h>

class VertexArray;
class VertexBuffer;
class Shader;

class BulletDebugDraw : public btIDebugDraw
{
public:
	BulletDebugDraw(const BulletDebugDraw& bdd) = delete;
	BulletDebugDraw& operator=(const BulletDebugDraw& bdd) = delete;

	static BulletDebugDraw* Instance();

	void Init();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void reportErrorWarning(const char* warningString) override;
	virtual void draw3dText(const btVector3& location, const char* textString) override;
	virtual void setDebugMode(int debugMode) override { m_DebugMode = debugMode; }
	virtual int	getDebugMode() const override { return m_DebugMode; }

private:
	BulletDebugDraw();
	~BulletDebugDraw();

	VertexArray* m_LineArray;
	std::shared_ptr<VertexBuffer> m_LineBuffer;
	Shader* m_DebugShader;

	int m_DebugMode = 0;
};
