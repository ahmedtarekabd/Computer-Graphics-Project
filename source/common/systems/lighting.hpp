#pragma once

#include "../ecs/world.hpp"
#include "../components/light.hpp"
#include <shader/shader.hpp>
#include "../asset-loader.hpp"
#include "../deserialize-utils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include <glm/vec3.hpp>

namespace our
{

    // The light system is responsible for storing and updating all lightComponents in the Scene.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/light.hpp"

    class LightingSystem
    {

        ShaderProgram *shader = nullptr;
        std::vector<LightComponent *> lights;

        glm::vec3 defaultAmbient;

    public:
        // Load Light Components to the shader
        void initialize(World *world, const nlohmann::json &config)
        {

            std::cout << "LightingSystem::initialize" << std::endl;

            // Get the shader program
            shader = AssetLoader<ShaderProgram>::get(config["shader"].get<std::string>());

            defaultAmbient = config.value("ambient", glm::vec3(0.15f, 0.15f, 0.15f));

            for (auto entity : world->getEntities())
            {

                // Get the light component
                LightComponent *light = entity->getComponent<LightComponent>();

                // If entity is light component
                if (light)
                {

                    // Set default ambient if not specified in the light entity (.json file)
                    if (light->ambient == glm::vec3(0.0f, 0.0f, 0.0f))
                        light->ambient = defaultAmbient;

                    // Set Uniforms light uniforms
                    light->setUniforms(shader);
                    // Add light to the lights vector
                    lights.push_back(light);
                    light->lightIndex = (int)lights.size() - 1;
                }
            }

            std::cout << "LightingSystem::initialize: lightCount = " << lights.size() << std::endl;
            LightComponent::lightCount = (int)lights.size();
            LightComponent::setNumberOfLightComponents(shader);
        }

        // Update Light Components
        void update()
        {
            // Set light count
            for (LightComponent *light : lights)
                light->setUniforms(shader);
        }

        // Clean up the lighting system
        void destroy()
        {
            shader = nullptr;
            lights.clear();
            LightComponent::currentLightIndex = 0;
            LightComponent::lightCount = 0;
        }
    };

}
