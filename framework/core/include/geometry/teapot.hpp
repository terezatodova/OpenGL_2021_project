#pragma once

#include "geometry.hpp"
#include "teapot.inl"

/** This class represents a geometry for a default teapot model. */
class Teapot : public Geometry {
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Creates a new @link Teapot such that the center of the bottom of its body is roughly in (0,0,0) and the radius of
     * the body is roughly 1. Its handle is in -X direction, its spout is in +X direction, and its lid is in +Y
     * direction.
     * 
     * @param 	position_loc 	The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc   	The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc	The location of texture coordinates vertex attribute for the VAO (use -1 if not
     * 							necessary).
     * @param 	tangent_loc  	The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc	The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     *
     * ### return	The created geometry.
     */
    Teapot(GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
           GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
           GLint bitangent_loc = DEFAULT_BITANGENT_LOC) : Geometry(GL_TRIANGLE_STRIP, 14, teapot_vertices_count, teapot_vertices, teapot_indices_count, teapot_indices) {
}

};
