#include "application.hpp"
#include "data.hpp"
#include <math.h>
#include <vector>
#include <iostream> 
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::make_shared;

float random() { return (float)rand() / ((float)RAND_MAX + 1.0f); }
float random_neg() { return (float)(rand() / ((float)RAND_MAX + 1.0f) * 2.0f) - 1.0f; }


GLuint load_texture_2d(const std::filesystem::path filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.generic_string().data(), &width, &height, &channels, 4);

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, std::log2(width), GL_RGBA8, width, height);

    glTextureSubImage2D(texture,
                        0,                         //
                        0, 0,                      //
                        width, height,             //
                        GL_RGBA, GL_UNSIGNED_BYTE, //
                        data);

    stbi_image_free(data);

    glGenerateTextureMipmap(texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

unsigned int loadCubemap(std::vector<std::filesystem::path> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].generic_string().data(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
    this->width = initial_width;
    this->height = initial_height;

    images_path = configuration.get_path("images", "/images");
    objects_path = configuration.get_path("objects", "/objects");

    // --------------------------------------------------------------------------
    //  Load/Create Objects
    // --------------------------------------------------------------------------
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "outside.obj")));
    // You can use from_file function to load a Geometry from .obj file
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "mirror.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "dresser.obj")));
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bedside_table.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "table_lamp.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "rug.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "chair.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "plant3/plant_base.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "plant3/plant_inside.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "plant3/plant_outside.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_frame.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_part1.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_part2.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_wrap.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_pillow1.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bed/bed_pillow2.obj")));

    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "globe/globe_stand.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "globe/globe.obj")));
    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "door/door_frame.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "door/door_base.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "door/door_handle.obj")));
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "plant_small/pot.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "plant_small/leaf.obj")));

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "lamp8.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "lamp8.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "lamp7.obj")));

    geometries.push_back(make_shared<Cube>());
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "UFO.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "cow.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "cone.obj")));
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "tree.obj")));

    outside = geometries[0];

    mirror = geometries[1];

    dresser = geometries[2];

    bedside_table = geometries[3];

    table_lamp = geometries[4];

    rug = geometries[5];
    chair = geometries[6];
    plant3 = geometries[7];
    plant_pot_inside = geometries[8];
    plant_pot_outside = geometries[9];

    bed_frame = geometries[10];
    bed_part1 = geometries[11];
    bed_part2 = geometries[12];
    bed_wrap = geometries[13];
    bed_pillow1 = geometries[14];
    bed_pillow2 = geometries[15];

    globe_stand = geometries[16];
    globe = geometries[17];

    door_frame = geometries[18];
    door_base = geometries[19];
    door_handle = geometries[20];
    
    plant_small_pot = geometries[21];
    plant_small_leaf = geometries[22];

    lamp1 = geometries[23];
    lamp2 = geometries[24];
    lamp3 = geometries[25];

    room = geometries[26];
    ufo = geometries[27];
    cow = geometries[28];
    cone = geometries[29];
    tree = geometries[30];
    


    wood = load_texture_2d(images_path / "light_wood.png");

    marble_texture = load_texture_2d(images_path / "bunny.jpg");
    
    rug_texture = load_texture_2d(images_path / "rug.jpg");

    chair_diffuse_texture= load_texture_2d(images_path / "chair/chair_diffuse.jpg");
    chair_ambient_texture= load_texture_2d(images_path / "chair/chair_ambient.jpg");
    chair_specular_texture= load_texture_2d(images_path / "chair/chair_specular.jpg");

    plant3_texture = load_texture_2d(images_path / "plant3/leaf.jpg");
    plant_pot_inside_texture = load_texture_2d(images_path / "plant3/stone.jpg");
    plant_pot_outside_texture = load_texture_2d(images_path / "plant3/vase.jpg");


    plush_body_ambient_texture = load_texture_2d(images_path / "plush/plush_body/BaseColor.png");
    plush_body_diffuse_texture = load_texture_2d(images_path / "plush/plush_body/Roughness.png");
    plush_body_specular_texture = load_texture_2d(images_path / "plush/plush_body/Metallic.png");
    plush_body_normal_texture = load_texture_2d(images_path / "plush/plush_body/Normal.png");

    white_bed_texture = load_texture_2d(images_path / "bed/white_bed.jpg");
    yellow_bed_texture = load_texture_2d(images_path / "bed/yellow_bed.jpg");
    blue_bed_texture = load_texture_2d(images_path / "bed/blue_bed.jpg");

    globe_stand_texture = load_texture_2d(images_path / "globe/globe_frame.png");
    globe_day_texture = load_texture_2d(images_path / "globe/globe_day.jpg");
    globe_night_texture = load_texture_2d(images_path / "globe/globe_night.jpg");

    door_frame_texture = load_texture_2d(images_path / "door/door_frame.jpg");
    door_base_texture = load_texture_2d(images_path / "door/door_base.jpg");

    small_plant_pot_normal_texture = load_texture_2d(images_path / "plant_small/POT_only_plant_Normal.png");
    small_plant_pot_diffuse_texture = load_texture_2d(images_path / "plant_small/POT_only_plant_BaseColor.png");
    small_plant_pot_ambient_texture = load_texture_2d(images_path / "plant_small/POT_only_plant_AO.png");
    small_plant_pot_specular_texture = load_texture_2d(images_path / "plant_small/POT_only_plant_Roughness.png");
    
    small_plant_leaf_normal_texture = load_texture_2d(images_path / "plant_small/normal_leaf_plant.png");
    small_plant_leaf_diffuse_texture = load_texture_2d(images_path / "plant_small/texture_of_leaf.png");
    small_plant_leaf_ambient_texture = load_texture_2d(images_path / "plant_small/opacity_of_leaf.png");
    small_plant_leaf_specular_texture = load_texture_2d(images_path / "plant_small/specular_of_leaf_copy.png");

    table_lamp_ambient_texture = load_texture_2d(images_path / "table_lamp/lamp_ambient.jpg");
    table_lamp_diffuse_texture = load_texture_2d(images_path / "table_lamp/lamp_base.jpg");
    table_lamp_specular_texture = load_texture_2d(images_path / "table_lamp/lamp_specular.jpg");
    table_lamp_normal_texture = load_texture_2d(images_path / "table_lamp/lamp_normal.jpg");

    dark_wood_texture = load_texture_2d(images_path / "dark_wood.jpg");

    lamp7_ambient_texture = load_texture_2d(images_path / "lamp7/lamp7_ao.jpg");
    lamp7_diffuse_texture= load_texture_2d(images_path / "lamp7/lamp7_diffuse.jpg");

    room_bot_texture = load_texture_2d(images_path / "ground.jpg");
    
    outside_texture = load_texture_2d(images_path / "mountains.png");

    room_texture = load_texture_2d(images_path / "room.jpg");
    room_texture_dark = load_texture_2d(images_path / "room_dark.jpg");

    ufo_normal_texture = load_texture_2d(images_path / "UFO/ufo_normal.png");
    ufo_ambient_texture = load_texture_2d(images_path / "UFO/ufo_ambient.png");
    ufo_diffuse_texture = load_texture_2d(images_path / "UFO/ufo_diffuse.png");
    ufo_specular_texture = load_texture_2d(images_path / "UFO/ufo_specular.png");

    cow_normal_texture = load_texture_2d(images_path / "cow/cow_normal.png");
    cow_ambient_texture = load_texture_2d(images_path / "cow/cow_ambient.jpeg");
    cow_diffuse_texture = load_texture_2d(images_path / "cow/cow_diffuse.jpg");
    cow_specular_texture = load_texture_2d(images_path / "cow/cow_specular.jpeg");
    
    tree_texture = load_texture_2d(images_path / "tree.jpeg");
   
    // --------------------------------------------------------------------------
    // Initialize UBO Data
    // --------------------------------------------------------------------------
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
    camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Stars lights
    for (size_t i = 0; i < 195; i++) {
        lights_night.push_back({
            glm::vec4(random_neg() * 300.0f, 10.0f + random() * 300.0f, random_neg() * 300.0f, 1.0f), // position
            glm::vec4(0.0f),                                                                      // ambient
            //glm::vec4(0.0f),   
            glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),                   // diffuse
            glm::vec4(0.0f)                                                                       // specular
        });
    }


    //light 1
    lights_night.push_back({.position = glm::vec4(-0.45f, 1.5f, 0.0f, 0.0f),
                        .ambient_color = glm::vec4(0.5f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                        .specular_color = glm::vec4(0.5f)});
    //light 2
    lights_night.push_back({.position = glm::vec4(-0.45f, 1.5f, 3.0f, 0.0f),
                        .ambient_color = glm::vec4(0.5f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                        .specular_color = glm::vec4(0.5f)});
    //light 1
    lights_day.push_back({.position = glm::vec4(-0.45f, 1.5f, 0.0f, 0.0f),
                        .ambient_color = glm::vec4(0.5f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                        .specular_color = glm::vec4(0.5f)});
    //light 2
    lights_day.push_back({.position = glm::vec4(-0.45f, 1.5f, 3.0f, 0.0f),
                        .ambient_color = glm::vec4(0.5f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                        .specular_color = glm::vec4(0.5f)});



    //table light
    lights_night.push_back({.position = glm::vec4(-1.76f, 0.7f, 0.5f, 0.0f),
                        .ambient_color = glm::vec4(0.1f),
                        .diffuse_color = glm::vec4(0.75f, 0.75f, 0.25f, 0.25f),
                        .specular_color = glm::vec4(0.1f)});

    
    //light next to chair
    lights_night.push_back({.position = glm::vec4(0.3f, 0.8f, 4.8f, 0.0f),
                        .ambient_color = glm::vec4(0.1f),
                        .diffuse_color = glm::vec4(0.75f, 0.75f, 0.25f, 0.25f),
                        .specular_color = glm::vec4(0.1f)});


                        
    //moon / sun
    lights_night.push_back({.position = glm::vec4(10.0f, 30.0f, 10.0f, 1.0f),
                        .ambient_color = glm::vec4(200.0f, 200.0f , 500.0f, 0.0f),
                        .diffuse_color = glm::vec4(500.0f, 500.0f, 2000.0f, 0.0f),
                        .specular_color = glm::vec4(10.0f, 10.0f, 50.0f, 0.0f)});

    lights_day.push_back({.position = glm::vec4(10.0f, 30.0f, 10.0f, 1.0f),
                        .ambient_color = glm::vec4(300.0f),
                        .diffuse_color = glm::vec4(1000.0f, 1000.0f, 500.0f, 1.0f),
                        .specular_color = glm::vec4(50.0f)});


    //cone light
    cone_light_ubo.position = glm::vec4(15.0f, 15.0f, 0.0f, 0.0f);
    cone_light_ubo.ambient = glm::vec4(5.0f);
    cone_light_ubo.diffuse = glm::vec4(8.0f);
    cone_light_ubo.specular = glm::vec4(3.0f);
    cone_light_ubo.direction = glm::vec3(0.0f, -1.0f, 0.0f);
    cone_light_ubo.cutoff = glm::cos(glm::radians(15.0f));

    glm::mat4 transform;

//objects
{
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(700.0f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.3f, 0.0554f, 0.32f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //mirror
   {
    transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    transform = glm::scale(transform, glm::vec3(2.3f));
    transform = glm::translate(transform, glm::vec3(0.2f, 0.33f, -2.2f));
    transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
   }

   //dresser
   {
    transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    transform = glm::scale(transform, glm::vec3(2.0f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-1.3f, 0.0f, -0.9f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
   }

    
    //bedside table
    {
    transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    transform = glm::scale(transform, glm::vec3(0.8f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-0.8f, -0.22f, -2.48f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
    }


    //table lamp
    {
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.7));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-2.8f, 0.8f, 0.9f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
    }
    //rug
    {
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(2.5));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.0f, -0.2f, 0.7f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});
    }
    
    
    //chair
    {
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(1.0f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(2.2f, -0.1f, -3.7f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});
    }

    //plant3
    {
        //plant
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(2.5f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.38f, 0.3f, -0.1f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //plant pot inside
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(0.25));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(3.8f, -1.4f, -0.89f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //plant pot outside
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(0.36));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(2.6f, -1.1f, -0.6f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});
    }

    //bed
    {
        //bed frame
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(2.5f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.2f, 0.0f, -0.4f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //bed part 1 
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(2.5f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.2f, 0.0f, -0.4f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //bed part 2
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(2.2f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.23f, 0.01f, -0.4f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //bed wrap
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(1.0f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.2f, 0.075f, -0.5f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //bed pillow 1
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(0.9f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.5f, 0.3f, -2.1f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

        //bed pillow 2
        transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
        transform = glm::scale(transform, glm::vec3(0.6f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(1.1f, 0.5f, -2.9f)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f),
                                .specular_color = glm::vec4(0.0f)});

    }

    //globe
{
    //double hours = time/3600;
    //stand
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.7f));
    transform = glm::translate(transform, glm::vec3(-2.6f, 1.3f, 2.9f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});


    //sphere
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.4f));
    transform = glm::translate(transform, glm::vec3(-4.55f, 2.5f, 5.05f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
}

    //door
{
    //frame
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(3.0f, 2.5f, 2.5f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-0.767f, 0.26f, 1.8f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //base
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(30.0f, 2.5f, 2.5f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-0.077f, 0.26f, 1.8f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //door handle, chrome material
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.5f, 0.4f, 0.4f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-4.6f, 1.25f, 11.6f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
}

    //plant 2
{
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(0.3f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-5.8f, 2.4f, 11.0f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
    
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(1.0f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-1.73f, 1.0f, 3.35f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
}


    //lamp1
    //chrome material
    transform = glm::mat4(1.0f);
    //transform = glm::rotate(transform, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    transform = glm::scale(transform, glm::vec3(1.2f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-0.3f, 1.7f, 2.3f)),
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(0.4f),
                            .specular_color = glm::vec4(0.774597f)});

    //lamp2
    //chrome material
    transform = glm::mat4(1.0f);
    //transform = glm::rotate(transform, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    transform = glm::scale(transform, glm::vec3(1.2f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(-0.3f, 1.7f, 0.0f)),
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(0.4f),
                            .specular_color = glm::vec4(0.774597f)});
                            
    //lamp3
    transform = glm::mat4(1.0f);
    //transform = glm::rotate(transform, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f)); 
    transform = glm::scale(transform, glm::vec3(1.5f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.2f, 0.1f, 3.2f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //room
    //bot
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.3f, -0.9f, 1.9f));
    transform = glm::scale(transform, glm::vec3(2.05, 0.3, 3.25));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

               
    //top
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.25f, 2.7f, 2.1f));
    transform = glm::scale(transform, glm::vec3(2.3, 0.05, 3.50));
    //transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

                            
    //back_side
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-2.3f, 1.05f, 1.9f));
    transform = glm::scale(transform, glm::vec3(0.05, 1.7, 3.25));
    transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
                            
    //mirror side
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.3f, 1.05f, 5.1f));
    transform = glm::scale(transform, glm::vec3(2.0, 1.7, 0.05));
    transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
                            
    //bed side
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(-0.3f, 1.05f, -1.3f));
    transform = glm::scale(transform, glm::vec3(2.0, 1.7, 0.05));
    transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
                            
    //window side1
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(1.7f, 1.05f, -0.2f));
    transform = glm::scale(transform, glm::vec3(0.05, 1.7, 1.15));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //window side2
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(1.7f, 1.05f, 4.15f));
    transform = glm::scale(transform, glm::vec3(0.05, 1.7, 1.0));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

                            
    //glass
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(1.7f, 1.05f, 2.15f));
    transform = glm::scale(transform, glm::vec3(0.04, 1.7, 2.0));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(0.5f, 0.5f, 1.0f, 0.5f),
                            .specular_color = glm::vec4(0.0f)});
    
    
    //UFO
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(15.0f, 15.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(5.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //cow
    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(15.0f, 1.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(3.0f));
    objects_ubos.push_back({.model_matrix = transform,
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //frame window
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(1.7f, 3.3f, 6.4f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(0.99f, 0.3f, 0.32f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    //cone
    transform = glm::mat4(1.0f);
    transform = glm::scale(transform, glm::vec3(15.0f, 30.0f, 15.0f));
    objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f)),
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.3f),
                            .specular_color = glm::vec4(0.0f)});

    //tree
    for (int i = 0; i <= 30; i++)
    {
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(8.0f + random_neg()*2.0f));
        objects_ubos.push_back({.model_matrix = glm::translate(transform, glm::vec3(4.0f + random_neg()*2, 0.0f, 1.0f * random_neg()*3)),
                                .ambient_color = glm::vec4(0.0f),
                                .diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                .specular_color = glm::vec4(0.0f)});
    }
    

    

}   

   

    glCreateVertexArrays(1, &skyboxVAO);
    glCreateBuffers(1, &skyboxVBO);
    glNamedBufferStorage(skyboxVBO, sizeof(skyboxVertices), &skyboxVertices, NULL);
    glVertexArrayVertexBuffer(skyboxVAO, 0, skyboxVBO, 0, 3*sizeof(float));
    glEnableVertexArrayAttrib(skyboxVAO, 0);
    glVertexArrayAttribFormat(skyboxVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(skyboxVAO, 0, 0);
    


    // --------------------------------------------------------------------------
    // Create Buffers
    // --------------------------------------------------------------------------
    glCreateBuffers(1, &camera_buffer);
    glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &light_buffer);
    glNamedBufferStorage(light_buffer, sizeof(LightUBO), &light_ubo, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &objects_buffer);
    glNamedBufferStorage(objects_buffer, sizeof(ObjectUBO) * objects_ubos.size(), objects_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &lights_night_buffer);
    glCreateBuffers(1, &lights_day_buffer);
    glNamedBufferStorage(lights_night_buffer, 200*sizeof(LightUBO), lights_night.data(), GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(lights_day_buffer, 3*sizeof(LightUBO), lights_day.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &cone_light_buffer);
    glNamedBufferStorage(cone_light_buffer, sizeof(ConeLightUBO), &cone_light_ubo, GL_DYNAMIC_STORAGE_BIT);
    
    glCreateFramebuffers(1,&framebuffer);
    glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_color);
    glTextureStorage2D(framebuffer_color, 1, GL_RGBA32F, width, height);
    glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_depth);
    glTextureStorage2D(framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);
    const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(framebuffer, 1, draw_buffers);
    glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, framebuffer_color, 0);
    glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, framebuffer_depth, 0);

    compile_shaders();
}

