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
#pragma comment(lib, "glew32.lib") �� 
#pragma comment(lib, "glfw3.lib")�� Microsoft �����Դϴ�. 
���� ���� ���Ͽ� ������ ���̺귯���� �����ϵ��� ��Ŀ�� �����ϴ� pragma�Դϴ�. 
�� ��� ���̺귯���� opengl32.lib(OpenGL ���̺귯��), 
glew32.lib(GLEW ���̺귯��) �� 
glfw3.lib(GLFW ���̺귯��)�Դϴ�.

#pragma warning(disable: 4711 4710 4100)�� Microsoft Visual C++ �����Ϸ����� �߻��ϴ� 
�Ϻ� ��� �޽����� ��Ȱ��ȭ�մϴ�. ��� ��ȣ�� 
4711(�ڵ� �ζ��� Ȯ���� ���� ���õ� �Լ�), 
4710(�ζ��ε��� ���� �Լ�) �� 
4100(�������� �ʴ� ���� �Ű�����)�Դϴ�.
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
//	// ������ + ������� ���õ� ���� ������ & ���� OpenGL state ����
//	GLFWwindow* window = glfwCreateWindow(
//		500,			// 
//		500,			// 
//		"Hello GLAD",
//		NULL,			// GLFWmonitor* monitor : glfwGetPrimaryMonitor()�� �ָ� Ǯ ��ũ��, NULL�� �ָ� windowed ���
//		NULL			// GLFWwindow* share : ������ �����츦 �Ѱ��ָ� �ش� �������� ���� ������� ���°� ������
//	);
//	// OpenGL �׸� ������ Ÿ�� ������ ����
//	glfwMakeContextCurrent(window);
//
//	// start GLAD
//	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//
//	// glfwWindowShouldClose�� return ���� 0�� �ƴ϶�� �ݺ� ����
//	while (!glfwWindowShouldClose(window)) {
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// glfw actions
//		// ������Ʈ�� �׷��� ��� ���
//		glfwSwapBuffers(window);
//		// �߻��� �̺�Ʈ�� ��Ƽ� ����
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}