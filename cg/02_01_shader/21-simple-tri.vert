#version 330 core

in vec4 vertexPos; // attribute

void main() {
	gl_Position = vertexPos;
}
