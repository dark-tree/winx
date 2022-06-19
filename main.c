
#define WINX_IMPLEMENT
#include "winx.h"

#include <stdio.h>
#include <sys/time.h>

void wait() {
	static int t0 = -1;
	static int frames = 0;

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	int t = (int) tv.tv_sec;

	if (t0 < 0) t0 = t;

	frames++;

	if (t - t0 >= 5.0) {
		GLfloat seconds = t - t0;
		GLfloat fps = frames / seconds;
		printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds, fps);
		t0 = t;
		frames = 0;
	}
}

void keyboardEvent(int state, int key) {
	if( state = WINX_PRESSED ) {
		exit(0);
	}
}

int main() {

	if (!winxOpen(500, 300, "test")) {
		printf("Error occured! %s\n", winxGetError());
		exit(1);
	}

//	winxSetKeyboardHandle(window, keyboardEvent);

	while(1) {
		winxPollEvents();

		// draw here (or not)

		winxSwapBuffers();
		wait();
	}

	winxClose();

}

