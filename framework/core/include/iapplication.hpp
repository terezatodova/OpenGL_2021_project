#pragma once

#define GLFW_INCLUDE_NONE // prevents the GLFW header from including the OpenGL header
#include <GLFW/glfw3.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "geometry.hpp"
#include "program.hpp"
#include "scene/scene_object.hpp"
#include "texture.hpp"

// Includes the most important GLM functions.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/color_space.hpp>
#include "imgui.h"
#include "utils/configuration.hpp"

/**
 * The base class for all OpenGL windows.
 *
 * @author	<a href="mailto:jan.byska@gmail.com">Jan Byška</a>
 * @author	<a href="mailto:matus.talcik@gmail.com">Matúš Talčík</a>
 */
class IApplication {

  protected:
    /** The current window width. */
    GLsizei width;

    /** The current window height. */
    GLsizei height;

    /** Command-line arguments. */
    std::vector<std::string> arguments;

    /** Configuration loaded from a file configuration.toml. */
    Configuration configuration;

    /**
     * Path to framework's shaders. Loaded from {@link configuration} if a configuration file is available. 
     * Otherwise working directory of a binary is used.
     */
    std::filesystem::path framework_shaders_path;

    /**
     * Path to applications shaders. Loaded from {@link configuration} if a configuration file is available. 
     * Otherwise working directory of a binary is used.
     */
    std::filesystem::path shaders_path;

    GLFWwindow* window = nullptr;

  public:
    // ----------------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------------

    /**
     * Constructs a new @link IApplication with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The arguments used to obtain the application directory.
     */
    IApplication(int initial_width, int initial_height, std::vector<std::string> arguments);

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------
    /**
     * This method is invoked when the window size changes.
     *
     * The implementation is left to the child class.
     *
     * @param 	width 	The new width.
     * @param 	height	The new height.
     */
    virtual void on_resize(int width, int height) = 0;

    /**
     * This method is invoked when the mouse cursor is moved.
     *
     * The implementation is left to the child class.
     *
     * @param 	x	The X-coordinate of the cursor relative to the upper-left corner of the window.
     * @param 	y	The Y-coordinate of the cursor relative to the upper-left corner of the window.
     */
    virtual void on_mouse_move(double x, double y) = 0;

    /**
     * This method is invoked when a mouse button is pressed or released.
     *
     * The implementation is left to the child class.
     *
     * @param 	button	The mouse button that was pressed or released.
     * @param 	action	One of GLFW_PRESS or GLFW_RELEASE.
     * @param 	mods  	Bit field describing which modifier keys were held down.
     */
    virtual void on_mouse_button(int button, int action, int mods) = 0;

    /**
     * This method is invoked when a keyboard key is pressed, released, or repeated.
     * The default implementation passes the input to ImGui.
     *
     * @param 	key		    The keyboard key that was pressed or released.
     * @param 	scancode	The system-specific scancode of the key.
     * @param 	action  	One of GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
     * @param 	mods		Bit field describing which modifier keys were held down.
     */
    virtual void on_key_pressed(int key, int scancode, int action, int mods) {
        ImGui::GetIO().KeysDown[key] = action;
    }

    // ----------------------------------------------------------------------------
    // Update
    // ----------------------------------------------------------------------------

    /**
     * This method is invoked from within the infinite OpenGL loop. The purpose of this method is to update the
     * application specific data before rendering the content.
     *
     * @param 	delta	The elapsed time between this and the previous frame (in milliseconds).
     */
    virtual void update(float delta) = 0;

    // ----------------------------------------------------------------------------
    // Render
    // ----------------------------------------------------------------------------
    /**
     * This method is invoked from within the infinite OpenGL loop. The main purpose of this method is to
     * render the application specific content.
     */
    virtual void render() = 0;

    // ----------------------------------------------------------------------------
    // GUI
    // ----------------------------------------------------------------------------
    /**
     * This method is invoked from within the infinite OpenGL loop. The main purpose of this method is to
     * render the application specific GUI.
     */
    virtual void render_ui() = 0;

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
  public:
    /**
     * Activates a custom frame buffer. The methods also automatically sets the full screen size viewport and possibly
     * also clears the buffer and sets the depth test.
     *
     * @param 	buffer	  	The buffer to activate (0 is the main screen).
     * @param 	clear	  	If @p true the bounded frame buffer will be cleared.
     * @param 	depth_test	if @p true the depth test will be activated; if @p false the depth test will be deactivated.
     */
    void activate_render_buffer(GLint buffer = 0, bool clear = true, bool depth_test = true) const {
        // Binds the frame buffer.
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);

