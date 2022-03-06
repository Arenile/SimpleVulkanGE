// This file is part of implementing an E-C System as seen here: 
// https://austinmorlan.com/posts/entity_component_system/

#pragma once

// std includes
#include <cstdint>
#include <queue>
#include <array>
#include <cassert>
#include <bitset>

// Project includes

namespace ge
{

// Type alias for entities and signature
using Entity = std::uint32_t;
using Signature = std::bitset<32>;
using ComponentType = std::uint8_t;

// Defines the size of the entity arrays
const Entity MAX_ENTITIES = 5000;
const ComponentType MAX_COMPONENTS = 32;

class GeEntityManager
{
    public:
        GeEntityManager();

        Entity createEntity();
        void destroyEntity(Entity entity);
        
        void setSignature(Entity entity, Signature signature);
        Signature getSignature(Entity entity);

    private:
        // Queue of unused entity IDs
        std::queue<Entity> mAvailableEntities{};

        // Array of signatures where index corresponds to entity ID
        std::array<Signature, MAX_ENTITIES> mSignatures{};

        // Total living entities for tracking limits
        uint32_t mLivingEntityCount{};

};

}