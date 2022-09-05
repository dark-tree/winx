
#include "glad/glad.h"
#include <winx.h>
#include <stdio.h>
#include "render.h"

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
	}

}

