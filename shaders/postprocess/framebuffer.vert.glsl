#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out vec2 texCoords;
out vec3 viewDir;

uniform mat4 cameraInvProjection;
uniform mat4 cameraInvView;

void main()
{
    // Pass through the position to the fragment shader
    gl_Position = vec4(inPos, 0.0, 1.0);

    // Pass through the texture coordinates
    texCoords = inTexCoords;

    // Transform UV coordinates to clip space
    vec4 clipCoords = vec4(inTexCoords * 2.0 - 1.0, 0.0, 1.0);

    // Transform from clip space to view space
    vec4 viewCoords = cameraInvProjection * clipCoords;
    
    // Perspective divide to get the correct view space coordinates
    viewCoords /= viewCoords.w;

    // Transform from view space to world space
    vec4 worldCoords = cameraInvView * vec4(viewCoords.xyz, 0.0);

    // Normalize the direction vector
    viewDir = normalize(worldCoords.xyz);
}
