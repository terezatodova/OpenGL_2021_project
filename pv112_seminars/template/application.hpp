#pragma once

#include "camera.hpp"
#include "cube.hpp"
#include "pv112_application.hpp"
#include "sphere.hpp"
#include "teapot.hpp"


// ----------------------------------------------------------------------------
// UNIFORM STRUCTS
// ----------------------------------------------------------------------------
struct CameraUBO {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};

struct LightUBO {
    glm::vec4 position;
    glm::vec4 ambient_color;
    glm::vec4 diffuse_color;
    glm::vec4 specular_color;
};

struct ConeLightUBO {
    glm::vec4 position;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec3 direction;
    float cutoff;
};

struct alignas(256) ObjectUBO {
    glm::mat4 model_matrix;  // [  0 -  64) bytes
    glm::vec4 ambient_color; // [ 64 -  80) bytes
    glm::vec4 diffuse_color; // [ 80 -  96) bytes

    // Contains shininess in .w element
    glm::vec4 specular_color; // [ 96 - 112) bytes
};

// Constants
const float clear_color[4] = {0.0, 0.0, 0.0, 1.0};
const float clear_depth[1] = {1.0};

class Application : public PV112Application {
  public:
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    ~Application();

    /** @copydoc PV112Application::render */
    void render() override;

    /** @copydoc PV112Application::render_ui */
    void render_ui() override;

    /** @copydoc PV112Application::compile_shaders */
    void compile_shaders() override;

    /** @copydoc PV112Application::delete_shaders */
    void delete_shaders() override;

    /** @copydoc PV112Application::on_resize */
    void on_resize(int width, int height) override;

    /** @copydoc PV112Application::on_mouse_move */
    void on_mouse_move(double x, double y) override;

    /** @copydoc PV112Application::on_mouse_button */
    void on_mouse_button(int button, int action, int mods) override;

    /** @copydoc PV112Application::on_key_pressed */
    void on_key_pressed(int key, int scancode, int action, int mods) override;

  private:
    size_t width;
    size_t height;

    std::filesystem::path images_path;
    std::filesystem::path objects_path;

    // Main program
    ShaderProgram main_program;
    ShaderProgram fog_program;
    ShaderProgram textured_program;
    ShaderProgram mirror_program;
    ShaderProgram draw_light_program;
    ShaderProgram reflect_program;
    GLuint skybox_program = 0;
    GLuint postprocess_program = 0;

    // List of geometries used in the project
    std::vector<std::shared_ptr<Geometry>> geometries;
    // Shared pointers are pointers that automatically count how many times they are used. When there are 0 pointers to the object pointed by shared_ptrs, the object is automatically deallocated.
    // Consequently, we gain 3 main properties:
    // 1. Objects are not unnecessarily copied
    // 2. We don't have to track pointers
    // 3. We don't have to deallocate these geometries
    
    //Sphere sphere{};
    std::shared_ptr<Geometry> outside;
    std::shared_ptr<Geometry> bunny;

    std::shared_ptr<Geometry> dresser;

    std::shared_ptr<Geometry> bedside_table;
    
    std::shared_ptr<Geometry> plant3;
    std::shared_ptr<Geometry> plant_pot_inside;
    std::shared_ptr<Geometry> plant_pot_outside;

    std::shared_ptr<Geometry> plush;

    std::shared_ptr<Geometry> rug;

    std::shared_ptr<Geometry> chair;

    std::shared_ptr<Geometry> bed_frame;
    std::shared_ptr<Geometry> bed_part1;
    std::shared_ptr<Geometry> bed_part2;
    std::shared_ptr<Geometry> bed_wrap;
    std::shared_ptr<Geometry> bed_pillow1;
    std::shared_ptr<Geometry> bed_pillow2;
    
    std::shared_ptr<Geometry> globe_stand;
    std::shared_ptr<Geometry> globe;

    std::shared_ptr<Geometry> door_frame;
    std::shared_ptr<Geometry> door_base;
    std::shared_ptr<Geometry> door_handle;

    std::shared_ptr<Geometry> plant_small_leaf;
    std::shared_ptr<Geometry> plant_small_pot;

    std::shared_ptr<Geometry> table_lamp;

    std::shared_ptr<Geometry> mirror;

