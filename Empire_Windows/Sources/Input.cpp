#include "../Headers/Input.h"
#include <iostream>
#include "../Empire/ImGui/imgui.h"

static double yOffset = 0.0; //No otherway with glfw than to use static

void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//Add mousewheel scroll
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += (float)yoffset;

	yOffset = yoffset;
}

Input::Input() :
	m_Keys(0),
	m_MouseX(0.0f),
	m_MouseY(0.0f)
{
	glfwSetScrollCallback(RenderWindow::Instance()->m_Window, Scroll_Callback);
}

Input::~Input()
{}

void Input::GetInput()
{
	/* Keys */
	//W
	if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_W) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 1);
	else if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_W) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 1);

	//S
	if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_S) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 2);
	else if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_S) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 2);

	//D
	if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_D) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 3);
	else if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_D) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 3);

	//A
	if(glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_A) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 4);
	else if (glfwGetKey(RenderWindow::Instance()->m_Window, GLFW_KEY_A) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 4);

	/* Mouse */
	double xPos, yPos;
	glfwGetCursorPos(RenderWindow::Instance()->m_Window, &xPos, &yPos);
	m_MouseX = static_cast<float>(xPos);
	m_MouseY = static_cast<float>(yPos);

	//Left click
	if (glfwGetMouseButton(RenderWindow::Instance()->m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 5);
	else if (glfwGetMouseButton(RenderWindow::Instance()->m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 5);

	//Right click
	if (glfwGetMouseButton(RenderWindow::Instance()->m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		m_Keys = m_Keys | (1 << 6);
	else if (glfwGetMouseButton(RenderWindow::Instance()->m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		m_Keys = m_Keys & ~(1 << 6);

	//ScrollY
	if (yOffset > 0.0f)
		m_Keys = m_Keys | (1 << 7);
	else if (yOffset < 0.0f)
		m_Keys = m_Keys | (1 << 8);
	else
	{
		m_Keys = m_Keys & ~(1 << 7);
		m_Keys = m_Keys & ~(1 << 8);
	}
	yOffset = 0.0;
	
}