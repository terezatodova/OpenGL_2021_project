#version 450

in vec3 texture_coordinates;

layout(location = 4) uniform bool toon_shading = false;

uniform samplerCube skybox;

out vec4 final_color;

void main() {
    final_color = texture(skybox,texture_coordinates);
    if (toon_shading)
    {    
        if(final_color.r > 0.9)
            final_color.r = 1.0;
        else if(final_color.r > 0.85)
            final_color.r = 0.75;
        else if(final_color.r > 0.65)
            final_color.r = 0.5;
        else if(final_color.r > 0.35)
            final_color.r = 0.25;
        else
            final_color.r = 0.0;
            

        if(final_color.g > 0.9)
            final_color.g = 1.0;
        else if(final_color.g > 0.85)
            final_color.g = 0.75;
        else if(final_color.g > 0.65)
            final_color.g = 0.5;
        else if(final_color.g > 0.35)
            final_color.g = 0.25;
        else
            final_color.g = 0.0;

        if(final_color.b > 0.9)
            final_color.b = 1.0;
        else if(final_color.b > 0.85)
            final_color.b = 0.75;
        else if(final_color.b > 0.65)
            final_color.b = 0.5;
        else if(final_color.b > 0.35)
            final_color.b = 0.25;
        else
            final_color.b = 0.0;
    }
}