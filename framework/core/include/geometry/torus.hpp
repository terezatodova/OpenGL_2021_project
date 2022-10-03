#pragma once

#include "geometry.hpp"
#include "torus.inl"

/** This class represents a geometry for a default torus model. */
class Torus : public Geometry {
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Creates a torus whose center is in (0,0,0), its height is 1 (y coordinates are from -0.5 to 0.5), and its width
     * is 3 (x/z coordinates are from -1.5 to 1.5).
     *
     * @param 	position_loc 	The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc   	The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc	The location of texture coordinates vertex attribute for the VAO (use -1 if not
     * 							necessary).
     * @param 	tangent_loc  	The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc	The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     *
     * @return	The created geometry.
     */
    Torus(GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
          GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
          GLint bitangent_loc = DEFAULT_BITANGENT_LOC)
        : Geometry(GL_TRIANGLE_STRIP, 14, torus_vertices_count, torus_vertices, torus_indices_count, torus_indices) {
    }
};
