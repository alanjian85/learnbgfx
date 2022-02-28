$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texture1, 0);
SAMPLER2D(s_texture2, 1);

uniform vec4 u_params;

#define u_mix_value u_params.x

void main() {
    gl_FragColor = mix(texture2D(s_texture1, v_texcoord0), texture2D(s_texture2, v_texcoord0), u_mix_value);
}