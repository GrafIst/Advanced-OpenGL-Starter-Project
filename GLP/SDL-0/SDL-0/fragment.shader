#version 330 core
in vec4 ourColor;
out vec4 FragColor;

uniform float r;
uniform float g;
uniform float b;


void main()
{
    FragColor = vec4(ourColor.x*r, ourColor.y*g, ourColor.z*b,1.f);
}
