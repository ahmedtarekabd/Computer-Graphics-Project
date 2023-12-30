#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class MovementSystem
    {
    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            Entity *player;
            for (auto entity : world->getEntities())
            {

                if (entity->name == "player")
                {
                    player = entity;
                }

                // Get the movement component if it exists
            }

            for (auto entity : world->getEntities())
            {
                MovementComponent *movement = entity->getComponent<MovementComponent>();
                // If the movement component exists
                if (movement)
                {

                    if (entity->name == "monster")
                    {
                        auto direction = player->localTransform.position - entity->localTransform.position;
                        direction = normalize(direction);
                        direction.y =0;
                        movement->linearVelocity = direction * 1.1f;

                        auto directionangle= atan2(direction.x,direction.z);

                       entity->localTransform.rotation.y =directionangle;
                    }
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    entity->localTransform.position += deltaTime * movement->linearVelocity;
                    // entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                }
            }
        }
    };

}
