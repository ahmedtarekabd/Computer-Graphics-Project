#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fsin;

out vec4 frag_color;

struct TexturedMaterial {
    sampler2D albedo_map;
    sampler2D roughness_map;
    sampler2D ambient_occlusion_map;
    sampler2D specular_map;
    sampler2D emissive_map;
};

uniform TexturedMaterial textured_material;

void main() {

    frag_color = fsin.color * vec4(texture(textured_material.albedo_map, fsin.tex_coord).rgb, 1.0);
    // frag_color = vec4(1.0, 1.0, 1.0, 1.0);
    return;

}