Application::~Application() {
    delete_shaders();
    glDeleteVertexArrays (1, &skyboxVAO);
    glDeleteBuffers(1, &camera_buffer);
    glDeleteBuffers(1, &light_buffer);
    glDeleteBuffers(1, &objects_buffer);
    glDeleteBuffers(1, &lights_night_buffer);
    glDeleteBuffers(1, &lights_day_buffer);
    glDeleteBuffers(1, &cone_light_buffer);
    
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &framebuffer_color);
    glDeleteTextures(1, &framebuffer_depth);
}

void Application::delete_shaders() {
    glDeleteProgram(skybox_program);
  }

void Application::compile_shaders() {
    delete_shaders();
    main_program = ShaderProgram{shaders_path / "main.vert", shaders_path / "main.frag"};
    fog_program = ShaderProgram{shaders_path / "fog.vert", shaders_path / "fog.frag"};
    textured_program = ShaderProgram{shaders_path / "textured.vert", shaders_path / "textured.frag"};
    mirror_program = ShaderProgram{shaders_path / "mirror.vert", shaders_path / "mirror.frag"};
    draw_light_program = ShaderProgram{shaders_path / "draw_light.vert", shaders_path / "draw_light.frag"};
    reflect_program = ShaderProgram{shaders_path / "reflect.vert", shaders_path / "reflect.frag"};
    skybox_program = create_program(shaders_path / "skybox.vert", shaders_path / "skybox.frag");
    postprocess_program = create_program(shaders_path / "postprocess.vert", shaders_path / "postprocess.frag");

}

