#version 330 core
in vec3 pos;
//in vec4 color;

uniform float offset;

uniform float dir;

out vec4 ourColor;


void main() {
	ourColor = vec4(pos, 1.f);

	float x = pos.x;
	float y = pos.y + (offset*dir);
	float z = pos.z;

	gl_Position = vec4(y, x, z, 1.0);
}