#pragma once

#include "../ecs/component.hpp"

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <shader/shader.hpp>

#include <iostream>

namespace our
{

    // This component denotes that any renderer should apply Light on the scene.
    class LightComponent : public Component
    {

    public:
        // Light source index
        static int currentLightIndex;
        // Number of lights
        static int lightCount;

        ShaderProgram *shader;

        // Light source index
        int lightIndex;
        // Light type
        int type;

        // Any Light should have a color
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 ambient;

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }
        // The set number of light components to the shader
        static void setNumberOfLightComponents(ShaderProgram *shader)
        {
            std::cout << "LightComponent::setNumberOfLightComponents" << lightCount << std::endl;
            shader->use();
            shader->set("lightCount", lightCount);
        }

        // Reads light parameters from the given json object
        virtual void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        virtual void setUniforms(ShaderProgram *shader);
    };

    //* Directional Types
    class DirectionalLight : public LightComponent
    {
    public:
        // The direction of the light
        glm::vec3 direction;

        static std::string getID() { return "Directional Light"; }
        void deserialize(const nlohmann::json &data) override;
        void setUniforms(ShaderProgram *shader) override;
    };

    //* Point Types
    class PointLight : public LightComponent
    {
    public:
        // Position of the light
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        // Attenuation of the light
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);

        // The ID of this component type is "Point Light"
        static std::string getID() { return "Point Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        void setUniforms(ShaderProgram *shader) override;
    };

    //* Spot Types
    class SpotLight : public LightComponent
    {
    public:
        // Position of the light
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        // The direction of the light
        glm::vec3 direction;

        // Attenuation of the light
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
        // Spot Light Angles
        float inner_angle = 0.0f, outer_angle = 0.0f;

        // The ID of this component type is "Spot Light"
        static std::string getID() { return "Spot Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        void setUniforms(ShaderProgram *shader) override;
    };

}
