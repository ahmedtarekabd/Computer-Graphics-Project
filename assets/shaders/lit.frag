#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fsin;

#define TYPE_DIRECTIONAL 0
#define TYPE_POINT 1
#define TYPE_SPOT 2

struct Light {
    int type;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 position;
    vec3 direction;
    // Point Light Specific
    vec3 attenuation;
    // Spot Light Specific
    float inner_angle, outer_angle;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 emissive;

    float shininess;
};

struct TexturedMaterial {
    sampler2D albedo_map;
    vec3 albedo_tint;
    sampler2D specular_map;
    vec3 specular_tint;
    sampler2D ambient_occlusion_map;
    sampler2D roughness_map;
    vec2 roughness_range;
    sampler2D emissive_map;
    vec3 emissive_tint;
};

// If the material is not textured
uniform Material material = Material(vec3(1.0), vec3(1.0), vec3(1.0), vec3(1.0), -1.0);    // Initalize shininess with -1 to indicate that the material is textured

uniform TexturedMaterial textured_material;

// If the material is textured
Material sample_material(TexturedMaterial tex_mat, vec2 tex_coord) {
    Material mat;

    mat.diffuse = tex_mat.albedo_tint * texture(tex_mat.albedo_map, tex_coord).rgb;
    // mat.specular = tex_mat.specular_tint * texture(tex_mat.specular_map, tex_coord).rgb;
    // mat.emissive = tex_mat.emissive_tint * texture(tex_mat.emissive_map, tex_coord).rgb;
    mat.ambient = mat.diffuse * texture(tex_mat.ambient_occlusion_map, tex_coord).r;

    float roughness = mix(tex_mat.roughness_range.x, tex_mat.roughness_range.y, texture(tex_mat.roughness_map, tex_coord).r);
    mat.shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;

    return mat;
}

#define MAX_LIGHT_COUNT 16
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;

out vec4 frag_color;

void main() {
    vec3 normal = normalize(fsin.normal);
    vec3 view = normalize(fsin.view);
    int count = int(min(light_count, MAX_LIGHT_COUNT));
    vec3 accumulated_light = vec3(0.0);

    Material acutalMaterial = material;

    // If the material is textured. If the material is textured, the shininess will be -1
    if (material.shininess < 0.0)
        acutalMaterial = sample_material(textured_material, fsin.tex_coord);
    

    for (int index = 0; index < count; index++) {
        Light light = lights[index];
        vec3 light_direction;
        float attenuation = 1;

        if (light.type == TYPE_DIRECTIONAL)
            light_direction = light.direction;
        // Point or Spot Light
        else {
            // Fragment to Light Direction
            light_direction = fsin.world - light.position;
            float distance = length(light_direction);   // Magnitude of the vector
            light_direction /= distance;    // Normalize the vector

            // Decrease the intensity of the light based on the distance
            attenuation *= 1.0 / dot(light.attenuation, vec3(distance * distance, distance, 1.0));

            if (light.type == TYPE_SPOT) {
                // Angle between the Center light direction and the fragment to light direction
                float angle = acos(dot(light.direction, light_direction));
                // Decrease the intensity of the light based on the angle
                // Kol ma el angle byzhr 3la el outer angle by2arab men el 0
                // Kol ma el angle byzhr 3la el inner angle by2arab men el 1
                attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
            }
        }

        vec3 reflected = reflect(light_direction, normal);
        float lambert = max(0.0, dot(normal, -light_direction));
        float phong = pow(max(0.0, dot(view, reflected)), acutalMaterial.shininess);
        vec3 diffuse = acutalMaterial.diffuse * light.diffuse * lambert;
        // vec3 specular = acutalMaterial.specular * light.specular * phong;
        vec3 ambient = acutalMaterial.ambient * light.ambient;
        accumulated_light += (diffuse + specular) * attenuation + ambient;
    }

    frag_color = fsin.color * vec4(accumulated_light, 1.0);
}