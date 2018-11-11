#version 330 core
out vec4 FragColor;

uniform float color;

void main()
{
    FragColor = vec4(color,color,color,1.0);
}