#pragma once

#include "color.hpp"
#include "geometry.hpp"
#include "glm/glm.hpp"
#include "model_ubo.hpp"
#include "program.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * The simple representation of a scene object.
 * @author	<a href="mailto:jan.byska@gmail.com">Jan By�ka</a>
 */
class SceneObject {
    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The UBO containing the model data. */
    ModelUBO model_ubo;
    /** The geometry representation of the scene object. */
    Geometry geometry;
    /** The current color. */
    Color color;
    /** The current position. */
    glm::vec3 position;
    /** The current scale.*/
    glm::vec3 scale;
    /** The current pitch rotation (in radians). */
    float pitch;
    /** The current yaw rotation (in radians). */
    float yaw;
    /** The current roll rotation (in radians). */
    float roll;

    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link SceneObject with custom values.
     *
     * @param 	geometry	The geometry.
     * @param 	color   	The color.
     */
    SceneObject(const Geometry& geometry, const Color& color) : SceneObject(geometry, color, glm::vec3(0)) {}

    /**
     * Constructs a new @link SceneObject with custom values.
     *
     * @param 	geometry	The geometry.
     * @param 	color   	The color.
     * @param 	position	The position.
     */
    SceneObject(const Geometry& geometry, const Color& color, const glm::vec3& position)
        : SceneObject(geometry, color, position, glm::vec3(1)) {}

    /**
     * Constructs a new @link SceneObject with custom values.
     *
     * @param 	geometry	The geometry.
     * @param 	color   	The color.
     * @param 	position	The position.
     * @param 	scale   	The scale.
     */
    SceneObject(const Geometry& geometry, const Color& color, const glm::vec3& position, const glm::vec3& scale)
        : SceneObject(geometry, color, position, scale, 0, 0, 0) {}

    /**
     * Constructs a new @link SceneObject with custom values.
     *
     * @param 	geometry	The geometry.
     * @param 	color   	The color.
     * @param 	position	The position.
     * @param 	scale   	The scale.
     * @param 	pitch   	The pitch rotation (in radians).
     * @param 	yaw			The yaw rotation (in radians).
     * @param 	roll		The roll rotation (in radians).
     */
    SceneObject(const Geometry& geometry, const Color& color, const glm::vec3& position, const glm::vec3& scale, float pitch,
                float yaw, float roll)
        : geometry(geometry), color(color), position(position), scale(scale), pitch(pitch), yaw(yaw), roll(roll) {
        build_matrix();
    }

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
    /** Renders the object into the current OpenGL context using a specified program. */
    void draw(ShaderProgram& program) const {
        if (program.is_valid()) {
            program.use();

            program.uniform("mat_diffuse", color.r, color.g, color.b, color.a);
            model_ubo.bind_buffer_base(ModelUBO::DEFAULT_MODEL_BINDING);
            geometry.bind_vao();
            geometry.draw();
        }
    }
    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
  public:
    /**
     * Sets a new position for the object.
     *
     * @param 	position	The position to set.
     */
    void set_position(glm::vec3 position) {
        this->position = position;
        build_matrix();
    }

    /**
     * Sets a new scale for the object.
     *
     * @param 	scale	The scale to set.
     */
    void set_scale(glm::vec3 scale) {
        this->scale = scale;
        build_matrix();
    }

    /**
     *  Sets a new pitch rotation (in radians) for the object.
     *
     * @param 	rotation	The rotation to set (in radians).
     */
    void set_pitch_rotation(float rotation) {
        this->pitch = rotation;
        build_matrix();
    }

    /**
     *  Sets a new yaw rotation (in radians) for the object.
     *
     * @param 	rotation	The rotation to set (in radians).
     */
    void set_yaw_rotation(float rotation) {
        this->yaw = rotation;
        build_matrix();
    }

    /**
     *  Sets a new roll rotation (in radians) for the object.
     *
     * @param 	rotation	The rotation to set (in radians).
     */
    void set_roll_rotation(float rotation) {
        this->roll = rotation;
        build_matrix();
    }

  private:
    /** Builds the model matrix based on the current position, scale, and rotation. */
    void build_matrix() {
        model_ubo.set_matrix(glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale) *
                             glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f)) *
                             glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
                             glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f)));
        model_ubo.update_opengl_data();
    }
};
