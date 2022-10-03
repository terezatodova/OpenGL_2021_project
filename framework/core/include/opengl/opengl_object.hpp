#pragma once

#include "glad.h"

/**
 * The base class for all wrappers of OpenGL objects (such as Textures, Buffers, etc.). This idea is that this
 * construct simplifies the work with these native OpenGL objects and shields students from the pure OpenGL in some
 * courses.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan By�ka</a>
 */
class OpenGLObject {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The corresponding OpenGL object. */
    GLuint opengl_object = 0;

    /** The target to which this object can be bound (i.e., the target used by the glBind*) methods. */
    GLenum target;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new {@link OpenGLObject} including the OpenGL counterpart.
     *
     * @param 	target	Target for the.
     */
    OpenGLObject(GLenum target) : target(target) {}

    /**
     * Copy constructor. Performs a deep copy the OpenGL object.
     *
     * @param other
     */
    // OpenGLObject(const OpenGLObject& other) {}

    /**
     * Copy assignment. Performs a deep copy the OpenGL object.
     *
     * @param other
     */
    // OpenGLObject& operator=(OpenGLObject other) {}
    // ^ Use this form with copy-and-swap idiom!

    /**
     * Move constructor. Moves the OpenGL object ID to the new object. Resets the ID of the old object to 0 so that
     * OpenGL's destructor does nothing.
     *
     * @param other
     */
    // OpenGLObject(OpenGLObject&& other) {}
    // Move assignment is automatic when there is swap idiom + copy construct + move constructor

    /**
     * Destroys this {@link OpenGLObject}. Note that the OpenGL counterpart is also destroyed.
     */
    virtual ~OpenGLObject() {}

  public:
    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
    /** Uploads the data from CPU to GPU. */
    void virtual update_opengl_data() const = 0;

    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
  public:
    /**
     * Returns the OpenGL counterpart of this object.
     *
     * @return	The OpenGL object.
     */
    GLuint get_opengl_object() const { return opengl_object; }
};