#include "../Empire/Headers/Precomp.h"
#include "../Empire/Headers/GameContext.h"
#include "../Empire/Headers/Timer.h"
#include "../Headers/RenderWindow.h"

int main()
{
	Timer* timer = Timer::Instance();
	float elapsed = 0.0f;
	unsigned int frames = 0;

	//Init screen
	GameContext::Instance()->Init(SCREENWIDTH, SCREENHEIGHT);

	while (!GameContext::Instance()->GetCloseProgram())
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

		//Input
		GameContext::Instance()->Update(timer->DeltaTime());
		GameContext::Instance()->Render();

		glFlush();
		eglSwapBuffers(RenderWindow::Instance()->m_EGLDisplay, RenderWindow::Instance()->m_EGLSurface);
	}

	timer->Release();

	return 0;
}