void Application::render() {
    // --------------------------------------------------------------------------
    // Update UBOs
    // --------------------------------------------------------------------------
    // Camera
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

    // --------------------------------------------------------------------------
    // Draw scene
    // --------------------------------------------------------------------------
    glViewport(0, 0, (GLsizei)this->width, (GLsizei)this->height);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Configure fixed function pipeline
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    //skybox
        
    if (night)
    {
        faces =
        {
            images_path / "skybox/right_night.jpg",
            images_path / "skybox/left_night.jpg",
            images_path / "skybox/top_night.jpg",
            images_path / "skybox/bottom_night.jpg",
            images_path / "skybox/front_night.jpg",
            images_path / "skybox/back_night.jpg",
        };
    }
    else 
    {
        faces=
        {
            images_path / "skybox/right_day.jpg",
            images_path / "skybox/left_day.jpg",
            images_path / "skybox/top_day.jpg",
            images_path / "skybox/bottom_day.jpg",
            images_path / "skybox/front_day.jpg",
            images_path / "skybox/back_day.jpg",
        };
    }

    unsigned int cubemapTexture = loadCubemap(faces);
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skybox_program);
    fog_program.uniform("toon_shading", toon_shading);
    
    int model_loc = glGetUniformLocation(skybox_program, "projection_matrix");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f)));
    int view_loc = glGetUniformLocation(skybox_program, "view_matrix");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(camera_ubo.view))));
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

    // Draw objects


    //stars
    if (night) {
        lights_buffer = &lights_night_buffer;
        Sphere sphere;
        draw_light_program.use();
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_night_buffer);
        sphere.bind_vao();
        glDrawElementsInstanced(sphere.mode, sphere.draw_elements_count, GL_UNSIGNED_INT , NULL , 195);
    } else {
        lights_buffer = &lights_day_buffer;
    }
    

   //with toon shading on we also add outlines to our objects - rendering to a custom framebuffer and postprocessing
    if (toon_shading && edge_detection) 
    {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearNamedFramebufferfv(framebuffer, GL_COLOR, 0, clear_color);
        glClearNamedFramebufferfv(framebuffer, GL_DEPTH, 0, clear_depth);
        glEnable(GL_DEPTH_TEST);
    }

