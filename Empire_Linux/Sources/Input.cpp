#include "../Headers/Input.h"
#include "../Empire/Headers/Renderer/Renderer.h"
#include "../Empire/ImGui/imgui.h"

Input::Input() :
	m_Keys(0),
	m_MouseX(0.0f),
	m_MouseY(0.0f),
	m_Root_X(0),
	m_Root_Y(0),
	m_Mask_Return(0),
	m_Window_Returned(0)
{}

Input::~Input()
{}

void Input::GetInput()
{
	ImGuiIO& io = ImGui::GetIO();

	//Reset scroll wheel | Thanks to Abdulla Donmez for letting me know
	m_Keys = m_Keys & ~(1 << 7);
	m_Keys = m_Keys & ~(1 << 8);

	/* Keys */
	while (XPending(RenderWindow::Instance()->m_XDisplay) > 0)
	{
		XNextEvent(RenderWindow::Instance()->m_XDisplay, &m_Xevent);

		KeySym key = XKeycodeToKeysym(RenderWindow::Instance()->m_XDisplay, (KeyCode)m_Xevent.xkey.keycode, 0);

		switch (m_Xevent.type)
		{
		case Expose:
		{
			XWindowAttributes attribs;
			XGetWindowAttributes(RenderWindow::Instance()->m_XDisplay, RenderWindow::Instance()->m_XWindow, &attribs);
			Renderer::Instance()->ResizeViewport(attribs.width, attribs.height);

			//ImGui
			io.DisplaySize.x = attribs.width;
			io.DisplaySize.y = attribs.height;
		}
		break;
		case KeyPress:
		{
			if (key == XK_w)				//W
				m_Keys = m_Keys | (1 << 1);
			if (key == XK_s)				//S
				m_Keys = m_Keys | (1 << 2);
			if (key == XK_d)				//D
				m_Keys = m_Keys | (1 << 3);
			if (key == XK_a)				//A
				m_Keys = m_Keys | (1 << 4);

			io.AddInputCharacter(key);
		}
		break;
		case KeyRelease:
		{
			if (key == XK_w)				//W
				m_Keys = m_Keys & ~(1 << 1);
			if (key == XK_s)				//S
				m_Keys = m_Keys & ~(1 << 2);
			if (key == XK_d)				//D
				m_Keys = m_Keys & ~(1 << 3);
			if (key == XK_a)				//A
				m_Keys = m_Keys & ~(1 << 4);
		}
		break;
		case ButtonPress:
		{
			if (m_Xevent.xbutton.button == Button4) //Scroll Up
			{
				m_Keys = m_Keys | (1 << 7);
				io.MouseWheel += 0.3f;
			}
			else if (m_Xevent.xbutton.button == Button5) //Scroll Down
			{
				m_Keys = m_Keys | (1 << 8);
				io.MouseWheel -= 0.3f;
			}
		}
			break;
		case KeymapNotify:
			XRefreshKeyboardMapping(&m_Xevent.xmapping);
			break;
		}
	}

	/* Mouse */
	int winX, winY;

	XQueryPointer(
		RenderWindow::Instance()->m_XDisplay,
		RenderWindow::Instance()->m_XWindow,
		&m_Window_Returned,
		&m_Window_Returned,
		&m_Root_X,
		&m_Root_Y,
		&winX,
		&winY,
		&m_Mask_Return
	);

	m_MouseX = static_cast<float>(winX);
	m_MouseY = static_cast<float>(winY);

	//Add mouse buttons to keys
	//Left click
	if (m_Mask_Return & (1 << 8))
		m_Keys = m_Keys | (1 << 5);
	else
		m_Keys = m_Keys & ~(1 << 5);

	//Right click
	if (m_Mask_Return & (1 << 10))
		m_Keys = m_Keys | (1 << 6);
	else
		m_Keys = m_Keys & ~(1 << 6);

	io.MousePos = ImVec2(m_MouseX, m_MouseY);
	io.MouseDown[0] = m_Keys & (1 << 5);
	io.MouseDown[1] = m_Keys & (1 << 6);
	io.MouseDrawCursor = false;

}
