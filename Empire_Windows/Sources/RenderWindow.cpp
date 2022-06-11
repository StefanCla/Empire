#include "../Headers/RenderWindow.h"
#include <stdlib.h>
#include <stdio.h>

//Reference:
//https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp
//https://learnopengl.com/Getting-started/Hello-Window
//https://www.glfw.org/docs/latest/quick.html

void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Error_Callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

RenderWindow* RenderWindow::Instance()
{
	static RenderWindow* s_Instance = new RenderWindow();
	return s_Instance;
}

RenderWindow::RenderWindow()
{}

RenderWindow::~RenderWindow()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void RenderWindow::Init(int width, int height)
{
	glfwSetErrorCallback(Error_Callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, "Empire", NULL, NULL);
	if (!m_Window)
	{
		printf("Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(m_Window, Framebuffer_Size_Callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}
}

glm::vec2 RenderWindow::GetWindowSize()
{
	int width, height;
	glfwGetWindowSize(m_Window, &width, &height);

	return glm::vec2(width, height);
}