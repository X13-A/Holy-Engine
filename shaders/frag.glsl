varying vec4 v_color;
varying vec2 v_uv;
uniform sampler2D u_sampler0;
uniform sampler2D u_sampler1;

void main(void) 
{
    gl_FragColor = texture2D(u_sampler0, v_uv) * (1.0 - v_uv.x) + texture2D(u_sampler1, v_uv) * v_uv.x;
}