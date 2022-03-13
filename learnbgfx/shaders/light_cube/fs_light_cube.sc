#include "common.sh"

uniform vec4 u_light_color;

void main() {
    gl_FragColor = u_light_color;
}