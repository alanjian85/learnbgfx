$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texture1, 0);
SAMPLER2D(s_texture2, 1);

void main() {
    gl_FragColor = mix(texture2D(s_texture1, v_texcoord0 * 2.0), texture2D(s_texture2, v_texcoord0 * 2.0), 0.2);
}