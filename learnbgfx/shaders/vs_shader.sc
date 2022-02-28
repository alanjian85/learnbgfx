$input a_position
$output v_color0

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_color0 = vec4(0.5, 0.0, 0.0, 1.0);
}