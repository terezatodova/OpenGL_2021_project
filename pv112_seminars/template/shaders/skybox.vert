#version 450

layout (location = 0) in vec3 actual_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

out vec3 texture_coordinates;

void main() {
    texture_coordinates = actual_position;
    vec4 position = projection_matrix * view_matrix * vec4(actual_position, 1.0);
    gl_Position = position.xyww;
}