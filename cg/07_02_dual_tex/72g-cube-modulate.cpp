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
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp> // for glm::to_string()
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr( )

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#include "./common.c"

// check for new syntax with texture objects
static const bool use_new_tex_syntax = true;

const unsigned int WIN_W = 300; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 300;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertFileName = "72-cube-modulate.vert";
const char* fragFileName = "72-cube-modulate.frag";

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

int levelSphere = 3;
int numVertSphere = 0;
glm::vec4 vertSphere[10240];
glm::vec4 normSphere[10240];
glm::vec4 colorSphere[10240];
glm::vec3 texCoordSphere[10240];

GLuint texObj = 0;

#include "earth-map-0.cpp"
#include "earth-map-1.cpp"
#include "earth-map-2.cpp"
#include "earth-map-3.cpp"
#include "earth-map-4.cpp"
#include "earth-map-5.cpp"

void initTex(void) {
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	// make a tex object
	glGenTextures(1, &texObj);
	// bind the texture object to the texture unit #0
	glActiveTexture(GL_TEXTURE0); // select tex unit #0
	glBindTexture(GL_TEXTURE_CUBE_MAP, texObj);
	// new for OpenGL 4.5+: load an image to the texture object
	if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
		printf("using OpenGL 4.5 syntax\n");
		glTextureStorage2D(texObj, 1, GL_RGBA8, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT);
		glTextureSubImage3D(texObj, 0, 0, 0, 0, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_0);
		glTextureSubImage3D(texObj, 0, 0, 0, 1, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_1);
		glTextureSubImage3D(texObj, 0, 0, 0, 2, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_2);
		glTextureSubImage3D(texObj, 0, 0, 0, 3, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_3);
		glTextureSubImage3D(texObj, 0, 0, 0, 4, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_4);
		glTextureSubImage3D(texObj, 0, 0, 0, 5, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 1,
		                    GL_RGBA, GL_UNSIGNED_BYTE, earth_map_5);
		glGenerateTextureMipmap(texObj);
		glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameterf(texObj, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else { // original style: load an image to the texture unit (and its associated texture object)
		printf("using OpenGL 3.3+ syntax\n");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_1);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_2);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_4);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA, EARTH_MAP_0_WIDTH, EARTH_MAP_0_HEIGHT, 0,
		             GL_RGBA, GL_UNSIGNED_BYTE, earth_map_5);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void triangles(int level, const glm::vec3 a, const glm::vec3 b, const glm::vec3 c) {
	if (numVertSphere >= sizeof(vertSphere) / sizeof(vertSphere[0])) {
		printf("vertex buffer overflow...\n");
		return;
	}
	if (level <= 0) {
		vertSphere[numVertSphere] = glm::vec4(a.x, a.y, a.z, 1.0F);
		vertSphere[numVertSphere + 1] = glm::vec4(b.x, b.y, b.z, 1.0F);
		vertSphere[numVertSphere + 2] = glm::vec4(c.x, c.y, c.z, 1.0F);
		normSphere[numVertSphere] = glm::vec4(a.x, a.y, a.z, 1.0F);
		normSphere[numVertSphere + 1] = glm::vec4(b.x, b.y, b.z, 1.0F);
		normSphere[numVertSphere + 2] = glm::vec4(c.x, c.y, c.z, 1.0F);
		// random color for each face
		glm::vec4 color( 1.0f, 1.0f, 1.0f, 1.0f );
		color.r = ((rand() % 1000) / 1000.0F) * 0.25f + 0.75f;
		color.g = ((rand() % 1000) / 1000.0F) * 0.25f + 0.75f;
		color.b = ((rand() % 1000) / 1000.0F) * 0.25f + 0.75f;
		colorSphere[numVertSphere] = color;
		colorSphere[numVertSphere + 1] = color;
		colorSphere[numVertSphere + 2] = color;
		// texCoord calculation
		texCoordSphere[numVertSphere].s = a.x;
		texCoordSphere[numVertSphere].t = a.y;
		texCoordSphere[numVertSphere].p = a.z;
		texCoordSphere[numVertSphere + 1].s = b.x;
		texCoordSphere[numVertSphere + 1].t = b.y;
		texCoordSphere[numVertSphere + 1].p = b.z;
		texCoordSphere[numVertSphere + 2].s = c.x;
		texCoordSphere[numVertSphere + 2].t = c.y;
		texCoordSphere[numVertSphere + 2].p = c.z;
		// done
		numVertSphere += 3;
	} else {
		glm::vec3 ab = normalize(a + b);
		glm::vec3 bc = normalize(b + c);
		glm::vec3 ca = normalize(c + a);
		// call sub-triangles
		triangles(level - 1, a, ab, ca);
		triangles(level - 1, b, bc, ab);
		triangles(level - 1, c, ca, bc);
		triangles(level - 1, ab, bc, ca);
	}
}

