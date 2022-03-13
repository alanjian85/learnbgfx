$input a_position, a_normal
$output v_position, v_normal

#include "common.sh"

void main() {
    gl_Position = u_modelViewProj * vec4(a_position, 1.0);
    
    v_position = vec3(u_model[0] * vec4(a_position, 1.0));
    v_normal = normalize(mat3(transpose(inverse(u_model[0]))) * a_normal);
}