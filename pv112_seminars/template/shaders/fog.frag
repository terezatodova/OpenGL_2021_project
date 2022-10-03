#version 450

layout(binding = 0, std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
}
camera;

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 1, std430) buffer Lights {
	Light lights[];
};


layout(binding = 2, std140) uniform Object {
    mat4 model_matrix;

    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
}
object;

struct FogParameters
{
	vec3 color;
	float linearStart;
	float linearEnd;
	float density;
	
	int equation;
	bool isEnabled;
};

layout(binding = 3, std140) uniform Cone_light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
    vec3 direction;
    float cutoff;
}
cone_light;

layout(location = 3) uniform bool night = false;
layout(location = 4) uniform bool toon_shading = false;

layout(binding = 3) uniform sampler2D albedo_texture;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;
layout(location = 3) in float visibility;

layout(location = 0) out vec4 final_color;

void main() {
    vec3 color_sum = vec3(0.0);
    for (int i = 0; i < lights.length(); i++)
    {
        Light light = lights[i];
        vec3 light_vector = light.position.xyz - fs_position * light.position.w;
        vec3 L = normalize(light_vector);
        vec3 N = normalize(fs_normal);
        vec3 E = normalize(camera.position - fs_position);
        vec3 H = normalize(L + E);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0001);

        vec3 ambient = object.ambient_color.rgb * light.ambient_color.rgb;
        vec3 diffuse = object.diffuse_color.rgb * (texture(albedo_texture, fs_texture_coordinate).rgb) *
                    light.diffuse_color.rgb;
        
        if (night) {
            diffuse.r = diffuse.r*0.5;
            diffuse.g = diffuse.g*0.5;
            diffuse.b = diffuse.b;
        }

        vec3 specular = object.specular_color.rgb * light.specular_color.rgb;

        vec3 color = ambient.rgb + NdotL * diffuse.rgb + pow(NdotH, object.specular_color.w) * specular;

        if (light.position.w == 1.0) {
            color /= (dot(light_vector, light_vector));
        }

        color_sum += color;
    }

    //cone_light
    {
        vec3 cone_light_vector = cone_light.position.xyz - fs_position * cone_light.position.w;
        vec3 L = normalize(cone_light_vector);
        vec3 N = normalize(fs_normal);
        vec3 E = normalize(camera.position - fs_position);
        vec3 H = normalize(L + E);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0001);
        float theta = dot(normalize(cone_light.position.xyz - fs_position), normalize(-cone_light.direction));

        vec3 specular;
        vec3 ambient;
        vec3 diffuse;
        vec3 color;

        if(theta > cone_light.cutoff) {       
            ambient = object.ambient_color.rgb * cone_light.ambient_color.rgb;
            diffuse = object.diffuse_color.rgb * (texture(albedo_texture, fs_texture_coordinate).rgb) *
                        cone_light.diffuse_color.rgb;
            

            if (night) {
                diffuse.r = diffuse.r*0.5;
                diffuse.g = diffuse.g*0.5;
                diffuse.b = diffuse.b;
            }

            specular = object.specular_color.rgb * cone_light.specular_color.rgb;
            color = ambient.rgb + NdotL * diffuse.rgb + pow(NdotH, object.specular_color.w) * specular;
        } 
        
        else 
        {
            color = vec3(0.05) *  object.ambient_color.rgb * cone_light.ambient_color.rgb;
        }

        if (cone_light.position.w == 1.0) {
            color /= (dot(cone_light_vector, cone_light_vector));
        }

        color_sum += color;
    }

    color_sum = color_sum / (color_sum + 1.0);   // tone mapping
    color_sum = pow(color_sum, vec3(1.0 / 2.2)); // gamma correction
    final_color = vec4(color_sum, 1.0);

    //add fog
    if (night) {
        vec3 skyColour = vec3(0.5, 0.5, 0.5);
        final_color = mix(vec4(skyColour, 1.0), final_color, visibility);
    }

    if (toon_shading)
    {
        
        if(final_color.r > 0.9)
            final_color.r = 1.0;
        else if(final_color.r > 0.75)
            final_color.r = 0.75;
        else if(final_color.r > 0.5)
            final_color.r = 0.5;
        else if(final_color.r > 0.35)
            final_color.r = 0.25;
        else
            final_color.r = 0.0;
            

        if(final_color.g > 0.9)
            final_color.g = 1.0;
        else if(final_color.g > 0.75)
            final_color.g = 0.75;
        else if(final_color.g > 0.5)
            final_color.g = 0.5;
        else if(final_color.g > 0.35)
            final_color.g = 0.25;
        else
            final_color.g = 0.0;

        if(final_color.b > 0.9)
            final_color.b = 1.0;
        else if(final_color.b > 0.75)
            final_color.b = 0.75;
        else if(final_color.b > 0.5)
            final_color.b = 0.5;
        else if(final_color.b > 0.35)
            final_color.b = 0.25;
        else
            final_color.b = 0.0;
    }
}
