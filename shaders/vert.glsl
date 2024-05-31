attribute vec3 a_position;
attribute vec3 a_color;
attribute vec2 a_uv;

varying vec4 v_color;
varying vec2 v_uv;

void main(void) 
{
    gl_Position = vec4(a_position, 1.0);
    v_color = vec4(a_color, 1.0);
    v_uv = a_uv;
}
