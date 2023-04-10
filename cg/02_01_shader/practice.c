#define _CRT_SECURE_NO_WARNINGS
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)
#include <stdio.h>
#include <string.h> // for strrchr()

#include "./common.c"

// for glCreateWindow()
const unsigned int WIN_W = 300;
const unsigned int WIN_H = 300;

// for glfwSetWindowPos(window, win_x, win_y);
const unsigned int WIN_X = 100;
const unsigned int WIN_Y = 100;

const char* vertFileName = "21-simple-tri.vert";
const char* fragFileName = "21-simple-tri.frag";

// ID 저장
GLuint vert = 0;
GLuint frag = 0;
GLuint prog = 0;

void initFunc(void) {
	// 문자열로 Shader code 불러오기
	const char* vertSource = loadFile(vertFileName);
	const char* fragSource = loadFile(fragFileName);

	// vertex shader 생성 & id return
	vert = glCreateShader(GL_VERTEX_SHADER);
	// 쉐이더 보드 다운로드
	glShaderSource(vert, 1, &vertSource, NULL);
	// 컴파일 하여 목적파일 생성
	glCompileShader(vert);

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);

	// 쉐이더 프로그램 생성
	prog = glCreateProgram();
	// 쉐이더 프로그램에 vert, frag shader 붙이기
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	// 
	glLinkProgram(prog);
	
	glUseProgram(prog);

	free((void*)vertSource);
	free((void*)fragSource);
}

GLfloat vertexPos[] = {
	-0.5F, -0.5F, 0.0F, 1.0F,
	+0.5F, -0.5F, 0.0F, 1.0F,
	-0.5F, +0.5F, 0.0F, 1.0F,
};

void drawFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	// vertexPos
	// step02. vertexPos라는 변수를 가진 Attirbute register의 index 찾기
	GLuint loc = glGetAttribLocation(prog, "vertexPos");
	// step03. 해당 index를 갖는 Attribute register를 실행
	glEnableVertexAttribArray(loc);
	// Step04.C 프로그램의 vertexPos라는 배열과 연결
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, vertexPos);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glFinish();
}

void refreshFunc(GLFWwindow* window) {
	drawFunc();
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
	const char* basename = getBaseName(argv[0]);
	
	// start glfw, glew
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, basename, NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();

	// prepare
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(0.5F, 0.5F, 0.5F, 1.0F);

	// main loop
	// step01. 쉐이더 프로그램 실행
	initFunc();
	while (!glfwWindowShouldClose(window)) {
		//draw
		drawFunc();

		// glfw actions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//done
	glfwTerminate();
	return 0;
}