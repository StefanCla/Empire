#pragma once
#include "Headers/Precomp.h"

enum class VertexBufferType
{
	ModelVB, ColorVB, PositionVB, RadiusVB, ModelMatrixVB
};

enum class ShaderDataType
{
	Null = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Boolean
};

static unsigned int ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:   return 4;
	case ShaderDataType::Float2:  return 4 * 2;
	case ShaderDataType::Float3:  return 4 * 3;
	case ShaderDataType::Float4:  return 4 * 4;
	case ShaderDataType::Mat3:    return 4 * 3 * 3;
	case ShaderDataType::Mat4:    return 4 * 4 * 4;
	case ShaderDataType::Int:     return 4;
	case ShaderDataType::Int2:    return 4 * 2;
	case ShaderDataType::Int3:    return 4 * 3;
	case ShaderDataType::Int4:    return 4 * 4;
	case ShaderDataType::Boolean: return 1;
	default:					  assert(false && "Invalid ShaderDataType in ShaderDataTypeSize() of VertexBuffer\n"); break;
	}

	return 0;
}

struct BufferElement
{
public:
	std::string Name;
	ShaderDataType Type;
	unsigned int Size;
	unsigned int Offset;
	bool Normalized;

	BufferElement() {}
	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

	unsigned int GetComponentCount() const
	{
		switch (Type)
		{
		case ShaderDataType::Float:	  return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat3:	  return 4 * 3;
		case ShaderDataType::Mat4:	  return 4 * 4;
		case ShaderDataType::Int:	  return 1;
		case ShaderDataType::Int2:	  return 2;
		case ShaderDataType::Int3:	  return 3;
		case ShaderDataType::Int4:	  return 4;
		case ShaderDataType::Boolean: return 1;
		default:					  assert(false && ("Invalid ShaderDataType in GetComponentCount() of VertexBuffer\n")); break;
		}

		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {}
	BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateOffsetAndStride()
	{
		unsigned int offset = 0;
		m_Stride = 0;
		for (BufferElement& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	std::vector<BufferElement> m_Elements;
	unsigned int m_Stride = 0;
};

class VertexBuffer
{
public:
	VertexBuffer(unsigned int size);
	VertexBuffer(float* vertices, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBufferData(float* data, unsigned int size);

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

private:
	unsigned int m_RendererID;
	BufferLayout m_Layout;
};