void generateSphere(int level) {
	static glm::vec3 v[6] = {
		{ +1, 0, 0 },
		{ 0, +1, 0 },
		{ -1, 0, 0 },
		{ 0, -1, 0 },
		{ 0, 0, +1 },
		{ 0, 0, -1 },
	};
	numVertSphere = 0;
	triangles(level, v[0], v[1], v[4]);
	triangles(level, v[1], v[2], v[4]);
	triangles(level, v[2], v[3], v[4]);
	triangles(level, v[3], v[0], v[4]);
	triangles(level, v[1], v[0], v[5]);
	triangles(level, v[2], v[1], v[5]);
	triangles(level, v[3], v[2], v[5]);
	triangles(level, v[0], v[3], v[5]);
	printf("totally %d vertices\n", numVertSphere);
}

glm::mat4 mat = glm::mat4( 1.0F ); // mat = matDrag * matModel
glm::mat4 matDrag = glm::mat4( 1.0F );
glm::mat4 matUpdated = glm::mat4( 1.0F );

void keyFunc(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	case GLFW_KEY_R:
		mat = matDrag = matUpdated = glm::mat4( 1.0F ); // reset every thing
		break;
	case GLFW_KEY_1:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		printf("MAG/MIN_FILTER = NEAREST\n");
		break;
	case GLFW_KEY_2:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		printf("MAG/MIN_FILTER = LINEAR\n");
		break;
	case GLFW_KEY_3:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		printf("MIN_FILTER = NEAREST_MIPMAP_NEAREST\n");
		break;
	case GLFW_KEY_4:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		}
		printf("MIN_FILTER = LINEAR_MIPMAP_NEAREST\n");
		break;
	case GLFW_KEY_5:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		}
		printf("MIN_FILTER = NEAREST_MIPMAP_LINEAR\n");
		break;
	case GLFW_KEY_6:
		if (glewIsSupported("GL_VERSION_4_5") && (use_new_tex_syntax == true)) {
			glTextureParameterf(texObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		} else {
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		printf("MIN_FILTER = LINEAR_MIPMAP_LINEAR\n");
		break;
	}
}

int mousePressed = GL_FALSE;
glm::vec2 dragStart; // mouse dragging start point

glm::vec3 calcUnitVec(const glm::vec2& scr) {
	const GLfloat radius = min(WIN_W / 2.0F, WIN_H / 2.0F);
	glm::vec3 v;
	v.x = (scr.x - WIN_W / 2.0F) / radius;
	v.y = (WIN_H / 2.0F - scr.y) / radius;
	v.z = sqrtf(1.0F - v.x * v.x - v.y * v.y);
	return v;
}

glm::mat4 calcTrackball(const glm::vec2& start, const glm::vec2& cur) {
	glm::vec3 org = calcUnitVec( start );
	glm::vec3 dst = calcUnitVec( cur );
	glm::quat q = glm::rotation( org, dst );
	glm::mat4 m = glm::toMat4( q );
	return m;
}

void cursorEnterFunc(GLFWwindow* win, int entered) {
	printf("cursor %s the window\n", (entered == GL_FALSE) ? "leaving" : "entering");
	fflush(stdout);
}

void cursorPosFunc(GLFWwindow* win, double xscr, double yscr) {
	if (mousePressed == GL_TRUE) {
		glm::vec2 dragCur = glm::vec2((GLfloat)xscr, (GLfloat)yscr);
		matDrag = calcTrackball( dragStart, dragCur );
		mat = matDrag * matUpdated;
	}
}

