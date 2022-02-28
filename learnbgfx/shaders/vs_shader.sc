$input a_position, a_color0
$output v_color0

uniform vec4 u_params;

#define u_offset u_params.x

void main() {
    gl_Position = vec4(a_position, 1.0);
    gl_Position.x = gl_Position.x + u_offset;
    v_color0 = a_color0;
}