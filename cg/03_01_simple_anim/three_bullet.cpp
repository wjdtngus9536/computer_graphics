#ifndef __cplusplus
#error This file works only with C++
#endif
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#pragma warning(disable: 4711 4710 4100 4514 4626 4774 4365 4625 4464 4571 4201 5026 5027 5039)


#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)
#include <stdio.h>

#include "./common.c"

const unsigned int WIN_W = 300; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 300;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertFileName = "31-uniform-tri.vert";
const char* fragFileName = "31-uniform-tri.frag";

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

// object definitions

glm::vec4 planePos[] = { // big-size air-plane
	{ -0.2F, 0.0F, 0.0F, 1.0F, },
	{ +0.2F, 0.0F, 0.0F, 1.0F, },
	{ +0.0F, 0.3F, 0.0F, 1.0F, },
};

glm::vec4 planeColor[] = { // green colored
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
	{ 0.3F, 1.0F, 0.3F, 1.0F, },
};

const float planeStep = 0.01F;
glm::vec4 planeOrg = glm::vec4( 0.0F, -0.8F, 0.0F, 0.0F );
glm::vec4 planeCur = planeOrg;

glm::vec4 stonePos[] = { // dropping obstacle
	{ +0.00F, +0.1F, 0.0F, 1.0F, },
	{ +0.05F, +0.0F, 0.0F, 1.0F, },
	{ -0.05F, +0.0F, 0.0F, 1.0F, },
};

glm::vec4 stonePos2[] = { // dropping obstacle
	{ +0.00F, +0.1F, 0.0F, 1.0F, },
	{ +0.05F, +0.0F, 0.0F, 1.0F, },
	{ -0.05F, +0.0F, 0.0F, 1.0F, },
};

glm::vec4 stonePos3[] = { // dropping obstacle
	{ +0.00F, +0.1F, 0.0F, 1.0F, },
	{ +0.05F, +0.0F, 0.0F, 1.0F, },
	{ -0.05F, +0.0F, 0.0F, 1.0F, },
};

glm::vec4 stoneColor[] = { // red colored
	{ 1.0F, 0.0F, 0.0F, 1.0F, },
	{ 1.0F, 0.0F, 0.0F, 1.0F, },
	{ 1.0F, 0.0F, 0.0F, 1.0F, },
};

const float stoneStep = 0.00003F;
glm::vec4 stoneOrg = glm::vec4(0.0F, -0.4F, 0.0F, 0.0F );
glm::vec4 stoneOrg2 = glm::vec4(0.0F, -0.3F, 0.0F, 0.0F);
glm::vec4 stoneOrg3 = glm::vec4(0.0F, -0.1F, 0.0F, 0.0F);
glm::vec4 stoneCur = stoneOrg;
glm::vec4 stoneCur2 = stoneOrg2;
glm::vec4 stoneCur3 = stoneOrg3;

void updateFunc(void) {
	// moving stone! : you can add any AI logic here!
	//stoneCur.y += stoneStep;
	//stoneCur2.y += stoneStep;
	//stoneCur3.y += stoneStep;
}

void drawFunc(void) {
	// clear in gray color
	glClear(GL_COLOR_BUFFER_BIT);
	// provide the vertex attributes
	GLuint locPos = glGetAttribLocation(prog, "aPos");
	glEnableVertexAttribArray(locPos);
	// provide the color attributes
	GLuint locColor = glGetAttribLocation(prog, "aColor");
	glEnableVertexAttribArray(locColor);
	// provide the uniform vectors
	GLuint locMove = glGetUniformLocation(prog, "uMove");

	// draw the plane
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(planePos[0]));
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(planeColor[0]));
	glUniform4fv(locMove, 1, glm::value_ptr(planeCur));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// draw the stone
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stonePos[0]));
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stoneColor[0]));
	glUniform4fv(locMove, 1, glm::value_ptr(stoneCur));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// draw the stone
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stonePos2[0]));
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stoneColor[0]));
	glUniform4fv(locMove, 1, glm::value_ptr(stoneCur2));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// draw the stone
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stonePos2[0]));
	glVertexAttribPointer(locColor, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(stoneColor[0]));
	glUniform4fv(locMove, 1, glm::value_ptr(stoneCur3));
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// done
	glFinish();
}

void refreshFunc(GLFWwindow* window) {
	// refresh
	drawFunc();
	// GLFW action
	glfwSwapBuffers(window);
}

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
	case GLFW_KEY_Q:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_R: // reset
		if (action == GLFW_PRESS) {
			planeCur = planeOrg;
			stoneCur = stoneOrg;
		}
		break;
	// user interface: simple joystick!
	case GLFW_KEY_A: // left move
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			planeCur.x -= planeStep;
		}
		break;
	case GLFW_KEY_D: // right move
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			planeCur.x += planeStep;
		}
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			printf("test\n");
			drawFunc();
		}
	}
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