void mouseButtonFunc(GLFWwindow* win, int button, int action, int mods) {
	GLdouble x, y;
	switch (action) {
	case GLFW_PRESS:
		mousePressed = GL_TRUE;
		glfwGetCursorPos(win, &x, &y);
		dragStart = glm::vec2((GLfloat)x, (GLfloat)y);
		break;
	case GLFW_RELEASE:
		mousePressed = GL_FALSE;
		glfwGetCursorPos(win, &x, &y);
		glm::vec2 dragCur = glm::vec2((GLfloat)x, (GLfloat)y);
		matDrag = calcTrackball( dragStart, dragCur );
		mat = matDrag * matUpdated;
		matDrag = glm::mat4(1.0F); // reset
		matUpdated = mat; // update to the object matrix
		break;
	}
	fflush(stdout);
}

void updateFunc(void) {
	// do nothing
}

struct Light {
	glm::vec4 pos;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 att;
};
Light light = {
	{ -3.0f, 3.0f, 3.0f, 1.0f },
	{ 0.2f, 0.2f, 0.2f, 1.0f },
	{ 4.0f, 4.0f, 4.0f, 1.0f },
	{ 7.0f, 7.0f, 7.0f, 1.0f },
	{ 1.0f, 0.2f, 0.2f, 1.0f },
};

struct Material {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shineness;
};
Material material = {
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	32.0f,
};

void setLnM(void) {
	GLuint loc;
	// light
	loc = glGetUniformLocation(prog, "light.pos");
	glUniform4fv(loc, 1, glm::value_ptr(light.pos));
	loc = glGetUniformLocation(prog, "light.ambient");
	glUniform4fv(loc, 1, glm::value_ptr(light.ambient));
	loc = glGetUniformLocation(prog, "light.diffuse");
	glUniform4fv(loc, 1, glm::value_ptr(light.diffuse));
	loc = glGetUniformLocation(prog, "light.specular");
	glUniform4fv(loc, 1, glm::value_ptr(light.specular));
	loc = glGetUniformLocation(prog, "light.att");
	glUniform4fv(loc, 1, glm::value_ptr(light.att));
	// material
	loc = glGetUniformLocation(prog, "material.ambient");
	glUniform4fv(loc, 1, glm::value_ptr(material.ambient));
	loc = glGetUniformLocation(prog, "material.diffuse");
	glUniform4fv(loc, 1, glm::value_ptr(material.diffuse));
	loc = glGetUniformLocation(prog, "material.specular");
	glUniform4fv(loc, 1, glm::value_ptr(material.specular));
	loc = glGetUniformLocation(prog, "material.shineness");
	glUniform1f(loc, material.shineness);
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
	glVertexAttribPointer(locPos, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(vertSphere[0]));
	GLuint locNorm = glGetAttribLocation(prog, "aNorm");
	glEnableVertexAttribArray(locNorm);
	glVertexAttribPointer(locNorm, 4, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(normSphere[0]));
	GLuint locMat = glGetUniformLocation(prog, "uMat");
	glm::mat4 matAccum = mat;
	matAccum = glm::scale( matAccum, glm::vec3(0.9F, 0.9F, 0.9F));
	glUniformMatrix4fv(locMat, 1, GL_FALSE, glm::value_ptr(matAccum));
	// texture
	GLuint locTexCoord = glGetAttribLocation(prog, "aTexCoord");
	glEnableVertexAttribArray(locTexCoord);
	glVertexAttribPointer(locTexCoord, 3, GL_FLOAT, GL_FALSE, 0, glm::value_ptr(texCoordSphere[0]));
	GLuint locTexSampler = glGetUniformLocation(prog, "texSampler");
	glUniform1i(locTexSampler, 0);
	// set L & M properties
	setLnM();
	// draw the sphere
	glDrawArrays(GL_TRIANGLES, 0, numVertSphere);
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
	// argument processing
	if (argc > 1) {
		levelSphere = strtol( argv[1], nullptr, 10 );
	}
	generateSphere( levelSphere );
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
	glfwSetCursorEnterCallback(window, cursorEnterFunc);
	glfwSetCursorPosCallback(window, cursorPosFunc);
	glfwSetMouseButtonCallback(window, mouseButtonFunc);
	glClearColor(0.2F, 0.2F, 0.2F, 1.0F);
	// main loop
	initFunc();
	initTex();
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
