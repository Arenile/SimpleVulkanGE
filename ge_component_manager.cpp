// This implements the ComponentManager for the E-C System
// std includes

// Project includes
#include "ge_component_manager.hpp"

namespace ge
{

template<typename T> void GeComponentManager::registerComponent()
{
    const char* typeName = typeid(T).name();

    assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once!");

    // Add this component type to the component type map
    mComponentTypes.insert({typeName, mNextComponentType});

    // Create a ComponentArray pointer and add it to the component arrays map
    mComponentArrays.insert({typeName, std::make_shared<GeComponentArray<T>>()});

    // Increment the value so the next component registered will be different
    ++mNextComponentType;
}

template<typename T> ComponentType GeComponentManager::getComponentType()
{
    const char* typeName = typeid(T).name();

    assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use!");

    return mComponentTypes[typeName];
}

template<typename T> void GeComponentManager::addComponent(Entity entity, T component)
{
    // Add a component to the array for an entity
    getComponentArray<T>()->insertData(entity, component);
}

template<typename T> void GeComponentManager::removeComponent(Entity entity)
{
    // Remove a component from the array for an entity
    getComponentArray<T>()->removeData(entity);
}

template<typename T> T& GeComponentManager::getComponent(Entity entity)
{
    // Get a reference to a component from the array for an entity
    return getComponentArray<T>()->getData(entity);
}

}