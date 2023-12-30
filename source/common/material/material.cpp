#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"
#include "../texture/texture-utils.hpp"

#include <iostream>

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {

        // TODO: (Req 7) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {

        // TODO: (Req 7) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {

        // TODO: (Req 7) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);

        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        // Then we bind the sampler to unit 0 if there is a sampler used
        // Not used in menu-state materials
        if (sampler)
            sampler->bind(0);
        // Then we send 0 (the index of the texture unit we used above) to the "tex" uniform
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void TexturedLitMaterial::setup() const
    {

        // TODO: (Req 7) Write this function
        Material::setup();
        shader->set("alphaThreshold", alphaThreshold);
        shader->set("textured_material.albedo_tint", albedo_tint);
        shader->set("textured_material.specular_tint", specular_tint);
        shader->set("textured_material.emissive_tint", emissive_tint);

        // std::cout << "setup textured lit material albedo: " << albedo_map << std::endl;
        // std::cout << "setup textured lit material roughness: " << roughness_map << std::endl;
        // std::cout << "setup textured lit material specular: " << specular_map << std::endl;

        if (albedo_map)
        {
            // Bind the first albedo_map to unit 1
            glActiveTexture(GL_TEXTURE1);
            albedo_map->bind();
            sampler->bind(1);
            shader->set("textured_material.albedo_map", 1);
        }

        // Optional: Not all materials have specular map
        if (specular_map)
        {
            // Bind the first specular_map to unit 2
            glActiveTexture(GL_TEXTURE2);
            specular_map->bind();
            sampler->bind(2);
            shader->set("textured_material.specular_map", 2);
        }

        if (ambient_occlusion_map)
        {
            // Bind the first ambient_occlusion_map to unit 3
            glActiveTexture(GL_TEXTURE3);
            ambient_occlusion_map->bind();
            sampler->bind(3);
            shader->set("textured_material.ambient_occlusion_map", 3);
        }

        if (roughness_map)
        {
            // Bind the first roughness_map to unit 4
            glActiveTexture(GL_TEXTURE4);
            roughness_map->bind();
            sampler->bind(4);
            shader->set("textured_material.roughness_map", 4);
        }

        // Optional: Not all materials have emissive map
        if (emissive_map)
        {
            // Bind the first roughness_map to unit 5
            glActiveTexture(GL_TEXTURE5);
            emissive_map->bind();
            sampler->bind(5);
            shader->set("textured_material.emissive_map", 5);
        }

        if (normal_map)
        {
            // Bind the first roughness_map to unit 6
            glActiveTexture(GL_TEXTURE6);
            normal_map->bind();
            sampler->bind(6);
            shader->set("textured_material.normal_map", 6);
        }
    }

    // This function read the material data from a json object
    void TexturedLitMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;

        alphaThreshold = data.value("alphaThreshold", 0.0f);

        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo_map", ""));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular_map", ""));
        ambient_occlusion_map = AssetLoader<Texture2D>::get(data.value("ambient_occlusion_map", ""));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness_map", ""));
        emissive_map = AssetLoader<Texture2D>::get(data.value("emissive_map", ""));

        normal_map = AssetLoader<Texture2D>::get(data.value("normal_map", ""));

        albedo_tint = data.value("albedo_tint", glm::vec3(1.0f));
        specular_tint = data.value("specular_tint", glm::vec3(1.0f));
        emissive_tint = data.value("emissive_tint", glm::vec3(1.0f));

        if (!albedo_map)
            albedo_map = texture_utils::textureColor(data.value("albedo_tint", glm::vec4(1.0f)));
        if (!specular_map)
            specular_map = texture_utils::textureColor(glm::vec4(1.0f));
        if (!ambient_occlusion_map)
            ambient_occlusion_map = texture_utils::textureColor(glm::vec4(0.15f));
        if (!roughness_map)
            roughness_map = texture_utils::textureColor(glm::vec4(1.0f));
        if (!emissive_map)
            emissive_map = texture_utils::textureColor(data.value("emissive_tint", glm::vec4(0.0f)));

        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}