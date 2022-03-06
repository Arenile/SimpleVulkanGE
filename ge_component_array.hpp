// This header is for a Component Array for the E-C System
#pragma once

// std includes
#include <unordered_map>

// Project includes
#include "ge_entity_manager.hpp"

namespace ge
{

// We need an interface for component array to check for distroyed entities
class IComponentArray
{
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class GeComponentArray : public IComponentArray
{
    public:
        void insertData(Entity entity, T component);
        void removeData(Entity entity);

        T& getData(Entity entity)
        {
            assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retreiving non-existent component!");

            // Returns a reference to the entity's component
            return mComponentArray[mEntityToIndexMap[entity]];
        }

        void entityDestroyed(Entity entity) override
        {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
            {
                removeData(entity);
            }
        }
    private:
        // Packed array of components (generic type T)
        // set to a specified maximum amount, matching the max number
        // of entities allowed to exist simultaneously, so that each
        // entity has a unique spot
        std::array<T, MAX_ENTITIES> mComponentArray;

        // Map from an entity ID to an array index
        std::unordered_map<Entity, size_t> mEntityToIndexMap;
        
        // Map from an array index to an entity ID
        std::unordered_map<size_t, Entity> mIndexToEntityMap;

        // Total size of valid entries in the array
        size_t mSize;
};

}