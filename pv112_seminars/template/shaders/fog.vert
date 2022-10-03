#version 450

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

layout(binding = 1, std140) uniform Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} light;

layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;
layout(location = 3) out float visibility;

void main()
{
	fs_position = vec3(object.model_matrix * vec4(position, 1.0));
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	fs_texture_coordinate = texture_coordinate;
	
    float density = 0.007;
    float gradient = 1.5;
    vec4 position_to_cam = camera.view * vec4(fs_position, 1.0);
    float dist = length(position_to_cam.xyz);
    visibility = exp(-pow((dist*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    gl_Position = camera.projection * camera.view * object.model_matrix * vec4(position, 1.0);
}
