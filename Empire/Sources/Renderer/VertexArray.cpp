#include "Headers/Renderer/VertexArray.h"
#include "Headers/Renderer/VertexBuffer.h"
#include "Headers/Renderer/IndexBuffer.h"
#include "Headers/Renderer/Shader.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:  return GL_FLOAT;
	case ShaderDataType::Float2: return GL_FLOAT;
	case ShaderDataType::Float3: return GL_FLOAT;
	case ShaderDataType::Float4: return GL_FLOAT;
	case ShaderDataType::Mat3:   return GL_FLOAT;
	case ShaderDataType::Mat4:   return GL_FLOAT;
	case ShaderDataType::Int:    return GL_INT;
	case ShaderDataType::Int2:   return GL_INT;
	case ShaderDataType::Int3:   return GL_INT;
	case ShaderDataType::Int4:   return GL_INT;
	case ShaderDataType::Boolean:   return GL_BOOL;
	default:					 assert(false && "Invalid ShaderDataType in ShaderDataTypeToOpenGLBaseType() of VertexArray\n"); break;
	}

	return 0;
}

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(VertexBufferType type, unsigned int programID, const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	const auto& layout = vertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		int attribLoc = glGetAttribLocation(programID, element.Name.c_str());
		//assert(attribLoc >= 0 && "Attribute location was not found\n");
		if (element.Type == ShaderDataType::Mat4)
		{
			for (int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(attribLoc + i);
				glVertexAttribPointer(attribLoc + i, 4, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(i * sizeof(glm::vec4)));
				if (type != VertexBufferType::ModelVB)
					glVertexAttribDivisor(attribLoc + i, 1);
			}
		}
		else
		{
			glEnableVertexAttribArray(attribLoc);
			glVertexAttribPointer(attribLoc, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			if (type != VertexBufferType::ModelVB)
				glVertexAttribDivisor(attribLoc, 1);
		}
	}

	m_VertexBuffers.insert(std::make_pair(type, vertexBuffer));
}

const std::shared_ptr<VertexBuffer>& VertexArray::GetVertexBuffer(VertexBufferType type) const
{
	std::map<VertexBufferType, std::shared_ptr<VertexBuffer>>::const_iterator it = m_VertexBuffers.find(type);

	if (it != m_VertexBuffers.end())
		return it->second;
	else
		return nullptr;
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}
