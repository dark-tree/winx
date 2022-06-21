
 /*
  * Copyright (c) 2022, Vitali Baumtrok and magistermaks
  *
  * Boost Software License - Version 1.0 - August 17th, 2003
  *
  * Permission is hereby granted, free of charge, to any person or organization
  * obtaining a copy of the software and accompanying documentation covered by
  * this license (the "Software") to use, reproduce, display, distribute,
  * execute, and transmit the Software, and to prepare derivative works of the
  * Software, and to permit third-parties to whom the Software is furnished to
  * do so, all subject to the following:
  *
  * The copyright notices in the Software and this entire statement, including
  * the above license grant, this restriction and the following disclaimer,
  * must be included in all copies of the Software, in whole or in part, and
  * all derivative works of the Software, unless such copies or derivative
  * works are solely in the form of machine-executable object code generated by
  * a source language processor.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
  * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
  * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
  * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  * DEALINGS IN THE SOFTWARE.
  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef void (*WinxMouseEventHandle)(int, int);
typedef void (*WinxButtonEventHandle)(int, int);
typedef void (*WinxKeybordEventHandle)(int, int);
typedef void (*WinxScrollEventHandle)(int);
typedef void (*WinxCloseEventHandle)(void);

/// set a window hint, has to be called prior to winxOpen()
void winxHint(int hint, int value);

/// return and clear last winx error, or NULL if there was no error
char* winxGetError();

/// used to open new window
bool winxOpen(int width, int height, const char* title);

/// process pending events
void winxPollEvents();

/// swap display buffers
void winxSwapBuffers();

/// used to close window
void winxClose();

/// set window title
void winxSetTitle(const char* title);

/// set the handle for mouse movement events, requires an open window
void winxSetMouseHandle(WinxMouseEventHandle handle);

/// set the handle for mouse click events, requires an open window
void winxSetButtonEventHandle(WinxButtonEventHandle handle);

/// set the handle for keybord events, requires an open window
void winxSetKeybordEventHandle(WinxKeybordEventHandle handle);

/// set the handle for mouse scroll, requires an open window
void winxSetScrollEventHandle(WinxScrollEventHandle handle);

/// set the handle for window close button, requires an open window
void winxSetCloseEventHandle(WinxCloseEventHandle handle);

#define WINX_PRESSED 1
#define WINX_RELEASED 0

/// hint keys
#define WINX_HINT_VSYNC         0x01
#define WINX_HINT_RED_BITS      0x02
#define WINX_HINT_GREEN_BITS    0x03
#define WINX_HINT_BLUE_BITS     0x04
#define WINX_HINT_ALPHA_BITS    0x05
#define WINX_HINT_DEPTH_BITS    0x06
#define WINX_HINT_OPENGL_MAJOR  0x07
#define WINX_HINT_OPENGL_MINOR  0x08
#define WINX_HINT_OPENGL_CORE   0x09
#define WINX_HINT_OPENGL_DEBUG  0x0A
#define WINX_HINT_OPENGL_ROBUST 0x0B
#define WINX_HINT_MULTISAMPLES  0x0C

/// hint values
#define WINX_VSYNC_DISABLED 0
#define WINX_VSYNC_ENABLED 1

#if defined(__unix__) || defined(__linux__)
#	define WINX_GLX

#	define WXK_SPACE ' '
#	define WXK_TAB '\t'
#	define WXK_ESC XK_Escape
#	define WXK_ENTER XK_Return
#	define WXK_BACK XK_BackSpace
#	define WXK_UP XK_Up
#	define WXK_DOWN XK_Down
#	define WXK_LEFT XK_Left
#	define WXK_RIGHT XK_Right
#	define WXB_LEFT Button1
#	define WXB_CENTER Button2
#	define WXB_RIGHT Button3

#elif defined(_WIN32) || defined(_WIN64)
#	define WINX_WINAPI

#	define WXK_SPACE ' '
#	define WXK_TAB '\t'
#	define WXK_ESC VK_ESCAPE
#	define WXK_ENTER VK_RETURN
#	define WXK_BACK VK_BACK
#	define WXK_UP VK_UP
#	define WXK_DOWN VK_DOWN
#	define WXK_LEFT VK_LEFT
#	define WXK_RIGHT VK_RIGHT
#	define WXB_LEFT 1
#	define WXB_CENTER 2
#	define WXB_RIGHT 3

#endif

#if defined(WINX_IMPLEMENT)

#define SET_HINT(HINT_ENUM, HINT_VAR) case HINT_ENUM: HINT_VAR = value; break;

// dummy functions
void WinxDummyMouseEventHandle(int x, int y) {}
void WinxDummyButtonEventHandle(int type, int button) {}
void WinxDummyKeybordEventHandle(int type, int key) {}
void WinxDummyScrollEventHandle(int scroll) {}
void WinxDummyCloseEventHandle() {}

// hints
int __winx_hint_vsync = 0;
int __winx_hint_red_bits = 8;
int __winx_hint_green_bits = 8;
int __winx_hint_blue_bits = 8;
int __winx_hint_alpha_bits = 8;
int __winx_hint_depth_bits = 24;
int __winx_hint_opengl_major = 3;
int __winx_hint_opengl_minor = 0;
int __winx_hint_opengl_core = 1;
int __winx_hint_opengl_debug = 0;
int __winx_hint_opengl_robust = 0;
int __winx_hint_multisamples = 0;

// current error message
char* __winx_msg = NULL;

char* winxGetError() {
	char* copy = __winx_msg;
	__winx_msg = NULL;

	return copy;
}

void winxHint(int hint, int value) {
	switch (hint) {
		SET_HINT(WINX_HINT_VSYNC, __winx_hint_vsync);
		SET_HINT(WINX_HINT_RED_BITS, __winx_hint_red_bits);
		SET_HINT(WINX_HINT_GREEN_BITS,	__winx_hint_green_bits);
		SET_HINT(WINX_HINT_BLUE_BITS, __winx_hint_blue_bits);
		SET_HINT(WINX_HINT_ALPHA_BITS, __winx_hint_alpha_bits);
		SET_HINT(WINX_HINT_DEPTH_BITS, __winx_hint_depth_bits);
		SET_HINT(WINX_HINT_OPENGL_MAJOR, __winx_hint_opengl_major);
		SET_HINT(WINX_HINT_OPENGL_MINOR, __winx_hint_opengl_minor);
		SET_HINT(WINX_HINT_OPENGL_CORE, __winx_hint_opengl_core);
		SET_HINT(WINX_HINT_OPENGL_DEBUG, __winx_hint_opengl_debug);
		SET_HINT(WINX_HINT_OPENGL_ROBUST, __winx_hint_opengl_robust);
		SET_HINT(WINX_HINT_MULTISAMPLES, __winx_hint_multisamples);
	}
}

#undef SET_HINT

#endif // common

// begin winx GLX implementation
#if defined(WINX_GLX) && defined(WINX_IMPLEMENT)

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

// copied from glxext.h
typedef int ( *PFNGLXSWAPINTERVALMESAPROC) (unsigned int interval);
typedef GLXContext ( *PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display *dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA;
PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;

typedef struct {
	Display* display;
	Window window;
	GLXContext context;
	Atom WM_DELETE_WINDOW;

	WinxMouseEventHandle mouse;
	WinxButtonEventHandle button;
	WinxKeybordEventHandle keyboard;
	WinxScrollEventHandle scroll;
	WinxCloseEventHandle close;
} WinxHandle;

WinxHandle* winx = NULL;

__GLXextFuncPtr winxGetProc(char* name) {
	if (__winx_msg == NULL) {
		__GLXextFuncPtr proc = glXGetProcAddress(name);

		if (proc == NULL) {
			__winx_msg = "glXGetProcAddress: Failed to load function!";
		}

		return proc;
	}

	return NULL;
}

bool winxOpen(int width, int height, const char* title) {
	winx = (WinxHandle*) malloc(sizeof(WinxHandle));

	// set dummy function pointers
	winx->button = WinxDummyButtonEventHandle;
	winx->mouse = WinxDummyMouseEventHandle;
	winx->keyboard = WinxDummyKeybordEventHandle;
	winx->scroll = WinxDummyScrollEventHandle;
	winx->close = WinxDummyCloseEventHandle;

	// get display handle
	winx->display = XOpenDisplay(NULL);
	if (!winx->display) {
		__winx_msg = (char*) "XOpenDisplay: Failed to acquire display handle!";
		return false;
	}

	// GLX attributes
	int attributes[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
		GLX_RED_SIZE, __winx_hint_red_bits,
		GLX_GREEN_SIZE, __winx_hint_green_bits,
		GLX_BLUE_SIZE, __winx_hint_blue_bits,
		GLX_DEPTH_SIZE, __winx_hint_depth_bits,
		GLX_SAMPLE_BUFFERS, __winx_hint_multisamples ? 1 : 0,
		GLX_SAMPLES, __winx_hint_multisamples,
		GLX_STENCIL_SIZE, 1,
		None
	};

	int screen = DefaultScreen(winx->display);
	Window root = RootWindow(winx->display, screen);

	// find frame buffer config matching our attributes
	int count;
	GLXFBConfig *fbconfigs = glXChooseFBConfig(winx->display, screen, attributes, &count);
	if (!fbconfigs || count == 0) {
		__winx_msg = (char*) "glXChooseFBConfig: Failed to choose a frame buffer config!";
		return false;
	}

	// find visual based on framebuffer's config
	XVisualInfo* info = glXGetVisualFromFBConfig(winx->display, fbconfigs[0]);
	if (!info) {
		__winx_msg = (char*) "glXGetVisualFromFBConfig: Failed to choose a visual!";
		return false;
	}

	// set X11 window attributes
	XSetWindowAttributes x11_attributes;
	x11_attributes.background_pixel = 0;
	x11_attributes.border_pixel = 0;
	x11_attributes.colormap = XCreateColormap(winx->display, root, info->visual, AllocNone);
	x11_attributes.event_mask = StructureNotifyMask | ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

	unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	// finally create X11 window
	winx->window = XCreateWindow(winx->display, root, 0, 0, width, height, 0, info->depth, InputOutput, info->visual, mask, &x11_attributes);

	// set name
	winxSetTitle(title);

	// show X11 window
	XMapWindow(winx->display, winx->window);

	// create GLX context
	GLXContext context = glXCreateContext(winx->display, info, NULL, 1);
	if (!context) {
		__winx_msg = (char*) "glXCreateContext: Failed to create GLX context!";
		return false;
	}

	glXMakeCurrent(winx->display, winx->window, context);

	glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) winxGetProc("glXCreateContextAttribsARB");
	glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC) glXGetProcAddress("glXSwapIntervalMESA"); // optional

	if (__winx_msg != NULL) {
		return false;
	}

	glXMakeCurrent(winx->display, 0, 0);
    glXDestroyContext(winx->display, context);

	XFree(info);

	int flags = 0;

	if (__winx_hint_opengl_debug) flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
	if (__winx_hint_opengl_robust) flags |= GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB;

	int context_attributes[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, __winx_hint_opengl_major,
        GLX_CONTEXT_MINOR_VERSION_ARB, __winx_hint_opengl_minor,
		GLX_CONTEXT_PROFILE_MASK_ARB, __winx_hint_opengl_core ? GLX_CONTEXT_CORE_PROFILE_BIT_ARB : GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		GLX_CONTEXT_FLAGS_ARB, flags,
        None
    };

	winx->context = glXCreateContextAttribsARB(winx->display, fbconfigs[0], NULL, true, context_attributes);
    if (!winx->context) {
		__winx_msg = "glXCreateContextAttribsARB: Failed to create context";
        return false;
    }

    glXMakeCurrent(winx->display, winx->window, winx->context);

	// needed to handle the close button
	winx->WM_DELETE_WINDOW = XInternAtom(winx->display, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(winx->display, winx->window, &(winx->WM_DELETE_WINDOW), 1);

	// try setting vsync
	if (glXSwapIntervalMESA && __winx_hint_vsync) {
		glXSwapIntervalMESA(__winx_hint_vsync);
	}

	return true;
}

void winxPollEvents() {
	while( XPending(winx->display) > 0 ) {

		XEvent event;
		XNextEvent(winx->display, &event);

		switch (event.type) {

			case ClientMessage:
				if (event.xclient.data.l[0] == winx->WM_DELETE_WINDOW) {
					winx->close();
					break;
				}

			case KeyPress:
				winx->keyboard(WINX_PRESSED, XLookupKeysym(&event.xkey, 0));
				break;

			case KeyRelease:
				winx->keyboard(WINX_RELEASED, XLookupKeysym(&event.xkey, 0));
				break;

			case ButtonPress:
				if (event.xbutton.button == Button4) {
					winx->scroll(1);
					break;
				}

				if (event.xbutton.button == Button5) {
					winx->scroll(-1);
					break;
				}

				winx->button(WINX_PRESSED, event.xbutton.button);
				break;

			case ButtonRelease:
				winx->button(WINX_RELEASED, event.xbutton.button);
				break;

			case MotionNotify:
				winx->mouse(event.xmotion.x_root, event.xmotion.y_root);
				break;

			default:
				break;

		}

	}
}

void winxSwapBuffers() {
	glXSwapBuffers(winx->display, winx->window);
}

void winxClose() {
	glXDestroyContext(winx->display, winx->context);
	XDestroyWindow(winx->display, winx->window);
	XCloseDisplay(winx->display);

	free(winx);
	winx = NULL;
}

void winxSetTitle(const char* title) {
	XStoreName(winx->display, winx->window, title);
    XSetIconName(winx->display, winx->window, title);
}

void winxSetMouseHandle(WinxMouseEventHandle handle) {
	if (winx != NULL) winx->mouse = handle;
}

void winxSetButtonEventHandle(WinxButtonEventHandle handle) {
	if (winx != NULL) winx->button = handle;
}

void winxSetKeybordEventHandle(WinxKeybordEventHandle handle) {
	if (winx != NULL) winx->keyboard = handle;
}

void winxSetScrollEventHandle(WinxScrollEventHandle handle) {
	if (winx != NULL) winx->scroll = handle;
}

void winxSetCloseEventHandle(WinxCloseEventHandle handle) {
	if (winx != NULL) winx->close = handle;
}

#endif // GLX

#if defined(WINX_WINAPI) && defined(WINX_IMPLEMENT)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <gl/GL.h>

/* copied from wglext.h */
typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);

