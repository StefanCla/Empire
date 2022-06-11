#pragma once
#include "../Empire/Headers/Precomp.h"
#include "../Headers/RenderWindow.h"

class Input
{
public:
	Input();
	~Input();
	void GetInput();
	
	int m_Keys; //Also includes mouse
	float m_MouseX;
	float m_MouseY;

private:
	XEvent m_Xevent;
	uint m_Mask_Return;
	Window m_Window_Returned;
	int m_Root_X;
	int m_Root_Y;

};