#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "../ecs/world.hpp"
#include "../components/Collision.hpp"
#include "../application.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
using namespace std;

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
            std::vector<CollisionComponent *> monsters;

            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                auto collision = entity->getComponent<CollisionComponent>();
                // If the movement component exists
                if (collision)
                {

                    if (collision->getOwner()->name == "player")
                    {
                        player = entity;
                    }
                    if (collision->getOwner()->name == "target")
                    {
                        target = entity;
                    }
                    if (collision->getOwner()->name == "monster")
                    {
                        monsters.push_back(collision);
                    }
                }
            }

            auto playercenter = player->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
            CollisionComponent *m;
            for (int i = 0; i < monsters.size(); i++)
            {
                m = monsters[i];
                auto mcenter = m->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                playercenter.y = 0;
                mcenter.y += 1;
                auto pdistance = glm::distance(playercenter, mcenter);
                if (pdistance < 1.5)
                {
                    /// HENA hanro7 ll lose state
                    app->changeState("exit");
                }
            }

            auto targetdirection = target->getLocalToWorldMatrix() * glm::vec4(0.0, 0.0, -1.0, 0.0);
            targetdirection = normalize(targetdirection);

            if (app->getKeyboard().isPressed(GLFW_KEY_R))
            {
                for (int j = 0; j < monsters.size(); j++)
                {
                    m = monsters[j];
                    auto mcenter = m->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    mcenter.y += 1.5;
                    cout << "(" << mcenter.x << "," << mcenter.y << "," << mcenter.z << ")" << std::endl;
                    auto targetcenter = target->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    glm::vec4 Voc = mcenter - targetcenter;
                    glm::vec4 Poc = Voc * glm::dot(glm::normalize(Voc), targetdirection);
                    float d = ::sqrtf(glm::dot(Voc, Voc) - glm::dot(Poc, Poc));
                    cout << d;
                    if (d < 1)
                    {
                        auto it = monsters.begin() + j;
                        monsters.erase(it);
                        world->markForRemoval(m->getOwner());
                        world->deleteMarkedEntities();
                        if (monsters.size() == 0)
                        {
                            /// hena elwin
                            cout << "win";
                        }
                    }


                }
            }
        }
    };
}
