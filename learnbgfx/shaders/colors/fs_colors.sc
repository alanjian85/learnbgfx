$input v_color0

#include "common.sh"

uniform vec4 u_objectColor;

void main() {
    gl_FragColor = vec4(v_color0 * vec3(u_objectColor), 1.0);
}