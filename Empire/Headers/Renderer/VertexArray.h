#pragma once
#include "Headers/Precomp.h"
#include "Headers/Renderer/VertexBuffer.h"
#include <memory>
#include <map>

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(VertexBufferType type, unsigned int programID, const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	const std::shared_ptr<VertexBuffer>& GetVertexBuffer(VertexBufferType type) const;
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

private:
	unsigned int m_RendererID;

	std::map<VertexBufferType, std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
