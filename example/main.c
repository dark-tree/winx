
#include "glad/glad.h"
#include <winx.h>

#include <stdio.h>
#include <sys/time.h>

#include "render.h"

// for printing fps every 5s
void watch() {
	static int t0 = -1;
	static int frames = 0;

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	int t = (int) tv.tv_sec;

	if (t0 < 0) t0 = t;

	frames++;

	if (t - t0 >= 5.0) {
		float seconds = t - t0;
		float fps = frames / seconds;
		printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds, fps);
		t0 = t;
		frames = 0;
	}
}

void closeEventHandle() {
	winxClose();
	exit(0);
}

void resizeEventHandle(int w, int h) {
	glViewport(0, 0, w, h);
}

int main() {

	if (!winxOpen(400, 300, "WINX OpenGL 3.0 Example")) {
		printf("Error occured! %s\n", winxGetError());
		exit(1);
	}

//	winxSetMouseCapture(true);

    WinxCursor* cursor = winxCreateNullCursorIcon();
	winxSetCursorIcon(cursor);

	// use GLAD to load OpenGL functions
	gladLoadGL();

	// init OpenGL example
	init();

	winxSetCloseEventHandle(closeEventHandle);
	winxSetResizeEventHandle(resizeEventHandle);

	while(1) {
		draw();

		winxSwapBuffers();
		winxPollEvents();
		watch();
	}

	winxDeleteCursorIcon(cursor);

}

