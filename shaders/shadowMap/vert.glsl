#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) 
{
    float trash = (a_position.x + a_normal.x + a_uv.x) * 0; // HACK: use vertex attributes so they don't get optimized out...
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0 + trash);
}
