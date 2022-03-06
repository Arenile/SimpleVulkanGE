// Implementation for entity manager
// std includes

// Project includes
#include "ge_entity_manager.hpp"

namespace ge
{

GeEntityManager::GeEntityManager()
{
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
    {
        mAvailableEntities.push(entity);
    }
}

Entity GeEntityManager::createEntity()
{
    assert(mLivingEntityCount < MAX_ENTITIES && "UNABLE TO CREATE ENTITY, MAX REACHED!");

    Entity id = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mLivingEntityCount;

    return id;
}

void GeEntityManager::destroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "ENTITY OUT OF RANGE!");

    // Invalidate destroyed entity's signature
    mSignatures[entity].reset();

    // Put the destroyed ID at the back of the queue
    mAvailableEntities.push(entity);
    --mLivingEntityCount;
}

void GeEntityManager::setSignature(Entity entity, Signature signature) 
{
    assert(entity < MAX_ENTITIES && "ENTITY OUT OF RANGE!");

    // Put this entity's signature into the array
    mSignatures[entity] = signature;
}

Signature GeEntityManager::getSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "ENTITY OUT OF RANGE!");

    // Get the signature of this entity from the array
    return mSignatures[entity];
}

}