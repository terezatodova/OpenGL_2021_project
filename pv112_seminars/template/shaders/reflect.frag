#version 450

layout(binding = 0, std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
}
camera;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;

uniform samplerCube skybox;

layout(location = 0) out vec4 final_color;

void main() {
    vec3 I = normalize(fs_position - camera.position);
    vec3 R = reflect(I, normalize(fs_normal));
    final_color = vec4(texture(skybox, R).rgb, 1.0);
}
