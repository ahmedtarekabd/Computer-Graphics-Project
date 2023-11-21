#pragma once

#include <unordered_set>
#include "entity.hpp"

namespace our
{

    // This class holds a set of entities
    class World
    {
        std::unordered_set<Entity *> entities;         // These are the entities held by this world
        std::unordered_set<Entity *> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                       // when deleteMarkedEntities is called
    public:
        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json &data, Entity *parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity *add()
        {
            // TODO: (Req 8) Create a new entity, set its world member variable to this,
            //  and don't forget to insert it in the suitable container.
            Entity *newentity = new Entity(); // crete new entity
            newentity->world = this;          // set its world to this world
            entities.insert(newentity);       // insert in the entites list of this world
            return newentity;                 // returns new entity
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity *> &getEntities()
        {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity *entity)
        {
            // TODO: (Req 8) If the entity is in this world, add it to the "markedForRemoval" set.
            auto it = entities.find(entity); // search for the entity
            if (it != entities.end())
            {
                // entity found
                Entity *foundEntity = *it;
                markedForRemoval.insert(foundEntity); // insert it in the marked for removal list
            }
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities()
        {
            // TODO: (Req 8) Remove and delete all the entities that have been marked for removal
            Entity *delentity;                                                           // this is for the entity we want to delete
            for (auto it = markedForRemoval.begin(); it != markedForRemoval.end(); ++it) // loop through marked for removal list
            {
                delentity = *it;
                entities.erase(delentity); // erase it from entities list
                delete *it;                // delete it
            }
            markedForRemoval.clear(); // clear the marked for removal list
        }

        // This deletes all entities in the world
        void clear()
        {
            // TODO: (Req 8) Delete all the entites and make sure that the containers are empty
            for (auto it = entities.begin(); it != entities.end(); ++it) // loop through enties list
            {
                delete *it; // delete each one
            }
            entities.clear();         // clear the entities list
            markedForRemoval.clear(); // clear the marked for removal list
        }

        // Since the world owns all of its entities, they should be deleted alongside it.
        ~World()
        {
            clear();
        }

        // The world should not be copyable
        World(const World &) = delete;
        World &operator=(World const &) = delete;
    };

}