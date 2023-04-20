#version 330 core
#define PI 3.1415926538

in vec3 pos;
//in vec4 color;

uniform float offsetX;

uniform float offsetY;

uniform float time;

uniform float size;


out vec4 ourColor;


float getAngleFromCoordinate(float x, float y) {
	return atan(y / x);
}

vec2 getCoordinateFromAngle(float angle) {
	vec2 temp = vec2(0, 0);
	angle *= PI / 180;
	temp.x = cos(angle);
	temp.y = sin(angle);
	return temp;
}

void main() {
	ourColor = vec4(pos+1, 1.f);

	float angle = getAngleFromCoordinate(pos.x, pos.y);
	float testX = getCoordinateFromAngle(angle+time).x;
	float testY = getCoordinateFromAngle(angle+time).y;


	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

	gl_Position = vec4(x, y, z, 1.0);
}