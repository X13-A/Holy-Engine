#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) 
{
    gl_Position = projectionMatrix * viewMatrix * vec4(a_position, 1.0);
}
