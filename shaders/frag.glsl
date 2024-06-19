#version 330 core

in vec3 v_normal;
in vec2 v_uv;
in vec3 v_pos;
in vec3 v_cameraPos; // Receive computed camera position from vertex shader

out vec4 FragColor;

uniform sampler2D u_sampler0;
uniform sampler2D u_sampler1;

void main(void) 
{
    vec3 fractionalPos = fract(v_pos); // Get the fractional part of the position
    FragColor = vec4(fractionalPos, 1.0); // Set the fragment color to the fractional part of the position
    //FragColor = vec4(v_cameraPos * 10000, 1.0);
    return;

    // Uncomment and adjust for lighting or other effects if needed
    // vec3 lightDir = vec3(0, -1, 0);
    // float light = clamp(dot(lightDir, -v_normal), 0.0, 1.0) + 0.2;
    // FragColor = vec4(light, light, light, 1.0);
    // FragColor = vec4(v_normal, 1.0);

    // Example of using the camera position in a calculation
    // vec3 viewDir = normalize(v_cameraPos - v_pos);
    // float viewFactor = dot(viewDir, v_normal);
    // FragColor = vec4(viewFactor, viewFactor, viewFactor, 1.0);
}
