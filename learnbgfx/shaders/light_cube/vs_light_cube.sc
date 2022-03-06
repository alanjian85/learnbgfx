$input a_position, a_color0, a_texcoord0

#include <bgfx_shader.sh>

void main() {
    gl_Position = u_modelViewProj * vec4(a_position, 1.0);
}