//objects
    
    //outside
    {
    fog_program.use();
    //main_program.use();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
    fog_program.uniform("toon_shading", toon_shading);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer,  0 * 256, sizeof(ObjectUBO));
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, cone_light_buffer);

    fog_program.uniform("night", night);
    glBindTextureUnit(3, outside_texture);
    outside->draw();
    }


    main_program.use();

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, cone_light_buffer);
    main_program.uniform("blend", false);
    main_program.uniform("toon_shading", toon_shading);

    //dresser
    {
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 2 * 256, sizeof(ObjectUBO));

    main_program.uniform("has_texture", true);
    glBindTextureUnit(3, wood);
    dresser->draw();
    }

    //bedside table
    {
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 3 * 256, sizeof(ObjectUBO));

    main_program.uniform("has_texture", true);
    glBindTextureUnit(3, wood);
    bedside_table->draw();
    }
    
    
    //rug1
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 5 * 256, sizeof(ObjectUBO));

        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, rug_texture);
        rug->draw();
    }

    //plant3
    {
        //plant3
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 7 * 256, sizeof(ObjectUBO));

        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, plant3_texture);
        plant3->draw();

        //plant pot inside
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 8 * 256, sizeof(ObjectUBO));

        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, plant_pot_inside_texture);
        plant_pot_inside->draw();

        //plant pot outside
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 9 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, plant_pot_outside_texture);
        plant_pot_outside->draw();
    }

    //bed
    {
        //bed frame
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 10 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, wood);
        bed_frame->draw();

        //bed part 1
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 11 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, white_bed_texture);
        bed_part1->draw();

        //bed part 2
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 12 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, blue_bed_texture);
        bed_part2->draw();

        //bed wrap
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 13 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, yellow_bed_texture);
        bed_wrap->draw();


        //bed pillow1
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 14 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, white_bed_texture);
        bed_pillow1->draw();

        //bed pillow2
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 15 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, yellow_bed_texture);
        bed_pillow2->draw();
    }

    //globe
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 16 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, dark_wood_texture);
        globe_stand->draw();


        glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
        textured_program.uniform("toon_shading", toon_shading);


        time = glfwGetTime();
        glm::mat4 transform = glm::mat4(1.0f);
        angle = int(time) % 360 * 2;

        ObjectUBO globe_obj;
        GLuint globe_buffer = 0;
        transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(0.4f));
        transform = glm::translate(transform, glm::vec3(-4.55f, 2.5f, 5.05f));
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        globe_obj.model_matrix = transform;
        globe_obj.ambient_color = glm::vec4(0.0f);
        globe_obj.diffuse_color = glm::vec4(1.0f);
        globe_obj.specular_color = glm::vec4(0.0f);
        
        glCreateBuffers(1, &globe_buffer);
        glNamedBufferStorage(globe_buffer, sizeof(ObjectUBO), &globe_obj, GL_DYNAMIC_STORAGE_BIT);

        glBindBufferBase(GL_UNIFORM_BUFFER, 2, globe_buffer);
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, globe_day_texture);
        if (night) {
            glBindTextureUnit(3, globe_night_texture);
        }
        globe->draw();

    }

    //door
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 18 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, door_frame_texture);
        door_frame->draw();
        
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 19 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, door_base_texture);
        door_base->draw();
        
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 20 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", false);
        door_handle->draw();
    }


    //lamp1
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 23 * 256, sizeof(ObjectUBO));
    main_program.uniform("has_texture", false);
    lamp1->draw();

    //lamp2
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 24 * 256, sizeof(ObjectUBO));
    main_program.uniform("has_texture", false);
    lamp2->draw();

    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //mirror frame
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 1 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, dark_wood_texture);
        mirror->draw();
    }

    //room
    {

        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 26 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, room_bot_texture);
        room->draw();

        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 28 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, room_texture);
        room->draw();

    
        if (!walls_off) {
            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 27 * 256, sizeof(ObjectUBO));
            main_program.uniform("has_texture", true);
            glBindTextureUnit(3, room_texture_dark);
            room->draw();

            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 29 * 256, sizeof(ObjectUBO));
            main_program.uniform("has_texture", true);
            glBindTextureUnit(3, room_texture);
            room->draw();
            
            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 31 * 256, sizeof(ObjectUBO));
            main_program.uniform("has_texture", true);
            glBindTextureUnit(3, room_texture);
            room->draw();

            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 32 * 256, sizeof(ObjectUBO));
            main_program.uniform("has_texture", true);
            glBindTextureUnit(3, room_texture);
            room->draw();

            //window frame
            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 36 * 256, sizeof(ObjectUBO));
            main_program.uniform("has_texture", true);
            glBindTextureUnit(3, door_frame_texture);
            door_frame->draw();
        }

        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 30 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, room_texture);
        room->draw();
        
    }

     
    for (int i = 0; i <= 29; i++)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, (38+i) * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", true);
        glBindTextureUnit(3, tree_texture);
        tree->draw();
    }
    
    //textured program
    textured_program.use();
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, cone_light_buffer);
    textured_program.uniform("toon_shading", toon_shading);

    //lamp
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 4 * 256, sizeof(ObjectUBO));
    textured_program.uniform("has_3texture", true);
    textured_program.uniform("has_4texture", true);
    textured_program.uniform("has_5texture", true);
    textured_program.uniform("has_6texture", true);
    glBindTextureUnit(4, table_lamp_ambient_texture);
    glBindTextureUnit(3, table_lamp_diffuse_texture);
    glBindTextureUnit(5, table_lamp_specular_texture);
    glBindTextureUnit(6, table_lamp_normal_texture);
    
    table_lamp->draw();

    //lamp3
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 25 * 256, sizeof(ObjectUBO));
    textured_program.uniform("has_3texture", true);
    textured_program.uniform("has_4texture", true);
    textured_program.uniform("has_5texture", false);
    textured_program.uniform("has_6texture", false);
    glBindTextureUnit(3, lamp7_ambient_texture);
    glBindTextureUnit(4, lamp7_diffuse_texture);
    lamp3->draw();

    //plant small
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 21 * 256, sizeof(ObjectUBO));
        textured_program.uniform("has_3texture", true);
        textured_program.uniform("has_4texture", true);
        textured_program.uniform("has_5texture", true);
        textured_program.uniform("has_6texture", true);
        glBindTextureUnit(3, small_plant_pot_ambient_texture);
        glBindTextureUnit(4, small_plant_pot_diffuse_texture);
        glBindTextureUnit(5, small_plant_pot_specular_texture);
        glBindTextureUnit(6, small_plant_pot_normal_texture);
        plant_small_pot->draw();

        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 22 * 256, sizeof(ObjectUBO));
        textured_program.uniform("has_3texture", true);
        textured_program.uniform("has_4texture", true);
        textured_program.uniform("has_5texture", true);
        textured_program.uniform("has_6texture", true);
        glBindTextureUnit(3, small_plant_leaf_ambient_texture);
        glBindTextureUnit(4, small_plant_leaf_diffuse_texture);
        glBindTextureUnit(5, small_plant_leaf_specular_texture);
        glBindTextureUnit(6, small_plant_leaf_normal_texture);
        plant_small_leaf->draw();  
    }
    //chair
    
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 6 * 256, sizeof(ObjectUBO));

    textured_program.uniform("has_3texture", true);
    textured_program.uniform("has_4texture", true);
    textured_program.uniform("has_5texture", true);
    textured_program.uniform("has_6texture", false);
    glBindTextureUnit(3, chair_ambient_texture);
    glBindTextureUnit(4, yellow_bed_texture);
    glBindTextureUnit(5, chair_specular_texture);
    chair->draw();

    if (night || !night)
    {
        textured_program.use();
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
        textured_program.uniform("toon_shading", toon_shading);

        //UFO
        if (camouflage)
        {   
            reflect_program.use();
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 34 * 256, sizeof(ObjectUBO));
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);  	
            ufo->draw();
            textured_program.use();
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
            textured_program.uniform("toon_shading", toon_shading);
        }
        else
        {
            glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 34 * 256, sizeof(ObjectUBO));
            textured_program.uniform("has_3texture", true);
            textured_program.uniform("has_4texture", true);
            textured_program.uniform("has_5texture", true);
            textured_program.uniform("has_6texture", true);
            glBindTextureUnit(3, ufo_ambient_texture);
            glBindTextureUnit(4, ufo_diffuse_texture);
            glBindTextureUnit(5, ufo_specular_texture);
            glBindTextureUnit(6, ufo_normal_texture);
            ufo->draw();
        }

        //cow
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
        textured_program.uniform("toon_shading", toon_shading);

        time = glfwGetTime();
        glm::mat4 transform = glm::mat4(1.0f);
        float move = int(time) % 100;
        angle = int(time) % 360 * 4;

        ObjectUBO cow_obj;
        GLuint cow_buffer = 0;
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(15.0f, 1.0+move/10, 0.0f));
        transform = glm::scale(transform, glm::vec3(3.0f));
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(angle*0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        cow_obj.model_matrix = transform;
        cow_obj.ambient_color = glm::vec4(0.0f);
        cow_obj.diffuse_color = glm::vec4(1.0f);
        cow_obj.specular_color = glm::vec4(0.0f);
        
        glCreateBuffers(1, &cow_buffer);
        glNamedBufferStorage(cow_buffer, sizeof(ObjectUBO), &cow_obj, GL_DYNAMIC_STORAGE_BIT);

        glBindBufferBase(GL_UNIFORM_BUFFER, 2, cow_buffer);
        textured_program.uniform("has_3texture", true);
        textured_program.uniform("has_4texture", true);
        textured_program.uniform("has_5texture", true);
        textured_program.uniform("has_6texture", true);
        glBindTextureUnit(3, cow_ambient_texture);
        glBindTextureUnit(4, cow_diffuse_texture);
        glBindTextureUnit(5, cow_specular_texture);
        glBindTextureUnit(6, cow_normal_texture);
        
        cow->draw();
    }


    main_program.use();
    
    //glass window
    if (!walls_off)
    {
        main_program.uniform("toon_shading", toon_shading);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, cone_light_buffer);
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 33 * 256, sizeof(ObjectUBO));
        main_program.uniform("has_texture", false);
        main_program.uniform("blend", true);
        room->draw();
    }

    //cone
    main_program.uniform("toon_shading", toon_shading);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, *lights_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, cone_light_buffer);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 37 * 256, sizeof(ObjectUBO));
    main_program.uniform("has_texture", false);
    main_program.uniform("blend", true);
    cone->draw();

    if (toon_shading && edge_detection)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(postprocess_program);
        glBindTextureUnit(0,framebuffer_color);
        glDrawArrays( GL_TRIANGLES, 0, 3);
    }


/*
    glDisable(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    mirror_program.use();
    //glDrawArrays(GL_TRIANGLE_STRIP_ADJACENCY, 0, 4);
    main_program.use();
    glEnable(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_BUFFER_BIT);
 */   

}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

void Application::on_resize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Application::on_mouse_move(double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_button(int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);

    if (key == GLFW_KEY_N && action == GLFW_PRESS)  {
        night = !night;
    }
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS)  {
        walls_off = !walls_off;
    }
    
    if (key == GLFW_KEY_T && action == GLFW_PRESS)  {
        toon_shading = !toon_shading;
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS)  {
        edge_detection = !edge_detection;
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)  {
        camouflage = !camouflage;
    }
    
}
