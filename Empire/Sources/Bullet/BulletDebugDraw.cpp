#include "Headers/Bullet/BulletDebugDraw.h"
#include "Headers/GameContext.h"
#include "Headers/Camera.h"
#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/Shader.h"

BulletDebugDraw* BulletDebugDraw::Instance()
{
	static BulletDebugDraw* s_Instance = new BulletDebugDraw();
	return s_Instance;
}

BulletDebugDraw::BulletDebugDraw()
{
}

BulletDebugDraw::~BulletDebugDraw()
{
	delete m_DebugShader;
	delete m_LineArray;
}

void BulletDebugDraw::Init()
{
#ifdef LINUX
	m_DebugShader = new Shader("../../../Empire/Resources/Shaders/BtDebug.vert", "../../../Empire/Resources/Shaders/BtDebug.frag");
#elif WINDOWS
	m_DebugShader = new Shader("../Empire/Resources/Shaders/BtDebug.vert", "../Empire/Resources/Shaders/BtDebug.frag");
#endif
	m_LineArray = new VertexArray();

	m_LineBuffer = std::make_shared<VertexBuffer>(6 * sizeof(float));
	m_LineBuffer->SetLayout({
		{ ShaderDataType::Float3, "in_position" }
		});

	m_LineArray->AddVertexBuffer(VertexBufferType::ModelVB, m_DebugShader->GetRendererID(), m_LineBuffer);
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	if (m_DebugMode > 0)
	{
		m_LineArray->Bind();
		m_DebugShader->Bind();

		m_DebugShader->SetUniformMat4("u_viewProj", GameContext::Instance()->GetCamera()->GetViewProjectionMatrix());

		float lineData[6] = { from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ() };
		m_LineBuffer->SetBufferData(&lineData[0], 6 * sizeof(float));

		glDrawArrays(GL_LINES, 0, 2);

		m_LineArray->Unbind();
		m_DebugShader->Unbind();
	}
}

void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	if (m_DebugMode > 0)
		printf("%s\n", warningString);
}

void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}
