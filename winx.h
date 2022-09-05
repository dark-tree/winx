
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

/*
 * General WINX documentation:
 *
 *	WINX is a simple to use, minimal, cross-platform single-header window management library
 *	designed to easily create an OpenGL 3.0+ compatible window on both Windows and Linux (as well as on MacOS
 *	through X11).
 *
 *	To create a new window call the 'winxOpen' function. Note: WINX is a state machine and supports only
 *	one window per thread! If that call returns true window should have been successfully initialized and
 *	an OpenGL context created. Otherwise use 'winxGetError' to get the error message string.
 *
 *	if (!winxOpen(500, 300, "test")) {
 *		printf("Error occured! %s\n", winxGetError());
 *		exit(1);
 *	}
 *
 *	Some properties of the window that is created with 'winxOpen' can be modified with "hints" that have to be set prior to its
 *	creation with the 'winxHint' function. Note that WINX is allowed to ignore impossible to satisfy hints! See the list of available
 *	hints in the implementation of the 'winxHint' function.
 *
 *	Once a window is open you can begin the render loop and call 'winxSwapBuffers' and 'winxPollEvents'.
 *	To register callbacks for window events use the 'winxSet*EventHandle' family of functions and their respective
 *	'Winx*EventHandle' callback types. Clicking on a "close window" button can be detected with the 'winxSetCloseEventHandle'.
 *	The Vsync behaviour can also be changed after window creation using 'winxSetVsync'.
 *
 *	while(1) {
 *		// draw here
 *
 *		winxSwapBuffers(); // swap frame buffers
 *		winxPollEvents(); // poll events and call user event callbacks
 *	}
 *
 *	To close the window call 'winxClose'. Note that this doesn't reset any modified window hints!
 */

#ifndef WINX_H
#define WINX_H

#define WINX_VERSION "1.0.0"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct WinxCursor_s WinxCursor;

typedef void (*WinxMouseEventHandle)(int x, int y);
typedef void (*WinxButtonEventHandle)(int state, int button);
typedef void (*WinxKeybordEventHandle)(int state, int keycode);
typedef void (*WinxScrollEventHandle)(int scroll);
typedef void (*WinxCloseEventHandle)(void);
typedef void (*WinxResizeEventHandle)(int width, int height);
typedef void (*WinxFocusEventHandle)(bool focused);

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

/// used to close current window
void winxClose();

/// set title for current window
void winxSetTitle(const char* title);

/// set the icon of this window to a RGBA image, pass WINX_ICON_DEFAULT to revert
void winxSetIcon(int width, int height, unsigned char* buffer);

/// set desired vsync behaviour for current window
void winxSetVsync(int vsync);

/// check if the current window is focused
bool winxGetFocus();

/// focus the current window
void winxSetFocus();

/// constreain the mouse pointer to the current window
void winxSetMouseCapture(bool capture);

/// get the cursor icon for the mouse pointer from a RGBA image
WinxCursor* winxCreateCursorIcon(int width, int height, unsigned char* buffer, int x, int y);

/// get an empty cursor icon for the mouse pointer
WinxCursor* winxCreateNullCursorIcon();

/// free the allocated icon
void winxDeleteCursorIcon(WinxCursor* cursor);

/// set the cursor icon for the current window, accepts WINX_ICON_DEFAULT
void winxSetCursorIcon(WinxCursor* cursor);

/// set the position of the cursor within the current window
void winxSetCursorPos(int x, int y);

/// set the handle for mouse movement events for current window, pass NULL to unset
void winxSetMouseEventHandle(WinxMouseEventHandle handle);

/// set the handle for mouse click events for current window, pass NULL to unset
void winxSetButtonEventHandle(WinxButtonEventHandle handle);

/// set the handle for keybord events for current window, pass NULL to unset
void winxSetKeybordEventHandle(WinxKeybordEventHandle handle);

/// set the handle for mouse scroll for current window, pass NULL to unset
void winxSetScrollEventHandle(WinxScrollEventHandle handle);

/// set the handle for window close button for current window, pass NULL to unset
void winxSetCloseEventHandle(WinxCloseEventHandle handle);

/// set the handle for window resize event for current window, pass NULL to unset
void winxSetResizeEventHandle(WinxResizeEventHandle handle);

/// set the handle for window focus events for current window, pass NULL to unset
void winxSetFocusEventHandle(WinxFocusEventHandle handle);

/// reset all even handles for current window
void winxResetEventHandles();

#define WINX_ICON_DEFAULT NULL
#define WINX_PRESSED 1
#define WINX_RELEASED 0

/// hint keys
#define WINX_HINT_VSYNC         0x01
#define WINX_HINT_RED_BITS      0x02
#define WINX_HINT_GREEN_BITS    0x03
#define WINX_HINT_BLUE_BITS     0x04
#define WINX_HINT_ALPHA_BITS    0x05
#define WINX_HINT_DEPTH_BITS    0x06
#define WINX_HINT_STENCIL_BITS  0x07
#define WINX_HINT_OPENGL_MAJOR  0x08
#define WINX_HINT_OPENGL_MINOR  0x09
#define WINX_HINT_OPENGL_CORE   0x0A
#define WINX_HINT_OPENGL_DEBUG  0x0B
#define WINX_HINT_OPENGL_ROBUST 0x0C
#define WINX_HINT_MULTISAMPLES  0x0D

/// hint values
#define WINX_VSYNC_DISABLED 0
#define WINX_VSYNC_ENABLED 1
#define WINX_VSYNC_ADAPTIVE -1

#if defined(__unix__) || defined(__linux__)
#	define WINX_GLX

// based on https://code.woboq.org/kde/include/X11/keysymdef.h.html
#	define WXK_SPACE    ' '
#	define WXK_TAB 		'\t'
#	define WXK_ESC      0xff1b // XK_Escape
#	define WXK_ENTER    0xff0d // XK_Return
#	define WXK_BACK     0xff08 // XK_BackSpace
#	define WXK_UP       0xff52 // XK_Up
#	define WXK_DOWN     0xff54 // XK_Down
#	define WXK_LEFT     0xff51 // XK_Left
#	define WXK_RIGHT    0xff53 // XK_Right
#	define WXB_LEFT     1 // Button1 (X.h)
#	define WXB_CENTER   2 // Button2 (X.h)
#	define WXB_RIGHT    3 // Button3 (X.h)

#elif defined(_WIN32) || defined(_WIN64)
#	define WINX_WINAPI

// based on https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#	define WXK_SPACE    ' '
#	define WXK_TAB      '\t'
#	define WXK_ESC      0x1B // VK_ESCAPE
#	define WXK_ENTER    0x0D // VK_RETURN
#	define WXK_BACK     0x08 // VK_BACK
#	define WXK_UP       0x26 // VK_UP
#	define WXK_DOWN     0x28 // VK_DOWN
#	define WXK_LEFT     0x25 // VK_LEFT
#	define WXK_RIGHT    0x27 // VK_RIGHT
#	define WXB_LEFT     1
#	define WXB_CENTER   2
#	define WXB_RIGHT    3

#endif

#ifdef __cplusplus
}
#endif

#endif // WINX_H

