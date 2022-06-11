#include "Headers/Renderer/Shader.h"
#include "Headers/FileHandler.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	ShaderProgramSource source = Parse(vertexPath, fragmentPath);
	m_RendererID = CreateProgram(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

unsigned int Shader::GetAttribLocation(const std::string& attrib) const
{
	return glGetAttribLocation(m_RendererID, attrib.c_str());
}

unsigned int Shader::GetUniformLocation(const std::string& uniform) const
{
	return glGetUniformLocation(m_RendererID, uniform.c_str());
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	unsigned int location = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

ShaderProgramSource Shader::Parse(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexSource;
	std::string fragmentSource;

	FileHandler::Instance()->LoadText(vertexPath.c_str(), vertexSource);
	FileHandler::Instance()->LoadText(fragmentPath.c_str(), fragmentSource);

	return { vertexSource, fragmentSource };
}

unsigned int Shader::Compile(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		printf("Failed to compile %s shader\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
		printf("%s\n", message);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int program = glCreateProgram();
	unsigned int vertexShader = Compile(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = Compile(GL_FRAGMENT_SHADER, fragmentSource);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint RetinfoLen = 0;
		// check and report any errors
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &RetinfoLen);
		if (RetinfoLen > 1)
		{
			GLchar* infoLog = (GLchar*)malloc(sizeof(char) * RetinfoLen);
			glGetProgramInfoLog(program, RetinfoLen, NULL, infoLog);
			fprintf(stderr, "Error linking program:\n%s\n", infoLog);
			free(infoLog);
		}
	}
	glValidateProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
