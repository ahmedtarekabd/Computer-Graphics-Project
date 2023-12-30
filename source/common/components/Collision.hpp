#pragma once
#include "../ecs/component.hpp"
#include <glm/glm.hpp>

namespace our
{
    class CollisionComponent : public Component
    {
    public:
        float CollisionRadius = 1.0f;

        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads CollisionRadius from the given json object
        void deserialize(const nlohmann::json &data) override;
    };
}