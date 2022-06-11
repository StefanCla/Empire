//Windows
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Headers/RenderWindow.h"

//Shared project
#include "../Empire/Headers/Precomp.h"
#include "../Empire/Headers/Timer.h"
#include "../Empire/Headers/GameContext.h"

#include <stdlib.h>
#include <stdio.h>

//Windows bullet fix:
//https://pybullet.org/Bullet/phpBB3/viewtopic.php?f=9&t=12716&p=42258&hilit=malloc#p42258

int main()
{
	Timer* timer = Timer::Instance();
	float elapsed = 0.0f;
	unsigned int frames = 0;

	//Init screen
	GameContext::Instance()->Init(SCREENWIDTH, SCREENHEIGHT);

	while (!glfwWindowShouldClose(RenderWindow::Instance()->m_Window) && !GameContext::Instance()->GetCloseProgram())
	{
		timer->Tick();
		timer->Reset();
		elapsed += timer->DeltaTime();
		frames++;

		if (elapsed >= 1.0f)
		{
			printf("Average frame time: %f | FPS: %i\n", elapsed / static_cast<float>(frames), frames);

			elapsed = 0.0f;
			frames = 0;
		}

		GameContext::Instance()->Update(timer->DeltaTime());
		GameContext::Instance()->Render();

		glfwSwapBuffers(RenderWindow::Instance()->m_Window);
		glfwPollEvents();
	}

	return 0;
}