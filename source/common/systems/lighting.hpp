#pragma once

#include "../ecs/world.hpp"
#include "../components/light.hpp"
#include "../components/transform.hpp"
#include <shader/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The light system is responsible for moving every entity which contains a lightComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/light.hpp"
    class LightingSystem
    {

        ShaderProgram *shader = nullptr;

    public:
        // Load Light Components to the shader
        void initialize(World *world, const nlohmann::json &config)
        {

            // Get the shader program
            shader = AssetLoader<ShaderProgram>::get(config["shader"].get<std::string>());

            for (auto entity : world->getEntities())
            {
                if (entity->name == "Light")
                {
                    // Get the light component
                    LightComponent *light = entity->getComponent<LightComponent>();
                    // Set light count
                    light->setup();
                }
            }
        }
        // Clean up the renderer
        void destroy();
    };

}
