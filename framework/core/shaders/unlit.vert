#version 430 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
layout (location = 0) in vec4 position;  // The vertex position.
layout (location = 1) in vec3 normal;    // The vertex normal.
layout (location = 2) in vec2 tex_coord; // The vertex texture coordinates.
	
uniform mat4 projection;	// The projection matrix.
uniform mat4 view;			// The view matrix.

// The UBO with the model data.
layout (std140, binding = 1) uniform ModelData
  {
   mat4 model;			// The model matrix.
   mat4 model_inv;		// The inverse of the model matrix.
   mat3 model_it;		// The inverse of the transpose of the top-left part 3x3 of the model matrix.
  };

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
