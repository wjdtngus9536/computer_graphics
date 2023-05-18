#define _CRT_SECURE_NO_WARNINGS
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"./common.c"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

unsigned int WIN_W = 300;
unsigned int WIN_H = 300;
unsigned int WIN_X = 100;
unsigned int WIN_Y = 100;

const char* vertFileName = "31-uniform-tri.vert";
const char* fragFileName = "31-uniform-tri.frag";

GLuint vert = 0;
GLuint frag = 0;
GLuint prog = 0;

void initFunc(void) {
	const char* vertSource = loadFile(vertFileName);
	const char* fragSource = loadFile(fragFileName);

	GLint status;
	char buf[1024];
	
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
	printf("vert 컴파일 상태 = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vert, sizeof(buf), NULL, buf);
	printf("vert log = [%s]\n", buf);


	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, NULL);
	glCompileShader(frag);
	
	glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
	printf("fs 컴파일 상태 = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(frag, sizeof(buf), NULL, buf);
	printf("frag log = [%s]\n", buf);

	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("prog link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("link log = [%s]\n", buf);

	
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("prog validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
	printf("validate log = [%s]\n", buf);
	fflush(stdout);

	glUseProgram(prog);

	free((void*)vertSource);
	free((void*)fragSource);
}

GLfloat vertPos[] = {
	-0.5F, -0.5F, 0.0F, 1.0F,
	+0.5F, -0.5F, 0.0F, 1.0F,
	-0.5F, +0.5F, 0.0F, 1.0F,
};

GLfloat vertColor[] = {
	1.0F, 0.0F, 0.0F, 1.0F,
	0.0F, 1.0F, 0.0F, 1.0F,
	0.0F, 0.0F, 1.0F, 1.0F,
};

const float step = 0.0005F;

GLfloat moveCur[] = {
	0.0F, 0.0F, 0.0F, 0.0F
};

void updateFunc(void) {
	moveCur[0] += step;
}

void drawFunc(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint locPos = glGetAttribLocation(prog, "aPos");
	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, vertPos);

	GLuint locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, vertColor);

	GLuint locMove = glGetUniformLocation(prog, "uMove");
	glUniform4f(locMove, moveCur[0], moveCur[1], moveCur[2], moveCur[3]);
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

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, basename, NULL, NULL);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(0.5F, 0.5F, 0.5F, 1.0F);


	initFunc();
	while(! glfwWindowShouldClose(window)) {
		updateFunc();
		drawFunc();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}