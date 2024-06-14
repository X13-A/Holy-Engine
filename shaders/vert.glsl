attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;

varying vec3 v_normal;
varying vec2 v_uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
    v_normal = a_normal;
    v_uv = a_uv;
}
