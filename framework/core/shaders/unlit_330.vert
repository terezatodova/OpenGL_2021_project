#version 330 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
layout (location = 0) in vec4 position;  // The vertex position.
layout (location = 1) in vec3 normal;    // The vertex normal.
layout (location = 2) in vec2 tex_coord; // The vertex texture coordinates.
	
uniform mat4 projection;	// The projection matrix.
uniform mat4 view;			// The view matrix.
uniform mat4 model;			// The model matrix.

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// There are no output variables for this shader.

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	gl_Position = projection * view * model * position;
}
