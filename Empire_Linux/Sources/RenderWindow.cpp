#include "../Headers/RenderWindow.h"

RenderWindow* RenderWindow::Instance()
{
	static RenderWindow* s_Instance = new RenderWindow();
	return s_Instance;
}

RenderWindow::RenderWindow()
{}

RenderWindow::~RenderWindow()
{
	XDestroyWindow(m_XDisplay, m_XWindow);
	XFree(m_XScreen);
	XCloseDisplay(m_XDisplay);
}

void RenderWindow::Init(int width, int height)
{
	setenv("DISPLAY", ":0", 1); //Thanks to Abdulla for mentioning this
	m_XDisplay = XOpenDisplay(NULL);
	if (m_XDisplay == NULL)
		printf("Failed to initialize XDisplay");

	m_XScreen = DefaultScreenOfDisplay(m_XDisplay);
	if (m_XScreen == NULL)
		printf("Failed to initialize XScreen");

	m_XScreenID = DefaultScreen(m_XDisplay);
	//m_XRootWindow = RootWindowOfScreen(m_XScreen);
	m_XRootWindow = DefaultRootWindow(m_XDisplay);
	if (m_XRootWindow == 0)
		printf("Failed to initialize XRootWindow");

	XSetWindowAttributes windowAttribs;
	windowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;
	windowAttribs.border_pixel = 0;
	windowAttribs.background_pixel = 0;
	windowAttribs.background_pixmap = None;
	windowAttribs.override_redirect = True;

	m_XWindow = XCreateWindow(m_XDisplay, m_XRootWindow, 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &windowAttribs);
	if (m_XWindow == 0)
		printf("Failed to initialize XWindow");

	//m_EGLNativeWindow = (EGLNativeWindowType)m_XWindow;

	XWMHints xHints;
	XEvent xEvent;
	Atom wm_state;

	//xAttributes.override_redirect = false;
	//XChangeWindowAttributes(m_XDisplay, m_XWindow, CWOverrideRedirect, &xAttributes);

	xHints.input = GL_TRUE;
	xHints.flags = InputHint;
	XSetWMHints(m_XDisplay, m_XWindow, &xHints);

	char* title = (char*)"Empires";
	XClearWindow(m_XDisplay, m_XWindow);
	XMapWindow(m_XDisplay, m_XWindow);
	XMapRaised(m_XDisplay, m_XWindow);
	XStoreName(m_XDisplay, m_XWindow, title);

	wm_state = XInternAtom(m_XDisplay, "_NET_WM_STATE", GL_FALSE);

	memset(&xEvent, 0, sizeof(xEvent));
	xEvent.type = ClientMessage;
	xEvent.xclient.window = m_XWindow;
	xEvent.xclient.message_type = wm_state;
	xEvent.xclient.format = 32;
	xEvent.xclient.data.l[0] = 1;
	xEvent.xclient.data.l[1] = GL_FALSE;
	XSendEvent(
		m_XDisplay,
		DefaultRootWindow(m_XDisplay),
		GL_FALSE,
		SubstructureNotifyMask,
		&xEvent
	);

	InitEGL(width, height);
	PrintResolutionOptions();
}

void RenderWindow::InitEGL(int width, int height)
{
	EGLint majorVersion, minorVersion, numConfigs;

	m_EGLDisplay = eglGetDisplay((EGLNativeDisplayType)m_XDisplay);
	if (m_EGLDisplay == EGL_NO_DISPLAY)
	{
		printf("Failed to initialize EGL Display\n");
		exit(0);
	}

	if (!eglInitialize(m_EGLDisplay, &majorVersion, &minorVersion))
	{
		EGLint err = eglGetError();
		printf("Failed to initialize EGL: %i", err);
		exit(err);
	}

	if (!eglGetConfigs(m_EGLDisplay, NULL, 0, &numConfigs))
	{
		EGLint err = eglGetError();
		printf("Failed to initialize EGL configs: %i", err);
		exit(err);
	}

	if (!eglChooseConfig(m_EGLDisplay, EGLAttributes, &m_EGLConfig, 1, &numConfigs))
	{
		EGLint err = eglGetError();
		printf("Failed to choose config: %i", err);
		exit(err);
	}

	m_EGLContext = eglCreateContext(m_EGLDisplay, m_EGLConfig, NULL, EGL_GLES3);

	if (m_EGLContext == EGL_NO_CONTEXT)
	{
		EGLint err = eglGetError();
		printf("Failed to initialize EGL context: %i", err);
		exit(err);
	}

	m_EGLSurface = eglCreateWindowSurface(m_EGLDisplay, m_EGLConfig, m_XWindow, NULL);

	if (m_EGLSurface == EGL_NO_SURFACE)
	{
		EGLint err = eglGetError();
		printf("Failed to initialize EGL surface: %i", err);
		exit(err);
	}

	if (!eglMakeCurrent(m_EGLDisplay, m_EGLSurface, m_EGLSurface, m_EGLContext))
	{
		EGLint err = eglGetError();
		printf("Failed to set the current EGL context: %i", err);
		exit(err);
	}

	// 0 for immediate swap (may cause tearing), 1 to lock speed to 60fps
	eglSwapInterval(m_EGLDisplay, 0);

	printf("This SBC supports version %i.%i of EGL\n", majorVersion, minorVersion);
}

void RenderWindow::PrintResolutionOptions()
{
	// Get possible resolutions
	xrrs = XRRSizes(m_XDisplay, 0, &m_NumSizes);

	for (int i = 0; i < m_NumSizes; i++) {
		short* rates;
		printf("\n\t%2i : %4i x %4i   (%4imm x%4imm ) ", i, xrrs[i].width, xrrs[i].height, xrrs[i].mwidth, xrrs[i].mheight);
		rates = XRRRates(m_XDisplay, 0, i, &m_NumSizes);

		for (int j = 0; j < m_NumRates; j++) {
			m_PossibleFrequencies[i][j] = rates[j];
			printf("%4i ", rates[j]);
		}
	}

	printf("\n");
}

glm::vec2 RenderWindow::GetWindowSize()
{
	XWindowAttributes attribs;
	XGetWindowAttributes(RenderWindow::Instance()->m_XDisplay, RenderWindow::Instance()->m_XWindow, &attribs);

	return glm::vec2(attribs.width, attribs.height);
}
