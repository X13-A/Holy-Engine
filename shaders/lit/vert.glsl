#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec4 FragPosLightSpace;
out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 lightMatrix;

void main(void) 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    Normal = (normalMatrix * vec4(a_normal, 1)).xyz;
    TexCoords = a_uv;
    WorldPos = (modelMatrix * vec4(a_position, 1.0)).xyz;
    FragPosLightSpace = lightMatrix * vec4(WorldPos, 1.0);
}
