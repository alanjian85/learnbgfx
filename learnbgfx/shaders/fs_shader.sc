$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texture1, 0);
SAMPLER2D(s_texture2, 1);

void main() {
    gl_FragColor = mix(texture2D(s_texture1, v_texcoord0), texture2D(s_texture2, vec2(1.0 - v_texcoord0.x, v_texcoord0.y)), 0.2);
}