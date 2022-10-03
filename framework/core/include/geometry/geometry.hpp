#pragma once

#include "geometry_base.hpp"
#include "glad.h"
#include "glm/glm.hpp"
#include "model_ubo.hpp"
#include "program.hpp"
#include <filesystem>
#include <vector>

/**
 * The implementation of the Geometry_Base class providing the
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 * @author	<a href="mailto:matus.talcik@gmail.com">Matúš Talčík</a>
 */
class Geometry : public Geometry_Base {

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:

    /** Creates a new empty @link Geometry object. */
    Geometry() : Geometry_Base(GL_POINTS, 0, 0, 0) {}

    /**
     * Creates a @link Geometry object.
     * @param   mode                The mode that will be used for rendering the geometry.
     * @param   vertex_buffer_size  The number of float values stored in the buffer for each vertex.
     * @param 	vertices_count	    The number of vertices that the created geometry will have.
     * @param 	vertices	    The actual geometry vertices.
     * @param 	indices_count 	    The number of indices wihing the geometry.
     * @param 	indices		    The actual indices.
     * @param 	position_loc  	    The location of position vertex attribute for the VAO (use -1 if not necessary).
     * @param 	normal_loc	    The location of normal vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tex_coord_loc 	    The location of texture coordinates vertex attribute for the VAO (use -1 if not necessary).
     * @param 	tangent_loc   	    The location of tangent vertex attribute for the VAO (use -1 if not necessary).
     * @param 	bitangent_loc 	    The location of bitangent vertex attribute for the VAO (use -1 if not necessary).
     */
    Geometry(GLenum mode, int vertex_buffer_size, int vertices_count, const float* vertices, int indices_count,
             const unsigned int* indices, GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
             GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
             GLint bitangent_loc = DEFAULT_BITANGENT_LOC);

    Geometry(GLenum mode, int elements_per_vertex, std::vector<float> interleaved_vertices, std::vector<uint32_t> indices = {},
             GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
             GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
             GLint bitangent_loc = DEFAULT_BITANGENT_LOC);

    Geometry(GLenum mode, std::vector<float> positions, std::vector<float> normals, std::vector<float> tex_coords,
             std::vector<float> tangents = {}, std::vector<float> bitangents = {}, std::vector<uint32_t> indices = {},
             GLint position_loc = DEFAULT_POSITION_LOC, GLint normal_loc = DEFAULT_NORMAL_LOC,
             GLint tex_coord_loc = DEFAULT_TEX_COORD_LOC, GLint tangent_loc = DEFAULT_TANGENT_LOC,
             GLint bitangent_loc = DEFAULT_BITANGENT_LOC);

    static Geometry from_file(std::filesystem::path file_path);

    /**
     * Creates a new @link Geometry object from another geometry performing a deep copy.
     *
     * @param 	other	The other geometry.
     */
    Geometry(const Geometry& other);

    Geometry(Geometry&& other) : Geometry() { swap(*this, other); };

    /**
     * Destroys this @link Geometry. Note that we do not delete the OpenGL counterparts in order to avoid problems when
     * the object is copied locally and then it goes out of scope.
     */
    virtual ~Geometry();

    // ----------------------------------------------------------------------------
    // Operators
    // ----------------------------------------------------------------------------
  public:
    /**
     * The assignment operator.
     *
     * @param 	rhs	The right hand side.
     *
     * @return	A shallow copy of this object.
     */
    Geometry& operator=(Geometry other) {
        swap(*this, other);

        return *this;
    };

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
  private:
    /** Initialize Vertex Array Object for the geometry. */
    void init_vao();
};