    std::shared_ptr<Geometry> lamp1;
    std::shared_ptr<Geometry> lamp2;
    std::shared_ptr<Geometry> lamp3;
    
    std::shared_ptr<Geometry> room;
    std::shared_ptr<Geometry> window;

    std::shared_ptr<Geometry> ufo;
    std::shared_ptr<Geometry> cow;
    std::shared_ptr<Geometry> cone;
    std::shared_ptr<Geometry> tree;


    // Default camera that rotates around center. Feel free to take inspiration and code your own.
    Camera camera;

    // UBOs
    GLuint camera_buffer = 0;
    CameraUBO camera_ubo;

    GLuint light_buffer = 0;
    LightUBO light_ubo;

    GLuint objects_buffer = 0;
    std::vector<ObjectUBO> objects_ubos;

    // Lights
    GLuint *lights_buffer;

    std::vector<LightUBO> lights_day;
    GLuint lights_day_buffer = 0;
    
    std::vector<LightUBO> lights_night;
    GLuint lights_night_buffer = 0;

    //1 cone light - ufo
    GLuint cone_light_buffer = 0;
    ConeLightUBO cone_light_ubo;

    //framebuffers
    GLuint framebuffer_mirror;
    GLuint framebuffer_color_mirror;
    GLuint framebuffer_depth_mirror;
    //outline
    GLuint framebuffer;
    GLuint framebuffer_color;
    GLuint framebuffer_depth;

    // Textures
    GLuint marble_texture = 0;
    GLuint plant3_texture = 0;
    GLuint plant_pot_inside_texture = 0;
    GLuint plant_pot_outside_texture = 0;
    GLuint wood = 0;
    GLuint rug_texture = 0;
    GLuint plush_body_ambient_texture = 0;
    GLuint plush_body_diffuse_texture = 0;
    GLuint plush_body_specular_texture = 0;
    GLuint plush_body_normal_texture = 0;

    //GLuint grey_wood_texture = 0;
    GLuint yellow_bed_texture = 0;
    GLuint blue_bed_texture = 0;
    GLuint white_bed_texture = 0;

    GLuint chair_diffuse_texture = 0;
    GLuint chair_ambient_texture = 0;
    GLuint chair_specular_texture = 0;


    GLuint globe_stand_texture = 0;
    GLuint globe_day_texture = 0;
    GLuint globe_night_texture = 0;
    GLuint globe_stand_normal_texture = 0;
    GLuint globe_normal_texture = 0;

    GLuint door_frame_texture = 0;
    GLuint door_base_texture = 0;

    GLuint small_plant_pot_normal_texture = 0;
    GLuint small_plant_pot_diffuse_texture = 0;
    GLuint small_plant_pot_ambient_texture = 0;
    GLuint small_plant_pot_specular_texture = 0;
    
    GLuint small_plant_leaf_normal_texture = 0;
    GLuint small_plant_leaf_diffuse_texture = 0;
    GLuint small_plant_leaf_ambient_texture = 0;
    GLuint small_plant_leaf_specular_texture = 0;

    GLuint table_lamp_ambient_texture = 0;
    GLuint table_lamp_diffuse_texture = 0;
    GLuint table_lamp_specular_texture = 0;
    GLuint table_lamp_normal_texture = 0;

    GLuint dark_wood_texture = 0;
    
    GLuint lamp7_ambient_texture = 0;
    GLuint lamp7_diffuse_texture = 0;

    GLuint room_bot_texture = 0;

    GLuint outside_texture = 0;
    GLuint room_texture = 0;
    GLuint room_texture_dark = 0;

    GLuint ufo_normal_texture = 0;
    GLuint ufo_ambient_texture = 0;
    GLuint ufo_diffuse_texture = 0;
    GLuint ufo_specular_texture = 0;

    GLuint cow_normal_texture = 0;
    GLuint cow_ambient_texture = 0;
    GLuint cow_diffuse_texture = 0;
    GLuint cow_specular_texture = 0;

    GLuint tree_texture = 0;

    bool night = false;
    bool walls_off = false;
    bool toon_shading = false;
    bool edge_detection = false;
    bool camouflage = false;

  	GLuint skyboxVAO;
    GLuint skyboxVBO;
    unsigned int cubemapTexture;
    std::vector<std::filesystem::path> faces;
    float prev_angle = 0;

    Cube cube;

    double time;
    float angle;
};
