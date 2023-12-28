#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

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

        // glActiveTexture(GL_TEXTURE0);
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

        // Bind the first albedo_map to unit 0
        glActiveTexture(GL_TEXTURE0);
        albedo_map->bind();
        if (sampler)
            sampler->bind(0);
        shader->set("textured_material.albedo_map", 0);

        // Optional: Not all materials have specular map
        if (specular_map)
        {
            // Bind the first specular_map to unit 1
            glActiveTexture(GL_TEXTURE1);
            specular_map->bind();
            if (sampler)
                sampler->bind(1);
            shader->set("textured_material.specular_map", 1);
        }

        if (ambient_occlusion_map)
        {
            // Bind the first ambient_occlusion_map to unit 2
            glActiveTexture(GL_TEXTURE2);
            ambient_occlusion_map->bind();
            if (sampler)
                sampler->bind(2);
            shader->set("textured_material.ambient_occlusion_map", 2);
        }

        // Bind the first roughness_map to unit 3
        glActiveTexture(GL_TEXTURE3);
        roughness_map->bind();
        if (sampler)
            sampler->bind(3);
        shader->set("textured_material.roughness_map", 3);

        // Optional: Not all materials have emissive map
        if (emissive_map)
        {
            // Bind the first roughness_map to unit 4
            glActiveTexture(GL_TEXTURE4);
            emissive_map->bind();
            if (sampler)
                sampler->bind(4);
            shader->set("textured_material.roughness_map", 4);
        }
    }

    // This function read the material data from a json object
    void TexturedLitMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;

        albedo_map = AssetLoader<Texture2D>::get(data.value("albedo_map", ""));
        specular_map = AssetLoader<Texture2D>::get(data.value("specular_map", ""));
        ambient_occlusion_map = AssetLoader<Texture2D>::get(data.value("ambient_occlusion_map", ""));
        roughness_map = AssetLoader<Texture2D>::get(data.value("roughness_map", ""));
        emissive_map = AssetLoader<Texture2D>::get(data.value("emissive_map", ""));

        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}