#include <bgfx_shader.sh>

uniform vec4 u_objectColor;
uniform vec4 u_lightColor;

void main() {
    gl_FragColor = u_objectColor * u_lightColor;
}