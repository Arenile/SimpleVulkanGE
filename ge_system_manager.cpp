// Implementation of the System Manager for the E-C System
// std includes

// Project includes
#include "ge_system_manager.hpp"

namespace ge
{

template<typename T> std::shared_ptr<T> GeSystemManager::registerSystem()
{
    const char* typeName = typeid(T).name();

    assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once!");

    // Create a pointer to the system and return it so it can be used externally
    auto system = std::make_shared<T>();
    mSystems.insert({typeName, system});
    return system;
}

template<typename T> void GeSystemManager::setSignature(Signature signature)
{
    const char* typeName = typeid(T).name();

    assert(mSystems.find(typeName) != mSystems.end() && "Systems used before registered!");

    // Set the signatures for this system
    mSignatures.insert({typeName, signature});
}

void GeSystemManager::entityDestroyed(Entity entity)
{
    // Erase a destroyed entity from all system lists
    // mEntities is a set so no check needed for its existence
    for (auto const& pair : mSystems)
    {
        auto const& system = pair.second;

        system->mEntities.erase(entity);
    }
}

void GeSystemManager::entitySignatureChanged(Entity entity, Signature entitySignature)
{
    // Notify each system that an entity's signature changed
    for (auto const& pair : mSystems)
    {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSignatures[type];

        // Entity signature matches system signature - insert into set
        if ((entitySignature & systemSignature) == systemSignature)
        {
            system->mEntities.erase(entity);
        }
        // Entity signature does not match system signature - erase from set
        else 
        {
            system->mEntities.erase(entity);
        }
    }
}

}