varying vec3 v_normal;
varying vec2 v_uv;
uniform sampler2D u_sampler0;
uniform sampler2D u_sampler1;

void main(void) 
{
    vec3 lightDir = vec3(0, -1, 0);
    float light = clamp(dot(lightDir, -v_normal), 0.0, 1.0) + 0.2;
    //gl_FragColor = texture2D(u_sampler0, v_uv) * (1.0 - v_uv.x) + texture2D(u_sampler1, v_uv) * v_uv.x;
    gl_FragColor = vec4(light, light, light, 1);
    //gl_FragColor = vec4(v_normal, 1);
}