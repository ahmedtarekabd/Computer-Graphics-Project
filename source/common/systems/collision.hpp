#pragma once
#include <vector>
#include <iostream>
#include "../ecs/world.hpp"
#include "../components/Collision.hpp"
#include "../application.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem
    {
        Application *app;

    public:
        // This should be called every frame to update all entities containing a MovementComponent.
        void enter(Application *app)
        {
            this->app = app;
        }

        // void update(World *world, float deltaTime)
        // {
        //     CollisionComponent *c;
        //     // For each entity in the world
        //     for (auto entity : world->getEntities())
        //     {
        //         // Get the movement component if it exists
        //         c = entity->getComponent<CollisionComponent>();
        //         // If the movement component exists
        //         if (c)
        //         {
        //             std::cout << "f";
        //             // Change the position and rotation based on the linear & angular velocity and delta time.
        //         }
        //     }
        // }

        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            Entity *player;
            Entity *target;
            std::vector<CollisionComponent *> collisionentities;
    
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                auto collision = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if (collision)
                {
                    collisionentities.push_back(collision);
                    if (collision->getOwner()->name == "player")
                    {
                        player = entity;
                    }
                    if (collision->getOwner()->name == "target")
                    {
                        target = entity;
                    }
                }
            }

            CollisionComponent *firstentity;
            CollisionComponent *seconedentity;
            for (int i = 0; i < collisionentities.size(); i++)
            {
                firstentity = collisionentities[i];
                auto firstcenter = firstentity->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                for (int j = i + 1; j < collisionentities.size(); j++)
                {
                    seconedentity = collisionentities[j];
                    auto seconedcenter = seconedentity->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    auto distance = glm::distance(firstcenter, seconedcenter);
                    //std::cout << distance;
                    if (distance < 1.3)
                    {
                        if (firstentity->getOwner()->name == "player" && seconedentity->getOwner()->name == "monster")
                        {
                            world->markForRemoval(seconedentity->getOwner());
                            world->deleteMarkedEntities();
                            std::cout << "die";
                        }
                        if (firstentity->getOwner()->name == "monster" && seconedentity->getOwner()->name == "player")
                        {
                            world->markForRemoval(firstentity->getOwner());
                            world->deleteMarkedEntities();
                            std::cout << "die2";
                        }
                    }
                }

                for (int j = i + 1; j < collisionentities.size(); j++)
                {

                    seconedentity = collisionentities[j];
                    auto seconedcenter = seconedentity->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    if (firstentity->getOwner()->name == "target" && seconedentity->getOwner()->name == "monster" || firstentity->getOwner()->name == "monster" && seconedentity->getOwner()->name == "target")
                    {
                        auto distance = glm::distance(glm::vec2(firstcenter), glm::vec2(seconedcenter));
                        //std::cout << distance;
                        if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_RIGHT))
                        {
                            std::cout << "c";
                            if (distance < 1)
                            {
                                std::cout << "p";
                                if (seconedentity->getOwner()->name == "monster")
                                {
                                    seconedentity->CollisionRadius--;
                                    std::cout << "-";
                                    if (seconedentity->CollisionRadius == 0)
                                    {
                                        world->markForRemoval(seconedentity->getOwner());
                                        world->deleteMarkedEntities();
                                    }
                                }
                                else
                                {
                                    firstentity->CollisionRadius--;
                                    if (firstentity->CollisionRadius == 0)
                                    {
                                        world->markForRemoval(firstentity->getOwner());
                                        world->deleteMarkedEntities();
                                    }

                                }
                            }
                        }
                    }

                }
            }
        }
    };
}
