// Header for the implementation of componenets for the E-C System
#pragma once

// std includes
#include <memory>

// Project includes
#include "ge_component_array.hpp"

namespace ge
{

class GeComponentManager
{
    public:
        template<typename T> void registerComponent();

        template<typename T> ComponentType getComponentType();

        template<typename T> void addComponent(Entity entity, T component);

        template<typename T> void removeComponent(Entity entity);

        template<typename T> T& getComponent(Entity entity);

        void entityDestroyed(Entity entity)
        {
            // Notify each component array that an entity has been destroyed
            // If it has a component for that entity, it will be removed
            for (auto const& pair: mComponentArrays)
            {
                auto const& component = pair.second;

                component->entityDestroyed(entity);
            }
        }
    
    private:
        // Map from type string pointer to a component type
        std::unordered_map<const char*, ComponentType> mComponentTypes{};

        // Map from type string pointer to a component array
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

        // The component type to be assigned to the next registered component - starts at 0
        ComponentType mNextComponentType{};

        // Helper function for getting static cast pointer to the ComponentArray of type T
        template<typename T> std::shared_ptr<GeComponentArray<T>> getComponentArray();
};

}