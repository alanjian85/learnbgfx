$input a_position
$output v_position

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_position = a_position;
}