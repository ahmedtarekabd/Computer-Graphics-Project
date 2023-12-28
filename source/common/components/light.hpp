#pragma once

#include "../ecs/component.hpp"

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <shader/shader.hpp>

namespace our
{

    // This component denotes that any renderer should apply Light on the scene.
    class LightComponent : public Component
    {

        // Light source index
        static int currentLightIndex;
        // Number of lights
        static int lightCount;

    public:
        ShaderProgram *shader;

        // // Any light component should have a transform component
        // TransformComponent *transform;

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

        // Reads light parameters from the given json object
        virtual void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        virtual void setUniforms();
    };

    //* Directional Types
    class DirectionalLight : public LightComponent
    {
    public:
        // The direction of the light
        glm::vec3 direction;

        static std::string getID() { return "DirectionalLight"; }
        void deserialize(const nlohmann::json &data) override;
        void setUniforms() override;
    };

    //* Point Types
    class PointLight : public LightComponent
    {
    public:
        // Position of the light
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        // Attenuation of the light
        glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);

        // The ID of this component type is "PointLight"
        static std::string getID() { return "PointLight"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        void setUniforms() override;
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

        // The ID of this component type is "SpotLight"
        static std::string getID() { return "SpotLight"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json &data) override;

        // Set uniform values for the shader
        void setUniforms() override;
    };

}