#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_SWAP_COPY_ARB                 0x2029
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_SAMPLE_BUFFERS_ARB            0x2041
#define WGL_SAMPLES_ARB                   0x2042

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

typedef struct {
	HWND hndl;
	HDC device;
	HGLRC context;

	WinxMouseEventHandle mouse;
	WinxButtonEventHandle button;
	WinxKeybordEventHandle keyboard;
	WinxScrollEventHandle scroll;
	WinxCloseEventHandle close;
} WinxHandle;

WinxHandle* winx = NULL;

PROC winxGetProc(LPCSTR name) {
	if (__winx_msg == NULL) {
		PROC proc = wglGetProcAddress(name);

		if (proc == NULL) {
			__winx_msg = "wglGetProcAddress: Failed to load function!";
		}

		return proc;
	}

	return NULL;
}

LRESULT CALLBACK winxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (message) {
		case WM_KEYDOWN:
			winx->keyboard(WINX_PRESSED, wParam);
			break;

		case WM_KEYUP:
			winx->keyboard(WINX_RELEASED, wParam);
			break;

		case WM_MOUSEMOVE:
			winx->mouse(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_LBUTTONDOWN:
			winx->button(WINX_PRESSED, WXB_LEFT);
			break;

		case WM_LBUTTONUP:
			winx->button(WINX_RELEASED, WXB_LEFT);
			break;

		case WM_MBUTTONDOWN:
			winx->button(WINX_PRESSED, WXB_CENTER);
			break;

		case WM_MBUTTONUP:
			winx->button(WINX_RELEASED, WXB_CENTER);
			break;

		case WM_RBUTTONDOWN:
			winx->button(WINX_PRESSED, WXB_RIGHT);
			break;

		case WM_RBUTTONUP:
			winx->button(WINX_RELEASED, WXB_RIGHT);
			break;

		case WM_MOUSEWHEEL:
			winx->scroll(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;

		case WM_CLOSE:
			winx->close();
			break;

		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
	}

	return result;
}

bool winxOpen(int width, int height, const char* title) {

	winx = (WinxHandle*) malloc(sizeof(WinxHandle));
	HINSTANCE hinstance = GetModuleHandle(NULL);

	// set dummy function pointers
	winx->button = WinxDummyButtonEventHandle;
	winx->mouse = WinxDummyMouseEventHandle;
	winx->keyboard = WinxDummyKeybordEventHandle;
	winx->scroll = WinxDummyScrollEventHandle;
	winx->close = WinxDummyCloseEventHandle;

	// register window class
	char* clazz = "WinxOpenGLClass";

	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC) winxWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = clazz;
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex)) {
		__winx_msg = (char*) "RegisterClassEx: Failed to register window class!";
		return false;
	}

	HWND fakeHndl;
	HDC fakeDeviceContext;
	HGLRC fakeRenderContext;

	// create temporary window to get WGL context
	fakeHndl = CreateWindow(clazz, "WINX", WS_OVERLAPPEDWINDOW, 0, 0, 1, 1, NULL, NULL, hinstance, NULL);
	if (!fakeHndl) {
		__winx_msg = (char*) "CreateWindow: Failed to create temporary window!";
		return false;
	}

	fakeDeviceContext = GetDC(fakeHndl);
	if (!fakeDeviceContext) {
		__winx_msg = (char*) "GetDC: Failed to create temporary device context!";
		return false;
	}

	int __winx_hint_color_bits = __winx_hint_red_bits + __winx_hint_green_bits + __winx_hint_blue_bits + __winx_hint_alpha_bits;

	PIXELFORMATDESCRIPTOR pixelFormatDesc;
	memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = __winx_hint_color_bits;
	pixelFormatDesc.cAlphaBits = __winx_hint_alpha_bits;
	pixelFormatDesc.cDepthBits = __winx_hint_depth_bits;
	int fakePixelFormat = ChoosePixelFormat(fakeDeviceContext, &pixelFormatDesc);

	if (!fakePixelFormat) {
		__winx_msg = (char*) "ChoosePixelFormat: Failed to choose a pixel format!";
		return false;
	}

	if (!SetPixelFormat(fakeDeviceContext, fakePixelFormat, &pixelFormatDesc)) {
		__winx_msg = (char*) "SetPixelFormat: Failed to select a pixel format!";
		return false;
	}

	fakeRenderContext = wglCreateContext(fakeDeviceContext);
	if (!fakeRenderContext) {
		__winx_msg = (char*) "wglCreateContext: Failed to create temporary render context!";
		return false;
	}

	// open real window
	winx->hndl = CreateWindow(clazz, title, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hinstance, NULL);
	if (!winx->hndl) {
		__winx_msg = (char*) "CreateWindow: Failed to create window!";
		return false;
	}

	// create context
	winx->device = GetDC(winx->hndl);
	if (!winx->device) {
		__winx_msg = (char*) "GetDC: Failed to create device context!";
		return false;
	}

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	if (!wglMakeCurrent(fakeDeviceContext, fakeRenderContext)) {
		__winx_msg = (char*) "wglMakeCurrent: Failed to select temporary context!";
		return false;
	}

	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) winxGetProc("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) winxGetProc("wglCreateContextAttribsARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT"); // optional

	if (__winx_msg != NULL) {
		// winxGetProc set the error message
		return false;
	}

	int pixelFormat;
	BOOL status;
	UINT numFormats;

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, __winx_hint_color_bits,
		WGL_ALPHA_BITS_ARB, __winx_hint_alpha_bits,
		WGL_DEPTH_BITS_ARB, __winx_hint_depth_bits,
		WGL_SAMPLE_BUFFERS_ARB, __winx_hint_multisamples ? 1 : 0,
		WGL_SAMPLES_ARB, __winx_hint_multisamples,
		0
	};

	int flags = 0;

	if (__winx_hint_opengl_debug) flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
	if (__winx_hint_opengl_robust) flags |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;

	int  contextAttributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, __winx_hint_opengl_major,
		WGL_CONTEXT_MINOR_VERSION_ARB, __winx_hint_opengl_minor,
		WGL_CONTEXT_PROFILE_MASK_ARB, __winx_hint_opengl_core ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, flags,
		0
	};

	status = wglChoosePixelFormatARB(winx->device, pixelAttribs, NULL, 1, &pixelFormat, &numFormats);
	if (status && numFormats) {

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		DescribePixelFormat(winx->device, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

		if (!SetPixelFormat(winx->device, pixelFormat, &pfd)) {
			__winx_msg = (char*) "SetPixelFormat: Failed to select a pixel format!";
			return false;
		}

		winx->context = wglCreateContextAttribsARB(winx->device, 0, contextAttributes);
		if (!winx->context) {
			__winx_msg = (char*) "wglCreateContextAttribsARB: Failed to create a render context!";
			return false;
		}

	} else {
		__winx_msg = (char*) "wglChoosePixelFormatARB: Failed to choose a pixel format!";
		return false;
	}

	// close temporary window
	wglMakeCurrent(fakeDeviceContext, NULL);
	wglDeleteContext(fakeRenderContext);
	ReleaseDC(fakeHndl, fakeDeviceContext);
	DestroyWindow(fakeHndl);

	fakeDeviceContext = NULL;
	fakeRenderContext = NULL;
	fakeHndl = NULL;

	wglMakeCurrent(winx->device, winx->context);

	// set vsync
	if (wglSwapIntervalEXT && __winx_hint_vsync) {
		wglSwapIntervalEXT(__winx_hint_vsync);
	}

	// finish window creation
	ShowWindow(winx->hndl, 1);
	UpdateWindow(winx->hndl);

}

void winxPollEvents() {
	MSG event;

	while (PeekMessageA(&event, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&event);
		DispatchMessage(&event);
	}
}

void winxSwapBuffers() {
	SwapBuffers(winx->device);
}

void winxClose() {
	wglMakeCurrent(winx->device, NULL);
	wglDeleteContext(winx->context);
	ReleaseDC(winx->hndl, winx->device);
	DestroyWindow(winx->hndl);

	free(winx);
	winx = NULL;
}

void winxSetTitle(const char* title) {
	SetWindowTextA(winx->hndl, title);
}

void winxSetMouseHandle(WinxMouseEventHandle handle) {
	if (winx != NULL) winx->mouse = handle;
}

void winxSetButtonEventHandle(WinxButtonEventHandle handle) {
	if (winx != NULL) winx->button = handle;
}

void winxSetKeybordEventHandle(WinxKeybordEventHandle handle) {
	if (winx != NULL) winx->keyboard = handle;
}

void winxSetScrollEventHandle(WinxScrollEventHandle handle) {
	if (winx != NULL) winx->scroll = handle;
}

void winxSetCloseEventHandle(WinxCloseEventHandle handle) {
	if (winx != NULL) winx->close = handle;
}

#endif // WINAPI
