#version 450

const vec2 square[4] = {
	vec2(-5.0, -5.0),
	vec2(-5.0, 5.0),
	vec2(5.0, -5.0),
	vec2(5.0, 5.0)
};

void main()
{
    gl_Position = vec4(square[gl_VertexID], 0.0, 1.0);
}
