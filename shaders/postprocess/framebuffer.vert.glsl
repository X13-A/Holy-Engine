#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;
out vec3 viewDir;

uniform mat4 cameraInvProjection;
uniform mat4 cameraInvView;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    texCoords = inTexCoords;
   
    // Compute view direction in world space
    vec4 clipCoords = vec4(inTexCoords * 2.0 - 1.0, 0.0, 1.0);
    vec4 viewCoords = cameraInvProjection * clipCoords;
    viewCoords /= viewCoords.w;
    vec4 worldCoords = cameraInvView * vec4(viewCoords.xyz, 0.0);
    viewDir = normalize(worldCoords.xyz);
}
