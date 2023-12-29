#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp" // For deserializeVec3 with default value of glm::vec3

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define TYPE_DIRECTIONAL 0
#define TYPE_POINT 1
#define TYPE_SPOT 2

namespace our
{

    // Static variables
    int LightComponent::currentLightIndex = 0;
    int LightComponent::lightCount = 0;

    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        std::cout << "LightComponent::deserialize" << std::endl;

        type = TYPE_DIRECTIONAL;

        std::string componentType = data.value("type", "");
        if (componentType == "Directional Light")
            type = TYPE_DIRECTIONAL;
        else if (componentType == "Point Light")
            type = TYPE_POINT;
        else if (componentType == "Spot Light")
            type = TYPE_SPOT;
        else
            std::cout << "Light type is wrong or not specified." << std::endl;

        diffuse = data.value("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        specular = data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f));
        ambient = data.value("ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        // Set light index with current light index
        lightIndex = currentLightIndex;
        // Increment current light index
        currentLightIndex++;

        // shader = nullptr;
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
    }

    // Set uniform values for the shader
    void LightComponent::setup()
    {

        // Send total number of light sources to the shader
        shader->set("lightCount", lightCount);

        // Set light vector uniform values in the shader
        // Source: https://stackoverflow.com/questions/23591264/how-to-pass-uniform-array-of-struct-to-shader-via-c-code
        shader->set("lights[" + std::to_string(lightIndex) + "].type", type);
        shader->set("lights[" + std::to_string(lightIndex) + "].diffuse", diffuse);
        shader->set("lights[" + std::to_string(lightIndex) + "].specular", specular);
        shader->set("lights[" + std::to_string(lightIndex) + "].ambient", ambient);
    }

    // ********** Directional Light **********
    void DirectionalLight::deserialize(const nlohmann::json &data)
    {
        deserialize(data);

        glm::mat4 model_matrix = this->getOwner()->getLocalToWorldMatrix();

        // direction = glm::vec3(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2]);

        glm::quat rotation = glm::quat_cast(model_matrix);
        glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));
        direction = glm::normalize(eulerAngles);
    }

    void DirectionalLight::setup()
    {
        setup();
        shader->set("lights[" + std::to_string(lightIndex) + "].direction", direction);
    }

    // ********** Point Light **********
    void PointLight::deserialize(const nlohmann::json &data)
    {
        deserialize(data);
        glm::mat4 model_matrix = this->getOwner()->getLocalToWorldMatrix();
        position = glm::vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]);

        // position = data.value("position", glm::vec3(0.0f, 0.0f, 0.0f));
        attenuation = data.value("attenuation", glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void PointLight::setup()
    {
        setup();
        shader->set("lights[" + std::to_string(lightIndex) + "].position", position);
        shader->set("lights[" + std::to_string(lightIndex) + "].attenuation", attenuation);
    }

    // ********** Spot Light **********
    void SpotLight::deserialize(const nlohmann::json &data)
    {
        deserialize(data);

        glm::mat4 model_matrix = this->getOwner()->getLocalToWorldMatrix();
        position = glm::vec3(model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]);
        // direction = glm::vec3(model_matrix[0][0], model_matrix[1][1], model_matrix[2][2]);

        glm::quat rotation = glm::quat_cast(model_matrix);
        glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));
        direction = glm::normalize(eulerAngles);

        attenuation = data.value("attenuation", glm::vec3(0.0f, 0.0f, 0.0f));
        inner_angle = data.value("inner_angle", 0.0f);
        outer_angle = data.value("outer_angle", 0.0f);
    }

    void SpotLight::setup()
    {
        setup();
        shader->set("lights[" + std::to_string(lightIndex) + "].position", position);
        shader->set("lights[" + std::to_string(lightIndex) + "].direction", direction);
        shader->set("lights[" + std::to_string(lightIndex) + "].attenuation", attenuation);
        shader->set("lights[" + std::to_string(lightIndex) + "].inner_angle", inner_angle);
        shader->set("lights[" + std::to_string(lightIndex) + "].outer_angle", outer_angle);
    }

}