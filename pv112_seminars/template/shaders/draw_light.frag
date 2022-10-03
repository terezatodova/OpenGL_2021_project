#version 450


layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_color;

layout(location = 0) out vec4 final_color;

void main()
{
    vec3 result = fs_color / (fs_color + 1.0); // tone mapping
    result = pow(result, vec3(1.0 / 2.2));     // gamma correction
	final_color = vec4(result, 1.0);
}
