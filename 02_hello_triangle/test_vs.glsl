#version 410

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertex_colour;

out vec3 colour;

void main() {
	colour = vertex_colour;
	gl_Position = vec4(aPos, 1.0);
}
