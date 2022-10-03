#pragma once

#include "glad.h"
#include "opengl_object.hpp"
#include <span>
#include <vector>

/**
 * The base class for representing uniform buffer objects.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 * @author	<a href="mailto:cejka.honza@gmail.com ">Jan Čejka</a>
 */
template <class T> class UBO : public OpenGLObject {
    // ----------------------------------------------------------------------------
    // Static Variables
    // ----------------------------------------------------------------------------
  public:
    // The following constants define the default binding points of common uniform blocks.
    // Make sure these numbers corresponds to layout (binding=N) in shaders (or use glUniformBlockBinding).
    /** The default binding for UBOs with the camera data. */
    const static int DEFAULT_CAMERA_BINDING = 0;
    /** The default binding for UBOs with the model data. */
    const static int DEFAULT_MODEL_BINDING = 1;
    /** The default binding for UBOs with the lights data. */
    const static int DEFAULT_LIGHTS_BINDING = 2;
    /** The default binding for UBOs with the material data. */
    const static int DEFAULT_MATERIAL_BINDING = 3;

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The buffer data. */
    std::vector<T> data;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /** Constructs a new @link UBO and initializes the GPU buffer with data. */
    UBO(GLbitfield flags = 0) : OpenGLObject(GL_UNIFORM_BUFFER), data(1), flags(flags) {
        glCreateBuffers(1, &opengl_object);
        glNamedBufferStorage(opengl_object, sizeof(T) * data.size(), data.empty() ? nullptr : data.data(), flags);
    }

    /** Constructs a new @link UBO and initializes the GPU buffer with data. */
    UBO(std::span<T> data, GLbitfield flags = 0) : OpenGLObject(GL_UNIFORM_BUFFER), data(data), flags(flags) {
        glCreateBuffers(1, &opengl_object);
        glNamedBufferStorage(opengl_object, sizeof(T) * data.size(), data.empty() ? nullptr : data.data(), flags);
    }

    /**
     * @brief Copy constructor. Performs a deep copy of the UBO.
     *
     * @param other
     */
    UBO(const UBO& other) : OpenGLObject(other.target), data(other.data), flags(0) {
        glCreateBuffers(1, &opengl_object);
        glNamedBufferStorage(opengl_object, sizeof(T) * data.size(), data.empty() ? nullptr : data.data(), flags);
    }

    /**
     * @brief Copy assignment. Performs a deep copy of the UBO.
     *
     * @param other
     */
    UBO& operator=(UBO other) {
        swap(*this, other);

        return *this;
    }

    /**
     * @brief Move constructor. Moves the Buffer to the new object. Zeroes out the old object so that
     * OpenGL's destructor does nothing.
     *
     * @param other
     */
    UBO(UBO&& other) : UBO() { swap(*this, other); }

    friend void swap(UBO& first, UBO& second) noexcept {
        using std::swap;

        swap(first.opengl_object, second.opengl_object);
        swap(first.target, second.target);
        swap(first.data, second.data);
    }

    /**
     * Destroys this @link UBO. Note that we do not delete the OpenGL counterpart in order to avoid problems when
     * the object is copied locally and then it goes out of scope.
     */
    virtual ~UBO() { glDeleteBuffers(1, &opengl_object); }

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
  public:
    /**
     * Binds the buffer object to a specified indexed buffer target (binding point within the array specified by @link target).
     *
     * @param 	index	The binding point within the array specified by @link target.
     */
    void bind_buffer_base(GLuint index) const { glBindBufferBase(target, index, opengl_object); }

    /** Copies the data from CPU to GPU. */
    void update_opengl_data() const override {
        // Crash if the buffer disallows copying
        assert((flags & GL_DYNAMIC_STORAGE_BIT) == GL_DYNAMIC_STORAGE_BIT);

        if (!data.empty()) {
            glNamedBufferSubData(opengl_object, 0, sizeof(T) * data.size(), data.data());
        }
    }

  private:
    GLbitfield flags;
};