#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)
#include <stdio.h>
#include <string.h> // for strrchr()

const unsigned int WIN_W = 300; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 300;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y) 
const unsigned int WIN_Y = 100;

void refreshFunc(GLFWwindow* window) {
	// refresh
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	// GLFW action
	// 실시간 렌더링 시 플리커링이나 티어링 방지를 위해 이중 버퍼링에 필요한 윈도우의 전면 및 후면 버퍼를 교체하는 역할
	// 이전 버퍼가 화면에 표시되는 동안 숨겨진 버퍼에 그래픽을 렌더링. 그래피이 매끄럽고 시각적 결함 없이 렌더링 됨
	// 렌더링 프로세스가 끝날 때 항상 glfwSwapBuffers() 함수를 호출해야 함
	glfwSwapBuffers(window);
}

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	}
}

int main(int argc, char* argv[]) {
	// get your program name
#if defined(_WIN32) || defined(_WIN64)
	char* win_name = (strrchr(argv[0], '\\') == NULL) ? argv[0] : (strrchr(argv[0], '\\') + 1);
#else // Unix, Linux, MacOS
	char* win_name = (strrchr(argv[0], '/') == NULL) ? argv[0] : (strrchr(argv[0], '/') + 1);
#endif
	// start GLFW & GLEW
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, win_name, NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();

	// prepare
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(0.933F, 0.769F, 0.898F, 1.0F);

	// main loop
	while (! glfwWindowShouldClose(window)) {
		// draw
		glClear(GL_COLOR_BUFFER_BIT);
		glFinish();
		// GLFW actions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// done
	glfwTerminate();
	return 0;
}
