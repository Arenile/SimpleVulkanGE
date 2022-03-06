// This implements the Component Array for the E-C System
// std includes

// Project includes
#include "ge_component_array.hpp"

namespace ge
{

template<typename T> void GeComponentArray<T>::insertData(Entity entity, T component)
{
    assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to the same entity more than once!");

    // Put new entry at the end and update the maps
    size_t newIndex = mSize;
    mEntityToIndexMap[entity] = newIndex;
    mIndexToEntityMap[newIndex] = entity;
    mComponentArray[newIndex] = component;
    ++mSize;
}

template<typename T> void GeComponentArray<T>::removeData(Entity entity)
{
    assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing a non-existent component!");

    // Copy element at end into deleted element's place to maintain density
    size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
    size_t indexOfLastElement = mSize - 1;
    mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

    // Update map to point to moved spot
    Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
    mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    mEntityToIndexMap.erase(entity);
    mIndexToEntityMap.erase(indexOfLastElement);

    --mSize;
}

}