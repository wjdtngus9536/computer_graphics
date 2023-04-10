#include<GL/glew.h>
#include<GLFW/glfw3.h>
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
	//refresh
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();

	glfwSwapBuffers(window);
}

GLfloat clr[4] = { 0.933F, 0.796F, 0.898F, 1.0F };

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mod) {
	switch (key) {
	case 'Q': clr[0] += 0.01F; if (clr[0] > 1.0F) clr[0] = 1.0F; break;
	case 'W': clr[1] += 0.01F; if (clr[1] > 1.0F) clr[1] = 1.0F; break;
	case 'E': clr[2] += 0.01F; if (clr[2] > 1.0F) clr[2] = 1.0F; break;
	case 'A': clr[0] -= 0.01F; if (clr[0] < 0.0F) clr[0] = 0.0F; break;
	case 'S': clr[1] -= 0.01F; if (clr[1] < 0.0F) clr[1] = 0.0F; break;
	case 'D': clr[2] -= 0.01F; if (clr[2] < 0.0F) clr[2] = 0.0F; break;
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	}
	glClearColor(clr[0], clr[1], clr[2], clr[3]);
}

int main(int argc, char* argv[]) {
	// get your program name
#if defined(_WIN32) || defined(_WIN64)
	char* win_name = (strrchr(argv[0], '\\') == NULL) ? argv[0] : (strrchr(argv[0], '\\') + 1);
#else // Unix, Linux, MacOS
	char* win_name = (strrchr(argv[0], '/') == NULL) ? argv[0] : (strrchr(argv[0], '/') + 1);
#endif

	// start GLFW & glew
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, win_name, NULL, NULL); //glfwGetPrimaryMonitor()
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();

	// prepare
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(clr[0], clr[1], clr[2], clr[3]);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glFinish();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

/********************************************
#pragma comment(lib, "opengl32.lib"), 
#pragma comment(lib, "glew32.lib") 및 
#pragma comment(lib, "glfw3.lib")는 Microsoft 전용입니다. 
최종 실행 파일에 지정된 라이브러리를 포함하도록 링커에 지시하는 pragma입니다. 
이 경우 라이브러리는 opengl32.lib(OpenGL 라이브러리), 
glew32.lib(GLEW 라이브러리) 및 
glfw3.lib(GLFW 라이브러리)입니다.

#pragma warning(disable: 4711 4710 4100)은 Microsoft Visual C++ 컴파일러에서 발생하는 
일부 경고 메시지를 비활성화합니다. 경고 번호는 
4711(자동 인라인 확장을 위해 선택된 함수), 
4710(인라인되지 않은 함수) 및 
4100(참조되지 않는 형식 매개변수)입니다.
********************************************/


//#include<glad/glad.h>
//#include<GLFW/glfw3.h>
//#include<windows.h>
//#include "glad.c"
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glfw3.lib")
//
//int main(void) {
//	glfwInit();
//	// 윈도우 + 윈도우와 관련된 내부 변수들 & 현재 OpenGL state 생성
//	GLFWwindow* window = glfwCreateWindow(
//		500,			// 
//		500,			// 
//		"Hello GLAD",
//		NULL,			// GLFWmonitor* monitor : glfwGetPrimaryMonitor()를 주면 풀 스크린, NULL을 주면 windowed 모드
//		NULL			// GLFWwindow* share : 기존의 윈도우를 넘겨주면 해당 윈도우의 내부 변수들과 상태가 공유됨
//	);
//	// OpenGL 그림 생성할 타겟 윈도우 설정
//	glfwMakeContextCurrent(window);
//
//	// start GLAD
//	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//
//	// glfwWindowShouldClose의 return 값이 0이 아니라면 반복 종료
//	while (!glfwWindowShouldClose(window)) {
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// glfw actions
//		// 업데이트된 그래픽 결과 출력
//		glfwSwapBuffers(window);
//		// 발생한 이벤트들 모아서 관리
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}