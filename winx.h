
/*
 * MIT License
 *
 * Copyright (c) 2022 magistermaks
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
 *	Some properties of the window created with 'winxOpen' can be modified with "hints" that have to be set prior to its
 *	creation with the 'winxHint' function. Note that WINX is allowed to ignore impossible to satisfy hints! See the list of available
 *	hints below the function list, under "hint keys".
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

#define WINX_VERSION "1.1.0"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct WinxCursor_s WinxCursor;

typedef void (*WinxCursorEventHandle)(int x, int y);
typedef void (*WinxButtonEventHandle)(int state, int button);
typedef void (*WinxKeyboardEventHandle)(int state, int keycode);
typedef void (*WinxScrollEventHandle)(int scroll);
typedef void (*WinxCloseEventHandle)(void);
typedef void (*WinxResizeEventHandle)(int width, int height);
typedef void (*WinxFocusEventHandle)(bool focused);

/// set a window hint
/// has to be called prior to winxOpen()
void winxHint(int hint, int value);

/// return and clear last winx error
/// returns a NULL pointer if there was no error
char* winxGetError();

/// used to open new window with a given size and title
/// the title can later be changed with winxSetTitle()
bool winxOpen(int width, int height, const char* title);

/// process pending events
/// this will call the registered event handlers
void winxPollEvents();

/// swap display buffers
/// this needs to be called every frame
void winxSwapBuffers();

/// used to close current window
/// this resets all internal state apart from the window hints
void winxClose();

/// set title for current window
/// UTF-8 strings should be supported by this function
void winxSetTitle(const char* title);

/// set the icon of the current window to a RGBA image
/// pass WINX_ICON_DEFAULT as buffer to reset the icon (width and height will be ignored)
void winxSetIcon(int width, int height, unsigned char* buffer);

/// set desired vsync behaviour for current window
/// use one of WINX_VSYNC_DISABLED, WINX_VSYNC_ENABLED, or WINX_VSYNC_ADAPTIVE
void winxSetVsync(int vsync);

/// check if the current window is focused
/// you can also register a focus event handle using winxSetFocusEventHandle()
bool winxGetFocus();

/// focus the current window
/// this call is not guaranteed to be respected by the system
void winxSetFocus();

/// constrain the cursor pointer to the current window
/// this call is known to sometimes be ignored inside Virtual Machines
void winxSetCursorCapture(bool capture);

/// create a icon for the cursor from a RGBA image
/// the returned value should be freed with winxDeleteCursorIcon() when no longer needed
WinxCursor* winxCreateCursorIcon(int width, int height, unsigned char* buffer, int x, int y);

/// create an empty icon for the cursor
/// a helper method for creating an fully transparent cursor icon
WinxCursor* winxCreateNullCursorIcon();

/// free the allocated icon
/// deletes system resources associated with the icon if needed
void winxDeleteCursorIcon(WinxCursor* cursor);

/// set the cursor icon for the current window
/// pass WINX_ICON_DEFAULT to revert to the default system value
void winxSetCursorIcon(WinxCursor* cursor);

/// set the position of the cursor within the current window
/// this call is known to sometimes be ignored inside Virtual Machines
void winxSetCursorPos(int x, int y);

/// set the handle for cursor movement events for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetCursorEventHandle(WinxCursorEventHandle handle);

/// set the handle for cursor click events for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetButtonEventHandle(WinxButtonEventHandle handle);

/// set the handle for keyboard events for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetKeyboardEventHandle(WinxKeyboardEventHandle handle);

/// set the handle for cursor scroll for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetScrollEventHandle(WinxScrollEventHandle handle);

/// set the handle for window close button for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetCloseEventHandle(WinxCloseEventHandle handle);

/// set the handle for window resize event for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetResizeEventHandle(WinxResizeEventHandle handle);

/// set the handle for window focus events for current window
/// pass NULL to unset the associated event handler (if any)
void winxSetFocusEventHandle(WinxFocusEventHandle handle);

/// reset all even handles for current window
/// you can also pass NULL to a specific event handler setter to reset it
void winxResetEventHandles();

/// return a time in seconds since the winxOpen() was called (if successful)
/// this function emulates the behaviour of glfwGetTime()
double winxGetTime();

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

// BEGIN X11 KEYS
// based on https://code.woboq.org/kde/include/X11/keysymdef.h.html
#	ifndef WINX_NO_KEYS
#	define WXK_SPACE      0x0020 // XK_Space
#	define WXK_TAB        0xff09 // XK_Tab
#	define WXK_PAGE_UP    0xff55 // XK_Page_Up
#	define WXK_PAGE_DOWN  0xff56 // XK_Page_Down
#	define WXK_END        0xff57 // XK_End
#	define WXK_PRINT      0xff61 // XK_Print
#	define WXK_ESC        0xff1b // XK_Escape
#	define WXK_ENTER      0xff0d // XK_Return
#	define WXK_BACK       0xff08 // XK_BackSpace
#	define WXK_DELETE     0xffff // WX_Delete
#	define WXK_UP         0xff52 // XK_Up
#	define WXK_DOWN       0xff54 // XK_Down
#	define WXK_LEFT       0xff51 // XK_Left
#	define WXK_RIGHT      0xff53 // XK_Right
#	define WXK_SHIFT      0xffe1 // XK_Shift_L
#	define WXK_CONTROL    0xffe3 // XK_Control_L
#	define WXK_ALT        0xffe9 // XK_Alt_L
#	define WXK_F1         0xffbe // XK_F1
#	define WXK_F2         0xffbf // XK_F2
#	define WXK_F3         0xffc0 // XK_F3
#	define WXK_F4         0xffc1 // XK_F4
#	define WXK_F5         0xffc2 // XK_F5
#	define WXK_F6         0xffc3 // XK_F6
#	define WXK_F7         0xffc4 // XK_F7
#	define WXK_F8         0xffc5 // XK_F8
#	define WXK_F9         0xffc6 // XK_F9
#	define WXK_F10        0xffc7 // XK_F10
#	define WXK_F11        0xffc8 // XK_F11
#	define WXK_F12        0xffc9 // XK_F12
#	define WXK_F13        0xffca // XK_F13
#	define WXK_F14        0xffcb // XK_F14
#	define WXK_F15        0xffcc // XK_F15
#	define WXK_F16        0xffcd // XK_F16
#	define WXK_0          0x0030 // XK_0
#	define WXK_1          0x0031 // XK_1
#	define WXK_2          0x0032 // XK_2
#	define WXK_3          0x0033 // XK_3
#	define WXK_4          0x0034 // XK_4
#	define WXK_5          0x0035 // XK_5
#	define WXK_6          0x0036 // XK_6
#	define WXK_7          0x0037 // XK_7
#	define WXK_8          0x0038 // XK_8
#	define WXK_9          0x0039 // XK_9
#	define WXB_LEFT       1 // Button1 (X.h)
#	define WXB_CENTER     2 // Button2 (X.h)
#	define WXB_RIGHT      3 // Button3 (X.h)
#	endif

#elif defined(_WIN32) || defined(_WIN64)
#	define WINX_WINAPI

// BEGIN WIN KEYS
// based on https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#	ifndef WINX_NO_KEYS
#	define WXK_SPACE      0x20 // VK_SPACE
#	define WXK_TAB        0x09 // VK_TAB
#	define WXK_PAGE_UP    0x21 // VK_PRIOR
#	define WXK_PAGE_DOWN  0x22 // VK_NEXT
#	define WXK_END        0x23 // VK_END
#	define WXK_PRINT      0x2A // VK_PRINT
#	define WXK_ESC        0x1B // VK_ESCAPE
#	define WXK_ENTER      0x0D // VK_RETURN
#	define WXK_BACK       0x08 // VK_BACK
#	define WXK_DELETE     0x2E // VK_DELETE
#	define WXK_UP         0x26 // VK_UP
#	define WXK_DOWN       0x28 // VK_DOWN
#	define WXK_LEFT       0x25 // VK_LEFT
#	define WXK_RIGHT      0x27 // VK_RIGHT
#	define WXK_SHIFT      0xA0 // VK_LSHIFT
#	define WXK_CONTROL    0xA2 // VK_LCONTROL
#	define WXK_ALT        0x12 // VK_MENU
#	define WXK_F1         0x70 // VK_F1
#	define WXK_F2         0x71 // VK_F2
#	define WXK_F3         0x72 // VK_F3
#	define WXK_F4         0x73 // VK_F4
#	define WXK_F5         0x74 // VK_F5
#	define WXK_F6         0x75 // VK_F6
#	define WXK_F7         0x76 // VK_F7
#	define WXK_F8         0x77 // VK_F8
#	define WXK_F9         0x78 // VK_F9
#	define WXK_F10        0x79 // VK_F10
#	define WXK_F11        0x7A // VK_F11
#	define WXK_F12        0x7B // VK_F12
#	define WXK_F13        0x7C // VK_F13
#	define WXK_F14        0x7D // VK_F14
#	define WXK_F15        0x7E // VK_F15
#	define WXK_F16        0x7F // VK_F16
#	define WXK_0          0x30 // VK_0
#	define WXK_1          0x31 // VK_1
#	define WXK_2          0x32 // VK_2
#	define WXK_3          0x33 // VK_3
#	define WXK_4          0x34 // VK_4
#	define WXK_5          0x35 // VK_5
#	define WXK_6          0x36 // VK_6
#	define WXK_7          0x37 // VK_7
#	define WXK_8          0x38 // VK_8
#	define WXK_9          0x39 // VK_9
#	define WXB_LEFT       1
#	define WXB_CENTER     2
#	define WXB_RIGHT      3
#	endif

#endif

#ifdef __cplusplus
}
#endif

#endif // WINX_H
