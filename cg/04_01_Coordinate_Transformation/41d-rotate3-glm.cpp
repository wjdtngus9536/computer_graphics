﻿#ifndef __cplusplus
#error This file works only with C++
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

#pragma warning(disable: 4711 4710 4100 4514 4626 4774 4365 4625 4464 4571 4201 5026 5027 5039)

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)

#include "./common.c"

const unsigned int WIN_W = 300; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 300;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertFileName = "41-set-mat4.vert";
const char* fragFileName = "41-set-mat4.frag";

GLuint vert = 0; // vertex shader ID number
GLuint frag = 0; // fragment shader ID number
GLuint prog = 0; // shader program ID number

void initFunc(void) {
	const char* vertSource = loadFile( vertFileName );
	const char* fragSource = loadFile( fragFileName );
	char buf[1024]; // mesg buffer
	GLint status; // for glGetShaderiv()
	// vert: vertex shader
	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertSource, nullptr);
	glCompileShader(vert); // compile to get .OBJ
	glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
	printf("vert compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(vert, sizeof(buf), nullptr, buf);
	printf("vert log = [%s]\n", buf);
	// frag: fragment shader
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragSource, nullptr);
	glCompileShader(frag); // compile to get .OBJ
	glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
	printf("frag compile status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetShaderInfoLog(frag, sizeof(buf), nullptr, buf);
	printf("frag log = [%s]\n", buf);
	// prog: program
	prog = glCreateProgram();
	glAttachShader(prog, vert);
	glAttachShader(prog, frag);
	glLinkProgram(prog); // link to get .EXE
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	printf("prog link status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), nullptr, buf);
	printf("link log = [%s]\n", buf);
	glValidateProgram(prog);
	glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
	printf("prog validate status = %s\n", (status == GL_TRUE) ? "true" : "false");
	glGetProgramInfoLog(prog, sizeof(buf), nullptr, buf);
	printf("validate log = [%s]\n", buf);
	fflush(stdout);
	// execute it!
	glUseProgram(prog);
	// done
	free( (void*)vertSource );
	free( (void*)fragSource );
}

glm::vec4 vertPos[] = { // 6 * 3 = 18 vertices
	// face 0: v0-v1-v2
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	// face 1: v0-v2-v3
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	// face 2: v0-v3-v4
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	// face 3: v0-v4-v1
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	// face 4: v1-v4-v3
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	// face 5: v1-v3-v2
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	// your reference
#if 0
	{ 0.0F, 0.5F, 0.0F, 1.0F }, // v0
	{ 0.5F, -0.3F, 0.0F, 1.0F }, // v1
	{ 0.0F, -0.3F, -0.5F, 1.0F }, // v2
	{ -0.5F, -0.3F, 0.0F, 1.0F }, // v3
	{ 0.0F, -0.3F, 0.5F, 1.0F }, // v4
#endif
};

glm::vec4 vertColor[] = {
	// face 0: red
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	{ 1.0F, 0.3F, 0.3F, 1.0F, },
	// face 1: green
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	// face 2: blue
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	{ 0.3F, 0.3F, 1.0F, 1.0F, },
	// face 3: yellow
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	{ 1.0F, 1.0F, 0.3F, 1.0F, },
	// face 4: cyan
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	// face 5: cyan
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
	{ 0.3F, 1.0F, 1.0F, 1.0F, },
};


glm::vec3 angle = { 0.0F, 0.0F, 0.0F };
glm::vec3 dir = { 0.0F, 0.0F, 0.0F };

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_R:
		if (action == GLFW_PRESS) {
			angle = glm::vec3( 0.0F, 0.0F, 0.0F );
			dir = glm::vec3( 0.0F, 0.0F, 0.0F );
		}
		break;
	case GLFW_KEY_Q: dir.x = +1.0F; break;
	case GLFW_KEY_A: dir.x =  0.0F; break;
	case GLFW_KEY_Z: dir.x = -1.0F; break;
	case GLFW_KEY_W: dir.y = +1.0F; break;
	case GLFW_KEY_S: dir.y =  0.0F; break;
	case GLFW_KEY_X: dir.y = -1.0F; break;
	case GLFW_KEY_E: dir.z = +1.0F; break;
	case GLFW_KEY_D: dir.z =  0.0F; break;
	case GLFW_KEY_C: dir.z = -1.0F; break;
	}
}


system_clock::time_point lastTime = system_clock::now();
glm::mat4 mat = glm::mat4( 1.0F );

void updateFunc(void) {
	system_clock::time_point curTime = system_clock::now();
	milliseconds elapsedTimeMSEC = duration_cast<milliseconds>(curTime - lastTime); // in millisecond
	GLfloat theta = (elapsedTimeMSEC.count() / 1000.0F) * (float)M_PI; // in <math.h>, M_PI = pi
	angle += theta * dir;
	lastTime = curTime;
	// calculate the matrix: mat = Rz Ry Rx
	mat = glm::mat4(1.0F);
	mat = glm::rotate(mat, angle.z, glm::vec3(0.0F, 0.0f, 1.0f));
	mat = glm::rotate(mat, angle.y, glm::vec3(0.0F, 1.0f, 0.0f));
	mat = glm::rotate(mat, angle.x, glm::vec3(1.0F, 0.0f, 0.0f));
}

void drawFunc(void) {
	// z-buffer setting
	glEnable(GL_DEPTH_TEST);
	glDepthRange(0.0F, 1.0F);
	glClearDepthf(1.0F);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	// clear in gray color
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// provide the vertex attributes
	GLuint locPos = glGetAttribLocation(prog, "aPos");
	glEnableVertexAttribArray(locPos);
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertPos[0]));
	GLuint locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertColor[0]));
	GLuint locMat = glGetUniformLocation(prog, "uMat");
	glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(mat));
	// draw the pyramid
	glDrawArrays(GL_TRIANGLES, 0, 18); // 18 vertices
	// done
	glFinish();
}

void refreshFunc(GLFWwindow* window) {
	// refresh
	drawFunc();
	// GLFW action
	glfwSwapBuffers(window);
}

int main(int argc, char* argv[]) {
	const char* basename = getBaseName( argv[0] );
	// start GLFW & GLEW
	glfwInit();
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, basename, nullptr, nullptr);
	glfwSetWindowPos(window, WIN_X, WIN_Y);
	glfwMakeContextCurrent(window);
	glewInit();
	// prepare
	glfwSetWindowRefreshCallback(window, refreshFunc);
	glfwSetKeyCallback(window, keyFunc);
	glClearColor(0.5F, 0.5F, 0.5F, 1.0F);
	// main loop
	initFunc();
	while (! glfwWindowShouldClose(window)) {
		// animation loop
		updateFunc();
		drawFunc();
		// GLFW actions
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// done
	glfwTerminate();
	return 0;
}
