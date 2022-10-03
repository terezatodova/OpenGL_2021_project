#pragma once

#include "iapplication.hpp"
#include "utilities.hpp"

#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "imgui.h"
#include <glad/glad.h>

class PV112Application : public IApplication {

  public:
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------
    PV112Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------
    /**
     * @copydoc IApplication::update
     */
    virtual void update(float delta) override {}

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
    /**
     *  @copydoc IApplication::render
     */
    virtual void render() override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
    /**
     * @copydoc IApplication::render_ui
     */
    virtual void render_ui() override {}

    /**
     * @brief Compiles shaders and deletes previous ones.
     *
     */
    virtual void compile_shaders() {}

    /**
     * @brief Deletes shaders. Called in destructor as well as in `compile_shaders` at the beginning.
     *
     */
    virtual void delete_shaders() {}

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------
    /**
     * @copydoc IApplication::on_resize
     */
    virtual void on_resize(int width, int height) override {
        this->width = std::max(width, 1);
        this->height = std::max(height, 1);

        glViewport(0, 0, width, height);
    }

    /**
     * @copydoc IApplication::on_mouse_move
     */
    virtual void on_mouse_move(double x, double y) override {}

    /**
     * @copydoc IApplication::on_mouse_button
     */
    virtual void on_mouse_button(int button, int action, int mods) override {}

    /**
     * @copydoc IApplication::on_key_pressed
     */
    virtual void on_key_pressed(int key, int scancode, int action, int mods) override {
        if (action == GLFW_PRESS) {
            switch (key) {
            case GLFW_KEY_R:            
                compile_shaders();
                break;
            }
        }
        IApplication::on_key_pressed(key, scancode, action, mods);
    }
};
