#pragma once
#include "../Empire/Headers/Precomp.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <stdio.h>
#include <cstring>
#include <stdlib.h>

static const EGLint EGLAttributes[] =
{
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_ALPHA_SIZE, 8,
	EGL_DEPTH_SIZE,	8,
	EGL_SURFACE_TYPE,
	EGL_WINDOW_BIT,
#ifdef GLES3
	EGL_CONFORMANT,
	EGL_OPENGL_ES3_BIT_KHR,
#endif
	// Those two attributes enable anti-aliasing but heavily reduce frames
	// EGL_SAMPLE_BUFFERS, 1,
	// EGL_SAMPLES, 4,
	EGL_NONE
};

static const EGLint EGL_GLES3[] = {
	EGL_CONTEXT_MAJOR_VERSION_KHR,
	3,
	EGL_CONTEXT_MINOR_VERSION_KHR,
	1,
	EGL_NONE,
	EGL_NONE
};

class RenderWindow
{
public:
	Display* m_XDisplay;
	Window m_XWindow;
	EGLDisplay m_EGLDisplay;
	EGLSurface m_EGLSurface;

	RenderWindow(const RenderWindow& rw) = delete;
	RenderWindow& operator=(const RenderWindow& rw) = delete;

	static RenderWindow* Instance();

	void Init(int width, int height);
	glm::vec2 GetWindowSize();

private:
	RenderWindow();
	~RenderWindow();

	void InitEGL(int width, int height);
	void PrintResolutionOptions();

	Screen* m_XScreen;
	int m_XScreenID;
	Window m_XRootWindow;

	XRRScreenSize* xrrs;
	short m_PossibleFrequencies[64][64];
	int m_NumSizes;
	int m_NumRates;

	EGLContext m_EGLContext;
	EGLNativeWindowType m_EGLNativeWindow;
	EGLConfig m_EGLConfig;
};
