#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma warning(disable: 4711 4710 4100)
#include <stdio.h>
#include <string.h> // for strrchr()

#include "./common.c"

const unsigned int WIN_W = 500; // window size in pixels, (Width, Height)
const unsigned int WIN_H = 500;
const unsigned int WIN_X = 100; // window position in pixels, (X, Y)
const unsigned int WIN_Y = 100;

const char* vertSource =
"#version 330 core \n\
in vec4 vertexPos; \n\
void main(void) { \n\
   gl_Position = vertexPos; \n\
}";

const char* fragSource =
"#version 330 core \n\
out vec4 FragColor; \n\
void main(void) { \n\
   FragColor = vec4(1.0, 1.0, 0.0, 1.0); \n\
}";

GLuint vert = 0; // vertex shader ID number
GLuint frag = 0; // fragment shader ID number
GLuint prog = 0; // shader program ID number

void initFunc(void) {
    // vert: vertex shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSource, NULL);
    glCompileShader(vert); // compile to get .OBJ
    // frag: fragment shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSource, NULL);
    glCompileShader(frag); // compile to get .OBJ
    // prog: program
    prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog); // link to get .EXE
    // execute it!
    glUseProgram(prog);
}

GLfloat vertPos1[] = {
   -0.8F, -0.8F, 0.0F, 1.0F,
   +0.7F, -0.8F, 0.0F, 1.0F,
   -0.8F, +0.7F, 0.0F, 1.0F,
};


GLfloat vertPos2[] = {
   +0.8F, +0.8F, 0.0F, 1.0F,
   +0.8F, -0.7F, 0.0F, 1.0F,
   -0.7F, +0.8F, 0.0F, 1.0F,
};


//GLfloat vertPos1[] = {
//    -0.5F, -0.5F, 0.0F, 1.0F,
//    +0.0F, -0.5F, 0.0F, 1.0F,
//    -0.5F, +0.0F, 0.0F, 1.0F,
//};
//
//GLfloat vertPos2[] = {
//    0.5F, -0.5F, 0.0F, 1.0F,
//    0.0F, -0.5F, 0.0F, 1.0F,
//    0.5F, 0.0F, 0.0F, 1.0F,
//};

GLfloat vertColor[] = {
    1.0F, 1.0F, 0.0F, 1.0F,
    1.0F, 1.0F, 0.0F, 1.0F,
    0.0F, 0.0F, 1.0F, 1.0F, 
};

void drawFunc(void) {
    // clear in gray color
    glClear(GL_COLOR_BUFFER_BIT);
    // provide the vertex attributes
    GLuint loc1 = glGetAttribLocation(prog, "vertexPos");
    glEnableVertexAttribArray(loc1);
    glVertexAttribPointer(loc1, 4, GL_FLOAT, GL_FALSE, 0, vertPos1);
    // draw a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 두 번째 삼각형 그리기
    GLuint loc2 = glGetAttribLocation(prog, "vertexPos");
    glEnableVertexAttribArray(loc2);
    glVertexAttribPointer(loc2, 4, GL_FLOAT, GL_FALSE, 0, vertPos2);
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
        if (action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        break;
    }
}

int main(int argc, char* argv[]) {
    const char* basename = getBaseName(argv[0]);
    // start GLFW & GLEW
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, basename, NULL, NULL);
    glfwSetWindowPos(window, WIN_X, WIN_Y);
    glfwMakeContextCurrent(window);
    glewInit();
    // prepare
    glfwSetWindowRefreshCallback(window, refreshFunc);
    glfwSetKeyCallback(window, keyFunc);
    glClearColor(0.0F, 0.0F, 1.0F, 1.0F);
    // main loop
    initFunc();
    while (!glfwWindowShouldClose(window)) {
        // draw
        drawFunc();
        // GLFW actions
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // done
    glfwTerminate();
    return 0;
}