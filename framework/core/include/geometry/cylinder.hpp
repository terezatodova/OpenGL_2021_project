#pragma once

#include "geometry.hpp"
#include "cylinder.inl"

/** This class represents a geometry for a default cylinder model. */
class Cylinder : public Geometry {
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Creates a cylinder whose center is in (0,0,0), its height is 2 (y coordinates are from -1 to 1), and its radius
     * is 0.5 (x/z coordinates are from -0.5 to 0.5)
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
    Cylinder(GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
             GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
             GLint bitangent_loc = DEFAULT_BITANGENT_LOC)
        : Geometry(GL_TRIANGLE_STRIP, 14, cylinder_vertices_count, cylinder_vertices, cylinder_indices_count, cylinder_indices) {
    }
};
