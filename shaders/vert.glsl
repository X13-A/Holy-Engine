#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_normal;
out vec2 v_uv;
out vec3 v_pos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    v_normal = a_normal; // TODO: use inverse transpose of modelMatrix
    v_uv = a_uv;
    v_pos = (modelMatrix * vec4(a_position, 1.0)).xyz;
}
