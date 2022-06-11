#pragma once
#include "Headers/Precomp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RenderWindow
{
public:
	RenderWindow(const RenderWindow& rw) = delete;
	RenderWindow& operator=(const RenderWindow& rw) = delete;
	static RenderWindow* Instance();

	void Init(int width, int height);
	glm::vec2 GetWindowSize();

	GLFWwindow* m_Window;

private:
	RenderWindow();
	~RenderWindow();

};
