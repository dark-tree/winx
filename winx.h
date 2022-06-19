
/*
 * MIT License
 *
 * Copyright (c) 2020 magistermaks
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdbool.h>

typedef void (*WinxEventHandle)(int, int);

/// used to open new window
bool winxOpen(int width, int height, const char* title);

/// used to close window
void winxClose();

/// return and clear last winx error, or NULL if there was no error
char* winxGetError();

/// process pending events
void winxPollEvents();

/// swap display buffers
void winxSwapBuffers();

#define WINX_PRESSED 1
#define WINX_RELEASED 0

#if defined(__unix__) || defined(__linux__)
#	define WINX_GLX
#elif defined(_WIN32) || defined(_WIN64)
#	define WINX_WINAPI
#endif

// begin winx GLX implementation
#if defined(WINX_GLX) && defined(WINX_IMPLEMENT)

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <stdlib.h>

// dummy functions
void DummyEventHandle(int a, int b) {}

char* msg = NULL;

typedef struct {
	Display* display;
	Window window;
	GLXContext context;

	WinxEventHandle mouse;
	WinxEventHandle button;
	WinxEventHandle keyboard;
} WinxHandle;

WinxHandle* winx = NULL;

bool winxOpen(int width, int height, const char* title) {
	winx = (WinxHandle*) malloc(sizeof(WinxHandle));

	// set dummy function pointers
	winx->keyboard = DummyEventHandle;
	winx->button = DummyEventHandle;
	winx->mouse = DummyEventHandle;

	// get display handle
	winx->display = XOpenDisplay(NULL);
	if( !winx->display ) {
		msg = (char*) "Failed to acquire X11 display handle!";
		return false;
	}

	// GLX attributes
	int glx_attributes[] = {
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_DOUBLEBUFFER,
		GLX_DEPTH_SIZE, 8,
		GLX_STENCIL_SIZE, 1,
		None
	};

	int screen = DefaultScreen(winx->display);
	Window root = RootWindow(winx->display, screen);

	// find visual based on glx_attributes
	XVisualInfo* info = glXChooseVisual(winx->display, screen, glx_attributes);
	if( !info ) {
		msg = (char*) "Failed to acquire GLX visual!";
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
	winx->window = XCreateWindow(
		winx->display,
		root,
		0, // x
		0, // y
		width,
		height,
		0, // border width
		info->depth,
		InputOutput,
		info->visual,
		mask,
		&x11_attributes
	);

	// set name
	XStoreName(winx->display, winx->window, title);
    XSetIconName(winx->display, winx->window, title);

	// create GLX context
	winx->context = glXCreateContext(winx->display, info, NULL, 1);
	if( !winx->context ) {
		msg = (char*) "Failed to create GLX context!";
		return false;
	}

	XFree(info);

	// show X11 window
	XMapWindow(winx->display, winx->window);
	glXMakeCurrent(winx->display, winx->window, winx->context);

	return true;
}

void winxClose() {
	glXDestroyContext(winx->display, winx->context);
	XDestroyWindow(winx->display, winx->window);
	XCloseDisplay(winx->display);
}

char* winxGetError() {
	char* copy = msg;
	msg = NULL;

	return copy;
}

void winxPollEvents() {
	while( XPending(winx->display) > 0 ) {

		XEvent event;
		XNextEvent(winx->display, &event);

		switch( event.type ) {

			case Expose:
				break;

			case KeyPress:
				winx->keyboard(WINX_PRESSED, XLookupKeysym(&event.xkey, 0));
				break;

			case KeyRelease:
				winx->keyboard(WINX_RELEASED, XLookupKeysym(&event.xkey, 0));
				break;

			case ButtonPress:
				winx->button(WINX_PRESSED, event.xbutton.button);
				break;

			case ButtonRelease:
				winx->button(WINX_RELEASED, event.xbutton.button);
				break;

			case MotionNotify:
				winx->mouse(event.xmotion.x_root, event.xmotion.y_root);
				break;

		}

	}
}

void winxSwapBuffers() {
	glXSwapBuffers(winx->display, winx->window);
}

#endif // GLX

