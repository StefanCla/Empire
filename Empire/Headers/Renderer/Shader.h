#pragma once
#include "Headers/Precomp.h"

enum class ShaderType
{
	Default, Phong, Skybox
};

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	unsigned int GetAttribLocation(const std::string& attrib) const;
	unsigned int GetUniformLocation(const std::string& uniform) const;
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	unsigned int GetRendererID() const { return m_RendererID; }

private:
	ShaderProgramSource Parse(const std::string& vertexPath, const std::string& fragmentPath);
	unsigned int Compile(unsigned int type, const std::string& source);
	unsigned int CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);

	unsigned int m_RendererID;
};
