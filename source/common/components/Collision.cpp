#include "Collision.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    // Reads CollisionRadius from the given json object
    void CollisionComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object()) return;
        CollisionRadius = data.value("CollisionRadius", CollisionRadius);
    }
}