#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

uniform mat4 M;
uniform mat4 M_inv_transpose;
uniform mat4 view_projection;
uniform vec3 camera_position;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} vsout;

void main() {
    vsout.world = (M * vec4(position, 1.0)).xyz;
    vsout.view = camera_position - vsout.world;
    vsout.normal = normalize((M_inv_transpose * vec4(normal, 0.0)).xyz);
    gl_Position = view_projection * vec4(vsout.world, 1.0);
    vsout.color = color;
    // vsout.color = vec4(1.0, 1.0, 1.0, 1.0);
    vsout.tex_coord = tex_coord;
}