        // Clears the color and depth attachments (this is also fine if there is no color attachment).
        if (clear) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        // Activates (or deactivates) the depth test.
        if (depth_test) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    /**
     * Activates a custom-sized viewport.
     *
     * @param 	x	   	The X-coordination of the lower left corner of the viewport rectangle (0 by default).
     * @param 	y	   	The Y-coordination of the lower left corner of the viewport rectangle (0 by default).
     * @param 	width	The width of the viewport (value<=0 is used to indicate full screen).
     * @param 	height	The height of the viewport (value<=0 is used to indicate full screen).
     */
    void activate_viewport(GLint x = 0, GLint y = 0, GLsizei width = 0, GLsizei height = 0) const {
        glViewport(x, y, width > 0 ? width : this->width, height > 0 ? height : this->height);
    }

    /**
     * Activates a viewport such that it is positioned within a grid.
     *
     * @param 	x	  	The grid x coordinate (position of the requested viewport in the grid).
     * @param 	y	  	The grid y coordinate (position of the requested viewport in the grid).
     * @param 	grid_size_x 	The grid maximum grid size (the assumed number of viewports along x axis).
     * @param 	grid_size_y	The grid maximum grid size (the assumed  number of viewports along y axis).
     */
    void activate_viewport_grid(int x, int y, int grid_size_x, int grid_size_y) const {
        const int viewport_width = static_cast<int>(width) / grid_size_x;
        const int viewport_height = static_cast<int>(height) / grid_size_y;
        const int viewport_x = x * viewport_width;
        const int viewport_y = y * viewport_height;
        activate_viewport(viewport_x, viewport_y, viewport_width, viewport_height);
    }

    /**
     * Prepares the OpenGL context for rendering. A convenience method for calling @link activate_render_buffer and
     * @link activate_viewport.
     *
     * @param 	x		  	The X-coordination of the lower left corner of the viewport rectangle (0 by default).
     * @param 	y		  	The Y-coordination of the lower left corner of the viewport rectangle (0 by default).
     * @param 	width	  	The width of the viewport (value<=0 is used to indicate full screen).
     * @param 	height	  	The height of the viewport (value<=0 is used to indicate full screen).
     * @param 	buffer	  	The buffer to activate (0 is the main screen).
     * @param 	clear	  	If @p true the bounded frame buffer will be cleared.
     * @param 	depth_test	if @p true the depth test will be activated; if @p false the depth test will be deactivated.
     */
    void prepare_rendering(int x = 0, int y = 0, int width = 0, int height = 0, GLint buffer = 0, bool clear = true,
                           bool depth_test = true) const {
        activate_render_buffer(buffer, clear, depth_test);
        activate_viewport(x, y, width, height);
    }

    /**
     * Prepares the OpenGL context for rendering withing a grid of viewports. A convenience method for calling @link
     * activate_render_buffer and
     * @link activate_viewport.
     *
     * @param 	x		   	The grid x coordinate (position of the requested viewport in the grid).
     * @param 	y		   	The grid y coordinate (position of the requested viewport in the grid).
     * @param 	grid_size_x	The grid maximum grid size (the assumed number of viewports along x axis).
     * @param 	grid_size_y	The grid maximum grid size (the assumed  number of viewports along y axis).
     * @param 	buffer	   	The buffer to activate (0 is the main screen).
     * @param 	clear	   	If @p true the bounded frame buffer will be cleared.
     * @param 	depth_test 	if @p true the depth test will be activated; if @p false the depth test will be deactivated.
     */
    void prepare_rendering_grid(int x, int y, int grid_size_x, int grid_size_y, GLint buffer = 0, bool clear = false,
                                bool depth_test = true) const {
        activate_render_buffer(buffer, clear, depth_test);
        activate_viewport_grid(x, y, grid_size_x, grid_size_y);
    }

    /**
     * Uploads a projection and view matrices to a specified program. Note that the method also binds the program.
     *
     * @param 	program   	The program.
     * @param 	projection	The projection matrix.
     * @param 	view	  	The view matrix.
     */
    void upload_projection_view(const ShaderProgram& program, glm::mat4 projection, glm::mat4 view) const {
        if (program.is_valid()) {
            program.use();
            program.uniform_matrix("projection", {projection});
            program.uniform_matrix("view", {view});
        }
    }

    // ----------------------------------------------------------------------------
    // Getters & Setters
    // ----------------------------------------------------------------------------
    /**
     * Returns the current window width.
     *
     * @return	the window width.
     */
    size_t get_width() const { return this->width; }

    /**
     * Returns the current window height
     *
     * @return	the window height.
     */
    size_t get_height() const { return this->height; }

    std::filesystem::path get_framework_shaders_path() { return this->framework_shaders_path; };

    void set_window(GLFWwindow* window) {
        this->window = window;
    }
};
